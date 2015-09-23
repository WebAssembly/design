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
trapping means that execution in the WebAssembly instance is terminated and
abnormal termination is reported to the outside environment. In a JavaScript
environment such as a browser, a trap results in throwing a JavaScript exception.
If developer tools are active, attaching a debugger before the
termination would be sensible.

Callstack space is limited by unspecified and dynamically varying constraints
and is a source of [nondeterminism](Nondeterminism.md). If program callstack usage
exceeds the available callstack space at any time, a trap occurs.

## Types

### Local Types

The following types are called the *local types*:

  * `i32`: 32-bit integer
  * `i64`: 64-bit integer
  * `f32`: 32-bit floating point
  * `f64`: 64-bit floating point

Note that the local types `i32` and `i64` are not inherently signed or
unsigned. The interpretation of these types is determined by individual
operations.

Parameters and local variables use local types.

Also note that there is no need for a `void` type; function signatures use
[sequences of types](Calls.md) to describe their return values, so a `void`
return type is represented as an empty sequence.

### Memory Types

*Memory types* are a superset of the local types, adding the following:

  * `i8`: 8-bit integer
  * `i16`: 16-bit integer

Global variables and linear memory accesses use memory types.

## Linear Memory

The main storage of a WebAssembly instance, called the *linear memory*, is a
contiguous, byte-addressable range of memory spanning from offset `0` and
extending for `memory_size` bytes which can be dynamically adjusted by
[`resize_memory`](AstSemantics.md#resizing). The linear memory can be considered to
be an untyped array of bytes, and it is unspecified how embedders map this array
into their process' own [virtual memory][]. The linear memory is sandboxed; it
does not alias the execution engine's internal data structures, the execution
stack, local variables, or other process memory. The initial state of linear
memory is specified by the [module](Modules.md#initial-state-of-linear-memory).

  [virtual memory]: https://en.wikipedia.org/wiki/Virtual_memory

In the MVP, linear memory is not shared between threads of execution. Separate
instances can execute in separate threads but have their own linear memory and can
only communicate through messaging, e.g. in browsers using `postMessage`. It
will be possible to share linear memory between threads of execution when
[threads](PostMVP.md#threads) are added.

### Linear Memory Accesses

Linear memory access is accomplished with explicit `load` and `store` operations.
Integer loads can specify a size which is smaller than the result type as well as a 
signedness. Such integer loads perform an implicit sign- or zero-extension as specified below.

  * `i32.load8_s`: load 1 byte and sign-extend i8 to i32
  * `i32.load8_u`: load 1 byte and zero-extend i8 to i32
  * `i32.load16_s`: load 2 bytes and sign-extend i16 to i32
  * `i32.load16_u`: load 2 bytes and zero-extend i16 to i32
  * `i32.load`: load 4 bytes as i32
  * `i64.load8_s`: load 1 byte and sign-extend i8 to i64
  * `i64.load8_u`: load 1 byte and zero-extend i8 to i64
  * `i64.load16_s`: load 2 bytes and sign-extend i16 to i64
  * `i64.load16_u`: load 2 bytes and zero-extend i16 to i64
  * `i64.load32_s`: load 4 bytes and sign-extend i32 to i64
  * `i64.load32_u`: load 4 bytes and zero-extend i32 to i64
  * `i64.load`: load 8 bytes as i64
  * `f32.load`: load 4 bytes as f32
  * `f64.load`: load 8 bytes as f64

Stores have an additional input operand which is the `value` to store to memory.
Like loads, integer stores may specify a smaller size than the
operand size and include an implicit integer wrap operation which discards the upper bits.
Stores write only the bytes in the linear memory corresponding to their size.

  * `i32.store8`: wrap i32 to i8 and store 1 byte
  * `i32.store16`: wrap i32 to i16 and store 2 bytes
  * `i32.store`: (no conversion) store 4 bytes
  * `i64.store8`: wrap i64 to i8 and store 1 byte
  * `i64.store16`: wrap i64 to i16 and store 2 bytes
  * `i64.store32`: wrap i64 to i32 and store 4 bytes
  * `i64.store`: (no conversion) store 8 bytes
  * `f32.store`: (no conversion) store 4 bytes
  * `f64.store`: (no conversion) store 8 bytes

In addition to storing to memory, store instructions produce a value which is their 
`value` input operand without conversion.

### Addressing

Each linear memory access operation also has an address operand and an immediate
integer byte offset attribute. The immediate is the same type as the address'
index. The infinite-precision sum of the address operand's value with the byte
offset attribute's value is called the *effective address*, which is interpreted
as an unsigned byte index.

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

In wasm32, address operands and offset attributes have type `i32`, and linear
memory sizes are limited to 4 GiB (of course, actual sizes are further limited
by [available resources](Nondeterminism.md)). In wasm64, address operands and
offsets have type `i64`. The MVP only includes wasm32; subsequent versions
will add support for wasm64 and thus
[>4 GiB linear memory](FutureFeatures.md#linear-memory-bigger-than-4-gib).

### Alignment

Each linear memory access operation also has an immediate positive integer power
of 2 alignment attribute, which is the same type as the address' index. An
alignment value which is the same as the memory attribute size is considered
to be a *natural* alignment.

The linear memory operation names listed above specify natural alignment.
To specify unnatural alignment, the opcode name can be suffixed with `/n` for
`n` any integer power of 2. For example, `f64.load/2` specifies a `f32`
load with 2-byte alignment; `i32.load16_s/1` specifies a signed 2-byte load
that is unaligned (1-byte aligned).

The alignment applies to the effective address and not merely the address operand,
i.e. the immediate offset is taken into account when considering alignment.

If the effective address of a memory access is a multiple of the alignment
attribute value of the memory access, the memory access is considered *aligned*,
otherwise it is considered *misaligned*. Aligned and misaligned accesses have
the same behavior. Alignment affects performance as follows:

 * Aligned accesses with at least natural alignment are fast.
 * Aligned accesses with less than natural alignment may be somewhat slower
   (think: implementation makes multiple accesses, either in software or
    in hardware).
 * Misaligned access of any kind may be *massively* slower
   (think: implementation takes a signal and fixes things up).

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

  * After an out-of-bounds access, the instance can no longer execute code and any
    outstanding JavaScript [ArrayBuffer][] aliasing the linear memory are detached.
    * This would primarily allow hoisting bounds checks above effectful
      operations.
    * This can be viewed as a mild security measure under the assumption that
      while the sandbox is still ensuring safety, the instance's internal state
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

  [ArrayBuffer]: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/ArrayBuffer

### Resizing

Linear memory can be resized by a `resize_memory` builtin operation. The
`resize_memory` operation requires its operand to be a multiple of the system
page size. To determine page size, a nullary `page_size` operation is provided.

 * `resize_memory` : grow or shrink linear memory by a given delta which
    must be a multiple of `page_size`
 * `page_size` : nullary constant function returning page size in bytes

Also as stated [above](AstSemantics.md#linear-memory), linear memory is
contiguous, meaning there are no "holes" in the linear address space. After the
MVP, there are [future features](FutureFeatures.md#finer-grained-control-over-memory)
proposed to allow setting protection and creating mappings within the
contiguous linear memory.

The result type of `page_size` is `int32` for wasm32 and `int64` for wasm64.
The result value of `page_size` is an unsigned integer which is a power of 2.

(Note that the `page_size` value need not reflect the actual internal page size
of the implementation; it just needs to be a value suitable for use with
`resize_memory`)

## Local variables

Each function has a fixed, pre-declared number of local variables which occupy a single
index space local to the function. Parameters are addressed as local variables. Local
variables do not have addresses and are not aliased by linear memory. Local
variables have local types and are initialized to the appropriate zero value for their
type at the beginning of the function, except parameters which are initialized to the values
of the arguments passed to the function.

  * `get_local`: read the current value of a local variable
  * `set_local`: set the current value of a local variable

The details of index space for local variables and their types will be further clarified,
e.g. whether locals with type `i32` and `i64` must be contiguous and separate from
others, etc.

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

In the MVP, the length of the return types sequence may only be 0 or 1. This
restriction may be lifted in the future.

Direct calls to a function specify the callee by index into a function table.

  * `call`: call function directly

Calls must match the function signature exactly.

Like direct calls, calls to [imports](Modules.md#imports-and-exports) specify
the callee by index into an import table (defined by the sequence of import
declarations in the module import section) and the call must match the declared
signature of the import exactly.

  * `call_import` : call imported function directly

Indirect calls may be made to a value of function-pointer type. A 
function-pointer value may be obtained for a given function as specified by its index
in the function table.

  * `call_indirect`: call function indirectly
  * `addressof`: obtain a function pointer value for a given function

Function-pointer values are comparable for equality and the `addressof` operator
is monomorphic. Function-pointer values can be explicitly coerced to and from
integers (which, in particular, is necessary when loading/storing to memory
since memory only provides integer types). For security and safety reasons,
the integer value of a coerced function-pointer value is an abstract index and
does not reveal the actual machine code address of the target function.

In the MVP, function pointer values are local to a single module. The
[dynamic linking](DynamicLinking.md) feature is necessary for
two modules to pass function pointers back and forth.

Multiple return value calls will be possible, though possibly not in the
MVP. The details of multiple-return-value calls needs clarification. Calling a
function that returns multiple values will likely have to be a statement that
specifies multiple local variables to which to assign the corresponding return
values.

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

  * `i32.add`: sign-agnostic addition
  * `i32.sub`: sign-agnostic subtraction
  * `i32.mul`: sign-agnostic multiplication (lower 32-bits)
  * `i32.div_s`: signed division (result is truncated toward zero)
  * `i32.div_u`: unsigned division (result is [floored](https://en.wikipedia.org/wiki/Floor_and_ceiling_functions))
  * `i32.rem_s`: signed remainder (result has the sign of the dividend)
  * `i32.rem_u`: unsigned remainder
  * `i32.and`: sign-agnostic logical and
  * `i32.or`: sign-agnostic inclusive or
  * `i32.xor`: sign-agnostic exclusive or
  * `i32.shl`: sign-agnostic shift left
  * `i32.shr_u`: zero-replicating (logical) shift right
  * `i32.shr_s`: sign-replicating (arithmetic) shift right
  * `i32.eq`: sign-agnostic compare equal
  * `i32.ne`: sign-agnostic compare unequal
  * `i32.lt_s`: signed less than
  * `i32.le_s`: signed less than or equal
  * `i32.lt_u`: unsigned less than
  * `i32.le_u`: unsigned less than or equal
  * `i32.gt_s`: signed greater than
  * `i32.ge_s`: signed greater than or equal
  * `i32.gt_u`: unsigned greater than
  * `i32.ge_u`: unsigned greater than or equal
  * `i32.clz`: sign-agnostic count leading zero bits (defined for all values, including zero)
  * `i32.ctz`: sign-agnostic count trailing zero bits (defined for all values, including zero)
  * `i32.popcnt`: sign-agnostic count number of one bits

Shifts interpret their shift count operand as an unsigned value. When the shift
count is at least the bitwidth of the shift, `shl` and `shr_u` produce `0`, and
`shr_s` produces `0` if the value being shifted is non-negative, and `-1`
otherwise.

All comparison operations yield 32-bit integer results with `1` representing
`true` and `0` representing `false`.

## 64-bit integer operations

The same operations are available on 64-bit integers as the those available for
32-bit integers.

## Floating point operations

Floating point arithmetic follows the IEEE 754-2008 standard, except that:
 - The sign bit and significand bit pattern of any NaN value returned from a
   floating point arithmetic operation other than `neg`, `abs`, and `copysign`
   are not specified. In particular, the "NaN propagation"
   section of IEEE 754-2008 is not required. NaNs do propagate through
   arithmetic operations according to IEEE 754-2008 rules, the difference here
   is that they do so without necessarily preserving the specific bit patterns
   of the original NaNs.
 - WebAssembly uses "non-stop" mode, and floating point exceptions are not
   otherwise observable. In particular, neither alternate floating point
   exception handling attributes nor the non-computational operations on status
   flags are supported. There is no observable difference between quiet and
   signalling NaN. However, positive infinity, negative infinity, and NaN are
   still always produced as result values to indicate overflow, invalid, and
   divide-by-zero conditions, as specified by IEEE 754-2008.
 - WebAssembly uses the round-to-nearest ties-to-even rounding attribute, except
   where otherwise specified. Non-default directed rounding attributes are not
   supported.
 - The strategy for gradual underflow (subnormals) is
   [under discussion](https://github.com/WebAssembly/design/issues/148).

In the future, these limitations may be lifted, enabling
[full IEEE 754-2008 support](FutureFeatures.md#full-ieee-754-2008-conformance).

Note that not all operations required by IEEE 754-2008 are provided directly.
However, WebAssembly includes enough functionality to support reasonable library
implementations of the remaining required operations.

  * `f32.add`: addition
  * `f32.sub`: subtraction
  * `f32.mul`: multiplication
  * `f32.div`: division
  * `f32.abs`: absolute value
  * `f32.neg`: negation
  * `f32.copysign`: copysign
  * `f32.ceil`: ceiling operation
  * `f32.floor`: floor operation
  * `f32.trunc`: round to nearest integer towards zero
  * `f32.nearest`: round to nearest integer, ties to even
  * `f32.eq`: compare ordered and equal
  * `f32.ne`: compare unordered or unequal
  * `f32.lt`: compare ordered and less than
  * `f32.le`: compare ordered and less than or equal
  * `f32.gt`: compare ordered and greater than
  * `f32.ge`: compare ordered and greater than or equal
  * `f32.sqrt`: square root
  * `f32.min`: minimum (binary operator); if either operand is NaN, returns NaN
  * `f32.max`: maximum (binary operator); if either operand is NaN, returns NaN

  * `f64.add`: addition
  * `f64.sub`: subtraction
  * `f64.mul`: multiplication
  * `f64.div`: division
  * `f64.abs`: absolute value
  * `f64.neg`: negation
  * `f64.copysign`: copysign
  * `f64.ceil`: ceiling operation
  * `f64.floor`: floor operation
  * `f64.trunc`: round to nearest integer towards zero
  * `f64.nearest`: round to nearest integer, ties to even
  * `f64.eq`: compare ordered and equal
  * `f64.ne`: compare unordered or unequal
  * `f64.lt`: compare ordered and less than
  * `f64.le`: compare ordered and less than or equal
  * `f64.gt`: compare ordered and greater than
  * `f64.ge`: compare ordered and greater than or equal
  * `f64.sqrt`: square root
  * `f64.min`: minimum (binary operator); if either operand is NaN, returns NaN
  * `f64.max`: maximum (binary operator); if either operand is NaN, returns NaN

`min` and `max` operations treat `-0.0` as being effectively less than `0.0`.

In floating point comparisons, the operands are *unordered* if either operand
is NaN, and *ordered* otherwise.

## Datatype conversions, truncations, reinterpretations, promotions, and demotions

  * `i32.wrap[i64]`: wrap a 64-bit integer to a 32-bit integer
  * `i32.trunc_s[f32]`: truncate a 32-bit float to a signed 32-bit integer
  * `i32.trunc_s[f64]`: truncate a 64-bit float to a signed 32-bit integer
  * `i32.trunc_u[f32]`: truncate a 32-bit float to an unsigned 32-bit integer
  * `i32.trunc_u[f64]`: truncate a 64-bit float to an unsigned 32-bit integer
  * `i32.reinterpret[f32]`: reinterpret the bits of a 32-bit float as a 32-bit integer
  * `i64.extend_s[i32]`: extend a signed 32-bit integer to a 64-bit integer
  * `i64.extend_u[i32]`: extend an unsigned 32-bit integer to a 64-bit integer
  * `i64.trunc_s[f32]`: truncate a 32-bit float to a signed 64-bit integer
  * `i64.trunc_s[f64]`: truncate a 64-bit float to a signed 64-bit integer
  * `i64.trunc_u[f32]`: truncate a 32-bit float to an unsigned 64-bit integer
  * `i64.trunc_u[f64]`: truncate a 64-bit float to an unsigned 64-bit integer
  * `i64.reinterpret[f64]`: reinterpret the bits of a 64-bit float as a 64-bit integer
  * `f32.demote[f64]`: demote a 64-bit float to a 32-bit float
  * `f32.convert_s[i32]`: convert a signed 32-bit integer to a 32-bit float
  * `f32.convert_s[i64]`: convert a signed 64-bit integer to a 32-bit float
  * `f32.convert_u[i32]`: convert an unsigned 32-bit integer to a 32-bit float
  * `f32.convert_u[i64]`: convert an unsigned 64-bit integer to a 32-bit float
  * `f32.reinterpret[i32]`: reinterpret the bits of a 32-bit integer as a 32-bit float
  * `f64.promote[f32]`: promote a 32-bit float to a 64-bit float
  * `f64.convert_s[i32]`: convert a signed 32-bit integer to a 64-bit float
  * `f64.convert_s[i64]`: convert a signed 64-bit integer to a 64-bit float
  * `f64.convert_u[i32]`: convert an unsigned 32-bit integer to a 64-bit float
  * `f64.convert_u[i64]`: convert an unsigned 64-bit integer to a 64-bit float
  * `f64.reinterpret[i64]`: reinterpret the bits of a 64-bit integer as a 64-bit float

Wrapping and extension of integer values always succeed.
Promotion and demotion of floating point values always succeed.
Demotion of floating point values uses round-to-nearest ties-to-even rounding,
and may overflow to infinity or negative infinity as specified by IEEE 754-2008.
If the operand of promotion or demotion is NaN, the sign bit and significand
of the result are not specified.

Reinterpretations always succeed.

Conversions from integer to floating point always succeed, and use
round-to-nearest ties-to-even rounding.

Truncation from floating point to integer where IEEE 754-2008 would specify an
invalid operation exception (e.g. when the floating point value is NaN or
outside the range which rounds to an integer in range) traps.

## Feature test

To support [feature testing](FeatureTest.md), an AST node would be provided:

  * `has_feature`: return whether the given feature is supported, identified by string

In the MVP, `has_feature` would always return false. As features were added post-MVP,
`has_feature` would start returning true. `has_feature` is a pure function, always
returning the same value for the same string over the lifetime of a single
instance and other related (as defined by the host environment) instances.
