# Abstract Syntax Tree Semantics

WebAssembly code is represented as an abstract syntax tree
that has a basic division between statements and
expressions. Each function body consists of exactly one statement.
All expressions and operations are typed, with no implicit conversions or
overloading rules.

Verification of WebAssembly code requires only a single pass with constant-time
type checking and well-formedness checking.

Why not a stack-, register- or SSA-based bytecode?
* Trees allow a smaller binary encoding: [JSZap][], [Slim Binaries][].
* [Polyfill prototype][] shows simple and efficient translation to asm.js.

  [JSZap]: https://research.microsoft.com/en-us/projects/jszap/
  [Slim Binaries]: https://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.108.1711
  [Polyfill prototype]: https://github.com/WebAssembly/polyfill-prototype-1

WebAssembly offers a set of operations that are language-independent but closely
match operations in many programming languages and are efficiently implementable
on all modern computers.

Some operations may *trap* under some conditions, as noted below. In the MVP,
trapping means that execution in the WebAssembly module is terminated and
abnormal termination is reported to the outside environment. In a JS
environment such as a browser, a trap results in throwing a JS exception.
If developer tools are active, attaching a debugger before the
termination would be sensible.

Callstack space is limited by unspecified and dynamically varying constraints.
If program callstack usage exceeds the available callstack space at any time,
a trap occurs.

## Types

### Basic Types

The following types are called the *basic types*:

  * `int32`: 32-bit integer
  * `int64`: 64-bit integer
  * `float32`: 32-bit floating point
  * `float64`: 64-bit floating point

Note that the local types `int32` and `int64` are not inherently signed or
unsigned. The interpretation of these types is determined by individual
operations.

Also note that there is no need for a `void` type; function signatures use
[sequences of types](Calls.md) to describe their return values, so a `void`
return type is represented as an empty sequence.

### Local Types

*Local types* are a superset of the basic types, adding the following:

  * `funcid`: a function identifier for use in `call_indirect`

The zero value of `funcid` is the identifier for the first function in the
function table. (C/C++ compilers may wish to put a placeholder function at
this point in the table to implement a null pointer concept.)

Parameters and local variables use local types.

### Memory Types

*Memory types* are a different superset of the local types, adding the
following:

  * `int8`: 8-bit integer
  * `int16`: 16-bit integer

Global variables and linear memory accesses use memory types.

## Linear Memory

The main storage of a WebAssembly module, called the *linear memory*, is a
contiguous, byte-addressable range of memory spanning from offset `0` and
extending for `memory_size` bytes which can be dynamically adjusted by
[`resize_memory`](Modules.md#resizing). The linear memory can be considered to
be an untyped array of bytes. The linear memory is sandboxed; it does not alias
the execution engine's internal data structures, the execution stack, local
variables, global variables, or other process memory. The initial state of
linear memory is specified by the [module](Modules.md#initial-state-of-linear-memory).

In the MVP, linear memory is not shared between threads of execution. Separate
modules can execute in separate threads but have their own linear memory and can
only communicate through messaging, e.g. in browsers using `postMessage`. It
will be possible to share linear memory between threads of execution when
[threads](PostMVP.md#threads) are added.

### Linear Memory Operations

Linear memory operations are annotated with a memory type and perform a
conversion between that memory type and a local type.

Loads read data from linear memory, convert from their memory type to a basic
type, and return the result:

  * `int32.load_sx[int8]`: sign-extend to int32
  * `int32.load_sx[int16]`: sign-extend to int32
  * `int32.load_zx[int8]`: zero-extend to int32
  * `int32.load_zx[int16]`: zero-extend to int32
  * `int32.load[int32]`: (no conversion)
  * `int64.load_sx[int8]`: sign-extend to int64
  * `int64.load_sx[int16]`: sign-extend to int64
  * `int64.load_sx[int32]`: sign-extend to int64
  * `int64.load_zx[int8]`: zero-extend to int64
  * `int64.load_zx[int16]`: zero-extend to int64
  * `int64.load_zx[int32]`: zero-extend to int64
  * `int64.load[int64]`: (no conversion)
  * `float32.load[float32]`: (no conversion)
  * `float64.load[float64]`: (no conversion)

Stores have an operand providing a value to store. They convert from the value's
local type to their memory type, and write the resulting value to linear memory:

  * `int32.store[int8]`: wrap int32 to int8
  * `int32.store[int16]`: wrap int32 to int16
  * `int32.store[int32]`: (no conversion)
  * `int64.store[int8]`: wrap int64 to int8
  * `int64.store[int16]`: wrap int64 to int16
  * `int64.store[int32]`: wrap int64 to int32
  * `int64.store[int64]`: (no conversion)
  * `float32.store[float32]`: (no conversion)
  * `float64.store[float64]`: (no conversion)

Wrapping of integers simply discards any upper bits; i.e. wrapping does not
perform saturation, trap on overflow, etc.

In addition to storing a value to linear memory, store instructions also
reproduce their value operand, with no conversion applied.

### Addressing

Each linear memory access operation also has an address operand and an immediate
integer byte offset attribute. The infinite-precision sum of the address
operand's value with the byte offset attribute's value is called the
*effective address*, which is interpreted as an unsigned byte index.

Linear memory accesses access the bytes starting at the location in the linear
memory storage indexed by the effective address, and extending for the number
of bytes implied by the memory type attribute of the access.

If any of the accessed bytes are beyond `memory_size`, the access is considered
*out-of-bounds*. A module may optionally define that out-of-bounds includes
small effective addresses close to `0`
(see [discussion](https://github.com/WebAssembly/design/issues/204)).
The semantics of out-of-bounds accesses are discussed
[below](AstSemantics.md#out-of-bounds).

The use of infinite-precision in the effective address computation means that
the addition of the offset to the address does is never wrapped, so if the
address for an access is out-of-bounds, the effective address will always also
be out-of-bounds. This is intended to simplify folding of offsets into complex
address modes in hardware, and to simplify bounds checking optimizations.

In the MVP, address operands and offset attributes have type `int32`, and linear
memory sizes are limited to 4 GiB (of course, actual sizes are further limited
by [available resources](Nondeterminism.md)). In the future, to support
[>4GiB linear memory](FutureFeatures.md#heaps-bigger-than-4gib), support for
indices with type `int64` will be added.

### Alignment

Each linear memory access operation also has an immediate positive integer power
of 2 alignment attribute. An alignment value which is the same as the memory
attribute size is considered to be a *natural* alignment.

If the effective address of a memory access is a multiple of the alignment
attribute value of the memory access, the memory access is considered *aligned*,
otherwise it is considered *misaligned*. Aligned and misaligned accesses have
the same behavior. Alignment affects performance as follows:

 * Aligned accesses with at least natural alignment are fast.
 * Aligned accesses with less than natural alignment may be somewhat slower
   (think: implementation makes multiple accesses, either in software or
    in hardware).
 * Misaligned access of any kind may be *massively* slower
   (think: implementation takes a signal and fixes things up)

Thus, it is recommend that WebAssembly producers align frequently-used data
to permit the use of natural alignment access, and use loads and stores with
the greatest alignment values practical, while always avoiding misaligned
accesses.

Either tooling or an explicit opt-in "debug mode" in the spec should allow
execution of a module in a mode that threw exceptions on misaligned access.
(This mode would incur some runtime cost for branching on most platforms which
is why it isn't the specified default.)

### Out of bounds

The ideal semantics is for out-of-bounds accesses to trap, but the implications
are not yet fully clear.

There are several possible variations on this design being discussed and
experimented with. More measurement is required to understand the associated
tradeoffs.

  * After an out-of-bounds access, the module can no longer execute code and any
    outstanding JS ArrayBuffers aliasing the linear memory are detached.
    * This would primarily allow hoisting bounds checks above effectful
      operations.
    * This can be viewed as a mild security measure under the assumption that
      while the sandbox is still ensuring safety, the module's internal state
      is incoherent and further execution could lead to Bad Things (e.g., XSS
      attacks).
  * To allow for potentially more-efficient memory sandboxing, the semantics could
    allow for a nondeterministic choice between one of the following when an
    out-of-bounds access occurred.
    * The ideal trap semantics.
    * Loads return an unspecified value.
    * Stores are either ignored or store to an unspecified location in the linear memory.
    * Either tooling or an explicit opt-in "debug mode" in the spec should allow
      execution of a module in a mode that threw exceptions on out-of-bounds
      access.

### Resizing

Linear memory can be resized by a `resize_memory` builtin operation. The
`resize_memory` operation requires its operand to be a multiple of the system
page size. To determine page size, a nullary `page_size` operation is provided.

 * `resize_memory` : grow or shrink linear memory by a given delta which
    must be a multiple of `page_size`
 * `page_size` : nullary constant function returning page size

Also as stated [above](AstSemantics.md#linear-memory), linear memory is
contiguous, meaning there are no "holes" in the linear address space. After the
MVP, there are [future features](FutureFeatures.md#finer-grained-control-over-memory)
proposed to allow setting protection and creating mappings within the
contiguous linear memory.

## Local variables

Each function has a fixed, pre-declared number of local variables which occupy a single
index space local to the function. Parameters are addressed as local variables. Local
variables do not have addresses and are not aliased in the globals or memory. Local
variables have local types and are initialized to the appropriate zero value for their
type at the beginning of the function, except parameters which are initialized to the values
of the arguments passed to the function.

  * `get_local`: read the current value of a local variable
  * `set_local`: set the current value of a local variable

The details of index space for local variables and their types will be further clarified,
e.g. whether locals with type `int32` and `int64` must be contiguous and separate from
others, etc.

## Global variables

Global variables are storage locations outside the linear memory.
Every global has exactly one memory type.
Accesses to global variables specify the index as an integer literal.

  * `load_global`: load the value of a given global variable
  * `store_global`: store a given value to a given global variable

The specification will add atomicity annotations in the future. Currently
all global accesses can be considered "non-atomic".

## Control flow structures

WebAssembly offers basic structured control flow. All control flow structures
are statements.

  * `block`: a fixed-length sequence of statements
  * `if`: if statement
  * `do_while`: do while statement, basically a loop with a conditional branch
    (back to the top of the loop)
  * `forever`: infinite loop statement (like `while (1)`), basically an
    unconditional branch (back to the top of the loop)
  * `continue`: continue to start of nested loop
  * `break`: break to end from nested loop or block
  * `return`: return zero or more values from this function
  * `switch`: switch statement with fallthrough

Loops (`do_while` and `forever`) may only be entered via fallthrough at the top.
In particular, loops may not be entered directly via a `break`, `continue`, or
`switch` destination. Break and continue statements can only target blocks or
loops in which they are nested. These rules guarantee that all control flow
graphs are well-structured.

Structured control flow provides simple and size-efficient binary encoding and
compilation. Any control flow—even irreducible—can be transformed into structured
control flow with the
[Relooper](https://github.com/kripken/emscripten/raw/master/docs/paper.pdf)
[algorithm](http://dl.acm.org/citation.cfm?id=2048224&CFID=670868333&CFTOKEN=46181900),
with guaranteed low code size overhead, and typically minimal throughput
overhead (except for pathological cases of irreducible control
flow). Alternative approaches can generate reducible control flow via node
splitting, which can reduce throughput overhead, at the cost of increasing
code size (potentially very significantly in pathological cases).
Also,
[more expressive control flow constructs](FutureFeatures.md#more-expressive-control-flow)
may be added in the future.

## Calls

Each function has a *signature*, which consists of:

  * Return types, which are a sequence of local types
  * Argument types, which are a sequence of local types

Note that WebAssembly itself does not support variable-length argument lists
(aka varargs). C and C++ compilers are expected to implement this functionality
by storing arguments in a buffer in linear memory and passing a pointer to the
buffer.

In the MVP, the length of the return types vector may only be 0 or 1. This
restriction may be lifted in the future with the addition of support for
[multiple return values](FutureFeatures.md#multiple-return-values).

There are two forms of calls:

  * `call_direct`: call function directly
  * `call_indirect`: call function indirectly

Direct calls identify their function statically. Indirect calls have a
`funcid` operand which identifies the function at runtime.

Calls have a signature, which is the expected return types and argument types
(ignoring the `funcid` operand, in the case of `call_indirect`) of the
AST node. Call operations trap if the signature of the call differs from the
signature of the called function.

### Function pointers

Function pointer values are obtained through the use of a special operator:

  * `addressof`: obtain a `funcid` value for a given statically-identified function

and are comparable for equality:

  * `funcid.eq`: function identifier compare equal

Note that it is not possible to directly observe the bits of a `funcid`
value. They may be [converted into integers][], but the integers only hold an
index into the *function table*, a table with an entry for each function
appended to the table in the order that they are loaded into the program.

In the MVP, `funcid` values are local to a single module. The
[dynamic linking](FutureFeatures.md#dynamic-linking) feature is necessary for
two modules to pass `funcid` values back and forth.

  [converted into integers]: AstSemantics.md#datatype-conversions-truncations-reinterpretations-promotions-and-demotions

## Literals

Each local type allows literal values directly in the AST. See the
[binary encoding section](BinaryEncoding.md#constant-pool).

## Expressions with control flow

Expression trees offer significant size reduction by avoiding the need for
`set_local`/`get_local` pairs in the common case of an expression with only one,
immediate use. The following primitives provide AST nodes that express control
flow and thus allow more opportunities to build bigger expression trees and
further reduce `set_local`/`get_local` usage (which constitute 30-40% of total
bytes in the
[polyfill prototype](https://github.com/WebAssembly/polyfill-prototype-1)).
Additionally, these primitives are useful building blocks for
WebAssembly-generators (including the JavaScript polyfill prototype).

  * `comma`: evaluate and ignore the result of the first operand, evaluate and
    return the second operand
  * `conditional`: basically ternary `?:` operator

New operations may be considered which allow measurably greater
expression-tree-building opportunities.

## 32-bit Integer operations

Integer operations are signed, unsigned, or sign-agnostic. Signed operations
use two's complement signed integer representation.

Signed and unsigned operations trap whenever the result cannot be represented
in the result type. This includes division and remainder by zero, and signed
division overflow (`INT32_MIN / -1`). Signed remainder with a non-zero
denominator always returns the correct value, even when the corresponding
division would trap. Sign-agnostic operations silently wrap overflowing
results into the result type.

  * `int32.add`: sign-agnostic addition
  * `int32.sub`: sign-agnostic subtraction
  * `int32.mul`: sign-agnostic multiplication (lower 32-bits)
  * `int32.sdiv`: signed division (result is truncated toward zero)
  * `int32.udiv`: unsigned division
  * `int32.srem`: signed remainder (result has the sign of the dividend)
  * `int32.urem`: unsigned remainder
  * `int32.and`: sign-agnostic logical and
  * `int32.ior`: sign-agnostic inclusive or
  * `int32.xor`: sign-agnostic exclusive or
  * `int32.shl`: sign-agnostic shift left
  * `int32.shr`: sign-agnostic logical shift right
  * `int32.sar`: sign-agnostic arithmetic shift right
  * `int32.eq`: sign-agnostic compare equal
  * `int32.ne`: sign-agnostic compare unequal
  * `int32.slt`: signed less than
  * `int32.sle`: signed less than or equal
  * `int32.ult`: unsigned less than
  * `int32.ule`: unsigned less than or equal
  * `int32.sgt`: signed greater than
  * `int32.sge`: signed greater than or equal
  * `int32.ugt`: unsigned greater than
  * `int32.uge`: unsigned greater than or equal
  * `int32.clz`: sign-agnostic count leading zero bits (defined for all values, including zero)
  * `int32.ctz`: sign-agnostic count trailing zero bits (defined for all values, including zero)
  * `int32.popcnt`: sign-agnostic count number of one bits

Shifts interpret their shift count operand as an unsigned value. When the shift
count is at least the bitwidth of the shift, `shl` and `shr` produce `0`, and
`sar` produces `0` if the value being shifted is non-negative, and `-1`
otherwise.

All comparison operations yield 32-bit integer results with `1` representing
`true` and `0` representing `false`.

## 64-bit integer operations

The same operations are available on 64-bit integers as the those available for
32-bit integers.

## Floating point operations

Floating point arithmetic follows the IEEE-754 standard, except that:
 - The sign bit and significand bit pattern of any NaN value returned from a
   floating point arithmetic operation other than `neg`, `abs`, and `copysign`
   are not specified. In particular, the "NaN propagation"
   section of IEEE-754 is not required. NaNs do propagate through arithmetic
   operations according to IEEE-754 rules, the difference here is that they do
   so without necessarily preserving the specific bit patterns of the original
   NaNs.
 - WebAssembly uses "non-stop" mode, and floating point exceptions are not
   otherwise observable. In particular, neither alternate floating point
   exception handling attributes nor the non-computational operations on status
   flags are supported. There is no observable difference between quiet and
   signalling NaN. However, positive infinity, negative infinity, and NaN are
   still always produced as result values to indicate overflow, invalid, and
   divide-by-zero conditions, as specified by IEEE-754.
 - WebAssembly uses the round-to-nearest ties-to-even rounding attribute, except
   where otherwise specified. Non-default directed rounding attributes are not
   supported.
 - The strategy for gradual underflow (subnormals) is
   [under discussion](https://github.com/WebAssembly/design/issues/148).

In the future, these limitations may be lifted, enabling
[full IEEE-754 support](FutureFeatures.md#full-ieee-754-conformance).

Note that not all operations required by IEEE-754 are provided directly.
However, WebAssembly includes enough functionality to support reasonable library
implementations of the remaining required operations.

  * `float32.add`: addition
  * `float32.sub`: subtraction
  * `float32.mul`: multiplication
  * `float32.div`: division
  * `float32.abs`: absolute value
  * `float32.neg`: negation
  * `float32.copysign`: copysign
  * `float32.ceil`: ceiling operation
  * `float32.floor`: floor operation
  * `float32.trunc`: round to nearest integer towards zero
  * `float32.nearestint`: round to nearest integer, ties to even
  * `float32.eq`: compare ordered and equal
  * `float32.ne`: compare unordered or unequal
  * `float32.lt`: compare ordered and less than
  * `float32.le`: compare ordered and less than or equal
  * `float32.gt`: compare ordered and greater than
  * `float32.ge`: compare ordered and greater than or equal
  * `float32.sqrt`: square root
  * `float32.min`: minimum (binary operator); if either operand is NaN, returns NaN
  * `float32.max`: maximum (binary operator); if either operand is NaN, returns NaN

  * `float64.add`: addition
  * `float64.sub`: subtraction
  * `float64.mul`: multiplication
  * `float64.div`: division
  * `float64.abs`: absolute value
  * `float64.neg`: negation
  * `float64.copysign`: copysign
  * `float64.ceil`: ceiling operation
  * `float64.floor`: floor operation
  * `float64.trunc`: round to nearest integer towards zero
  * `float64.nearestint`: round to nearest integer, ties to even
  * `float64.eq`: compare ordered and equal
  * `float64.ne`: compare unordered or unequal
  * `float64.lt`: compare ordered and less than
  * `float64.le`: compare ordered and less than or equal
  * `float64.gt`: compare ordered and greater than
  * `float64.ge`: compare ordered and greater than or equal
  * `float64.sqrt`: square root
  * `float64.min`: minimum (binary operator); if either operand is NaN, returns NaN
  * `float64.max`: maximum (binary operator); if either operand is NaN, returns NaN

`min` and `max` operations treat `-0.0` as being effectively less than `0.0`.

In floating point comparisons, the operands are *unordered* if either operand
is NaN, and *ordered* otherwise.

## Datatype conversions, truncations, reinterpretations, promotions, and demotions

  * `int32.wrap[int64]`: wrap a 64-bit integer to a 32-bit integer
  * `int32.trunc_signed[float32]`: truncate a 32-bit float to a signed 32-bit integer
  * `int32.trunc_signed[float64]`: truncate a 64-bit float to a signed 32-bit integer
  * `int32.trunc_unsigned[float32]`: truncate a 32-bit float to an unsigned 32-bit integer
  * `int32.trunc_unsigned[float64]`: truncate a 64-bit float to an unsigned 32-bit integer
  * `int32.reinterpret[float32]`: reinterpret the bits of a 32-bit float as a 32-bit integer
  * `int64.extend_signed[int32]`: extend a signed 32-bit integer to a 64-bit integer
  * `int64.extend_unsigned[int32]`: extend an unsigned 32-bit integer to a 64-bit integer
  * `int64.trunc_signed[float32]`: truncate a 32-bit float to a signed 64-bit integer
  * `int64.trunc_signed[float64]`: truncate a 64-bit float to a signed 64-bit integer
  * `int64.trunc_unsigned[float32]`: truncate a 32-bit float to an unsigned 64-bit integer
  * `int64.trunc_unsigned[float64]`: truncate a 64-bit float to an unsigned 64-bit integer
  * `int64.reinterpret[float64]`: reinterpret the bits of a 64-bit float as a 64-bit integer
  * `float32.demote[float64]`: demote a 64-bit float to a 32-bit float
  * `float32.cvt_signed[int32]`: convert a signed 32-bit integer to a 32-bit float
  * `float32.cvt_signed[int64]`: convert a signed 64-bit integer to a 32-bit float
  * `float32.cvt_unsigned[int32]`: convert an unsigned 32-bit integer to a 32-bit float
  * `float32.cvt_unsigned[int64]`: convert an unsigned 64-bit integer to a 32-bit float
  * `float32.reinterpret[int32]`: reinterpret the bits of a 32-bit integer as a 32-bit float
  * `float64.promote[float32]`: promote a 32-bit float to a 64-bit float
  * `float64.cvt_signed[int32]`: convert a signed 32-bit integer to a 64-bit float
  * `float64.cvt_signed[int64]`: convert a signed 64-bit integer to a 64-bit float
  * `float64.cvt_unsigned[int32]`: convert an unsigned 32-bit integer to a 64-bit float
  * `float64.cvt_unsigned[int64]`: convert an unsigned 64-bit integer to a 64-bit float
  * `float64.reinterpret[int64]`: reinterpret the bits of a 64-bit integer as a 64-bit float
  * `funcid.decode[int32]` : convert an unsigned 32-bit integer to a function identifier
  * `int32.encode` : convert a function identifier to an unsigned 32-bit integer

Wrapping and extension of integer values always succeed.
Promotion and demotion of floating point values always succeed.
Demotion of floating point values uses round-to-nearest ties-to-even rounding,
and may overflow to infinity or negative infinity as specified by IEEE-754.
If the operand of promotion or demotion is NaN, the sign bit and significand
of the result are not specified.

Reinterpretations always succeed.

Conversions from integer to floating point always succeed, and use
round-to-nearest ties-to-even rounding.

Truncation from floating point to integer where IEEE-754 would specify an
invalid operation exception (e.g. when the floating point value is NaN or
outside the range which rounds to an integer in range) traps.

Encoding a `funcid` returns the index into the function table. If the index of
the function is too great to fit in the result type, encoding traps. Decoding
returns the `funcid` from an encoded function index. If the index is out of
bounds in the function table, decoding traps. In the MVP, `funcid` values may
only be converted to and from 32-bit integers. Support for 64-bit funcid may be
added in the future.
