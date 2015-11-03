# Abstract Syntax Tree Semantics

WebAssembly code is represented as an Abstract Syntax Tree (AST) that has a
basic division between statements and expressions. Each function body consists
of exactly one statement. All expressions and operators are typed, with no
implicit conversions or overloading rules.

Verification of WebAssembly code requires only a single pass with constant-time
type checking and well-formedness checking.

WebAssembly offers a set of operators that are language-independent but closely
match operators in many programming languages and are efficiently implementable
on all modern computers.

The [rationale](Rationale.md) document details why WebAssembly is designed as
detailed in this document.

## Order of evaluation

The evaluation order of child nodes is deterministic.

All nodes other than control flow constructs need to evaluate their child nodes
in the order they appear in the serialized AST.

For example, the s-expression presentation of the `i32.add` node
`(i32.add (set_local $x (i32.const 1)) (set_local $x (i32.const 2)))`
would first evaluate the child node  `(set_local $x (i32.const 1))` and
afterwards the child node `(set_local $x (i32.const 2))`.

The value of the local variable $x will be `2` after the `i32.add` node is fully
evaluated.

## Traps

Some operators may *trap* under some conditions, as noted below. In the MVP,
trapping means that execution in the WebAssembly instance is terminated and
abnormal termination is reported to the outside environment. In a JavaScript
environment such as a browser, a trap results in throwing a JavaScript exception.
If developer tools are active, attaching a debugger before the
termination would be sensible.

Callstack space is limited by unspecified and dynamically varying constraints
and is a source of [nondeterminism](Nondeterminism.md). If program callstack usage
exceeds the available callstack space at any time, a trap occurs.

Implementations must have an internal maximum call stack size, and every call
must take up some resources toward exhausting that size (of course, dynamic
resources may be exhausted much earlier). This rule exists to avoid differences
in observable behavior; if some implementations have this property and others
don't, the same program which runs successfully on some implementations may
consume unbounded resources and fail on others. Also, in the future, it is
expected that WebAssembly will add some form of stack-introspection
functionality, in which case such optimizations would be directly observable.

Support for explicit tail calls is planned in
[the future](FutureFeatures.md#general-purpose-proper-tail-calls),
which would add an explicit tail-call operator with well-defined effects
on stack introspection.

## Types

WebAssembly has the following *value types*:

  * `i32`: 32-bit integer
  * `i64`: 64-bit integer
  * `f32`: 32-bit floating point
  * `f64`: 64-bit floating point

Note that the value types `i32` and `i64` are not inherently signed or
unsigned. The interpretation of these types is determined by individual
operators.

Parameters and local variables have value types.

Also note that there is no need for a `void` type; function signatures use
[sequences of types](AstSemantics.md#calls) to describe their return values, so
a `void` return type is represented as an empty sequence.

## Linear Memory

The main storage of a WebAssembly instance, called the *linear memory*, is a
contiguous, byte-addressable range of memory spanning from offset `0` and
extending for `memory_size` bytes which can be dynamically grown by
[`grow_memory`](AstSemantics.md#resizing). The linear memory can be considered to
be an untyped array of bytes, and it is unspecified how embedders map this array
into their process' own [virtual memory][]. The linear memory is sandboxed; it
does not alias the execution engine's internal data structures, the execution
stack, local variables, or other process memory. The initial state of linear
memory is specified by the [module](Modules.md#linear-memory-section).

  [virtual memory]: https://en.wikipedia.org/wiki/Virtual_memory

In the MVP, linear memory is not shared between threads of execution. Separate
instances can execute in separate threads but have their own linear memory and can
only communicate through messaging, e.g. in browsers using `postMessage`. It
will be possible to share linear memory between threads of execution when
[threads](PostMVP.md#threads) are added.

### Linear Memory Accesses

Linear memory access is accomplished with explicit `load` and `store` operators.
Integer loads can specify a *storage size* which is smaller than the result type as
well as a signedness which determines whether the bytes are sign- or zero-
extended into the result type.

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
Like loads, integer stores may specify a smaller *storage size* than the operand
size in which case integer wrapping is implied.

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
`value` input operand before wrapping.

### Addressing

Each linear memory access operator has an address operand and an unsigned 
integer byte offset immediate. The immediate is the same type as the address'
index. The infinite-precision unsigned sum of the address operand's value with 
the immediate offset's value is called the *effective address*, which is 
interpreted as an unsigned byte index.

Linear memory operators access the bytes starting at the effective address and
extend for the number of bytes implied by the storage size. If any of the
accessed bytes are beyond `memory_size`, the access is considered
*out-of-bounds*.

The use of infinite-precision in the effective address computation means that
the addition of the offset to the address never causes wrapping, so if the
address for an access is out-of-bounds, the effective address will always also
be out-of-bounds.

In wasm32, address operands and offset attributes have type `i32`, and linear
memory sizes are limited to 4 GiB (of course, actual sizes are further limited
by [available resources](Nondeterminism.md)). In wasm64, address operands and
offsets have type `i64`. The MVP only includes wasm32; subsequent versions
will add support for wasm64 and thus
[>4 GiB linear memory](FutureFeatures.md#linear-memory-bigger-than-4-gib).

### Alignment

Each linear memory access operator also has an immediate positive integer power
of 2 alignment attribute, which is the same type as the address' index. An
alignment value which is the same as the memory attribute size is considered
to be a *natural* alignment. The alignment applies to the effective address and
not merely the address operand, i.e. the immediate offset is taken into account
when considering alignment.

If the effective address of a memory access is a multiple of the alignment
attribute value of the memory access, the memory access is considered *aligned*,
otherwise it is considered *misaligned*. Aligned and misaligned accesses have
the same behavior. 

Alignment affects performance as follows:

 * Aligned accesses with at least natural alignment are fast.
 * Aligned accesses with less than natural alignment may be somewhat slower
   (think: implementation makes multiple accesses, either in software or in
   hardware).
 * Misaligned access of any kind may be *massively* slower (think:
   implementation takes a signal and fixes things up).

Thus, it is recommend that WebAssembly producers align frequently-used data to
permit the use of natural alignment access, and use loads and stores with the
greatest alignment values practical, while always avoiding misaligned accesses.

### Out of Bounds

Out of bounds accesses trap.

### Resizing

In the MVP, linear memory can be resized by a `grow_memory` operator. This
operator requires its operand to be a multiple of the WebAssembly page size,
which is 64KiB on all engines (though large page support may be added in 
the [future](FutureFeatures.md#large-page-support).

 * `grow_memory` : grow linear memory by a given unsigned delta which
    must be a multiple of the page size.

As stated [above](AstSemantics.md#linear-memory), linear memory is contiguous,
meaning there are no "holes" in the linear address space. After the
MVP, there are [future features](FutureFeatures.md#finer-grained-control-over-memory)
proposed to allow setting protection and creating mappings within the
contiguous linear memory.

In the MVP, memory can only be grown. After the MVP, a memory shrinking
operator may be added. However, due to normal fragmentation, applications are
instead expected release unused physical pages from the working set using the
[`discard`](FutureFeatures.md#finer-grained-control-over-memory) future feature.

## Local variables

Each function has a fixed, pre-declared number of local variables which occupy a single
index space local to the function. Parameters are addressed as local variables. Local
variables do not have addresses and are not aliased by linear memory. Local
variables have value types and are initialized to the appropriate zero value for their
type at the beginning of the function, except parameters which are initialized to the values
of the arguments passed to the function.

  * `get_local`: read the current value of a local variable
  * `set_local`: set the current value of a local variable

The details of index space for local variables and their types will be further clarified,
e.g. whether locals with type `i32` and `i64` must be contiguous and separate from
others, etc.

## Control flow structures

WebAssembly offers basic structured control flow with the following constructs.
All control flow structures, except `case`, are statements.

 * `block`: a fixed-length sequence of statements with a label at the end
 * `loop`: a fixed-length sequence of statements with a label at the end
           and a loop header label at the top (note: this does not loop by
           itself, so one would often combine this with a br_if at the end
           to form a branch back to the top)
 * `if`: if statement with then body
 * `if_else`: if statement with then and else bodies
 * `br`: branch to a given label in an enclosing construct (see below)
 * `br_if`: conditionally branch to a given label in an enclosing construct
 * `tableswitch`: a jump table which may jump either to enclosed `case` blocks
                  or to labels in enclosing constructs (see below for a more
                  detailed description)
 * `case`: must be an immediate child of `tableswitch`; has a label declared
           in the `tableswitch`'s table and a body (as above, see below)
 * `return`: return zero or more values from this function

References to labels must occur within an *enclosing construct* that defined
the label. This means that references to an AST node's label can only happen
within descendents of the node in the tree. For example, references to a
`block`'s label can only occur within the `block`'s body. In practice,
one can arrange `block`s to put labels wherever one wants to jump to, except
for one restriction: one can't jump into the middle of a loop from outside
it. This restriction ensures the well-structured property discussed below.

`tableswitch` instructions have a zero-based array of labels, a "default"
label, an index operand, and a list of `case` nodes. A `tableswitch`
selects which label to branch to by looking up the index value in the label
array, and transferring control to that label. If the index is out of bounds,
it transfers control to the "default" label.

`case` nodes can only appear as immediate children of `tableswitch` statements.
They have a label, which must be declared in the immediately enclosing
`tableswitch`'s array, and a body which can contain arbitrary code. Control
falls through the end of a `case` block into the following `case` block, or
the end of the `tableswitch` in the case of the last `case`.


## Calls

Each function has a *signature*, which consists of:

  * Return types, which are a sequence of value types
  * Argument types, which are a sequence of value types

WebAssembly doesn't support variable-length argument lists (aka
varargs). Compilers targeting WebAssembly can instead support them through
explicit accesses to linear memory.

In the MVP, the length of the return types sequence may only be 0 or 1. This
restriction may be lifted in the future.

Direct calls to a function specify the callee by index into a *main function table*.

  * `call`: call function directly

A direct call to a function with a mismatched signature is a module verification error.

Like direct calls, calls to [imports](Modules.md#imports-and-exports) specify
the callee by index into an *imported function table* defined by the sequence of import
declarations in the module import section. A direct call to an imported function with a
mismatched signature is a module verification error.

  * `call_import` : call imported function directly

Indirect calls allow calling target functions that are unknown at compile time.
The target function is an expression of value type `i32` and is always the first
input into the indirect call.

A `call_indirect` specifies the *expected* signature of the target function with
an index into a *signature table* defined by the module. An indirect call to a
function with a mismatched signature causes a trap.

  * `call_indirect`: call function indirectly

Functions from the main function table are made addressable by defining an
*indirect function table* that consists of a sequence of indices into the
module's main function table. A function from the main table may appear more
than once in the indirect function table. Functions not appearing in the
indirect function table cannot be called indirectly.

In the MVP, indices into the indirect function table are local to a single
module, so wasm modules may use `i32` constants to refer to entries in their own
indirect function table. The [dynamic linking](DynamicLinking.md) feature is
necessary for two modules to pass function pointers back and forth. This will
mean concatenating indirect function tables and adding an operator `address_of`
that computes the absolute index into the concatenated table from an index in a
module's local indirect table. JITing may also mean appending more functions to
the end of the indirect function table.

Multiple return value calls will be possible, though possibly not in the
MVP. The details of multiple-return-value calls needs clarification. Calling a
function that returns multiple values will likely have to be a statement that
specifies multiple local variables to which to assign the corresponding return
values.

## Constants

These operators have an immediate operand of their associated type which is
produced as their result value. All possible values of all types are
supported (including NaN values of all possible bit patterns).

  * `i32.const`: produce the value of an i32 immediate
  * `i64.const`: produce the value of an i64 immediate
  * `f32.const`: produce the value of an f32 immediate
  * `f64.const`: produce the value of an f64 immediate

## Expressions with Control Flow

  * `comma`: evaluate and ignore the result of the first operand, evaluate and
    return the second operand
  * `conditional`: basically ternary `?:` operator

New operators may be considered which allow measurably greater
expression-tree-building opportunities.

## 32-bit Integer operators

Integer operators are signed, unsigned, or sign-agnostic. Signed operators
use two's complement signed integer representation.

Signed and unsigned operators trap whenever the result cannot be represented
in the result type. This includes division and remainder by zero, and signed
division overflow (`INT32_MIN / -1`). Signed remainder with a non-zero
denominator always returns the correct value, even when the corresponding
division would trap. Sign-agnostic operators silently wrap overflowing
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
  * `i32.clz`: sign-agnostic count leading zero bits (All zero bits are considered leading if the value is zero)
  * `i32.ctz`: sign-agnostic count trailing zero bits (All zero bits are considered trailing if the value is zero)
  * `i32.popcnt`: sign-agnostic count number of one bits

Shifts interpret their shift count operand as an unsigned value. When the shift
count is at least the bitwidth of the shift, `shl` and `shr_u` produce `0`, and
`shr_s` produces `0` if the value being shifted is non-negative, and `-1`
otherwise.

All comparison operators yield 32-bit integer results with `1` representing
`true` and `0` representing `false`.

## 64-bit integer operators

The same operators are available on 64-bit integers as the those available for
32-bit integers.

## Floating point operators

Floating point arithmetic follows the IEEE 754-2008 standard, except that:
 - The sign bit and significand bit pattern of any NaN value returned from a
   floating point arithmetic operator other than `neg`, `abs`, and `copysign`
   are not specified. In particular, the "NaN propagation"
   section of IEEE 754-2008 is not required. NaNs do propagate through
   arithmetic operators according to IEEE 754-2008 rules, the difference here
   is that they do so without necessarily preserving the specific bit patterns
   of the original NaNs.
 - WebAssembly uses "non-stop" mode, and floating point exceptions are not
   otherwise observable. In particular, neither alternate floating point
   exception handling attributes nor the non-computational operators on status
   flags are supported. There is no observable difference between quiet and
   signalling NaN. However, positive infinity, negative infinity, and NaN are
   still always produced as result values to indicate overflow, invalid, and
   divide-by-zero conditions, as specified by IEEE 754-2008.
 - WebAssembly uses the round-to-nearest ties-to-even rounding attribute, except
   where otherwise specified. Non-default directed rounding attributes are not
   supported.

In the future, these limitations may be lifted, enabling
[full IEEE 754-2008 support](FutureFeatures.md#full-ieee-754-2008-conformance).

Note that not all operators required by IEEE 754-2008 are provided directly.
However, WebAssembly includes enough functionality to support reasonable library
implementations of the remaining required operators.

  * `f32.add`: addition
  * `f32.sub`: subtraction
  * `f32.mul`: multiplication
  * `f32.div`: division
  * `f32.abs`: absolute value
  * `f32.neg`: negation
  * `f32.copysign`: copysign
  * `f32.ceil`: ceiling operator
  * `f32.floor`: floor operator
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

64-bit floating point operators:

  * `f64.add`: addition
  * `f64.sub`: subtraction
  * `f64.mul`: multiplication
  * `f64.div`: division
  * `f64.abs`: absolute value
  * `f64.neg`: negation
  * `f64.copysign`: copysign
  * `f64.ceil`: ceiling operator
  * `f64.floor`: floor operator
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

`min` and `max` operators treat `-0.0` as being effectively less than `0.0`.

In floating point comparisons, the operands are *unordered* if either operand
is NaN, and *ordered* otherwise.

## Datatype conversions, truncations, reinterpretations, promotions, and demotions

  * `i32.wrap/i64`: wrap a 64-bit integer to a 32-bit integer
  * `i32.trunc_s/f32`: truncate a 32-bit float to a signed 32-bit integer
  * `i32.trunc_s/f64`: truncate a 64-bit float to a signed 32-bit integer
  * `i32.trunc_u/f32`: truncate a 32-bit float to an unsigned 32-bit integer
  * `i32.trunc_u/f64`: truncate a 64-bit float to an unsigned 32-bit integer
  * `i32.reinterpret/f32`: reinterpret the bits of a 32-bit float as a 32-bit integer
  * `i64.extend_s/i32`: extend a signed 32-bit integer to a 64-bit integer
  * `i64.extend_u/i32`: extend an unsigned 32-bit integer to a 64-bit integer
  * `i64.trunc_s/f32`: truncate a 32-bit float to a signed 64-bit integer
  * `i64.trunc_s/f64`: truncate a 64-bit float to a signed 64-bit integer
  * `i64.trunc_u/f32`: truncate a 32-bit float to an unsigned 64-bit integer
  * `i64.trunc_u/f64`: truncate a 64-bit float to an unsigned 64-bit integer
  * `i64.reinterpret/f64`: reinterpret the bits of a 64-bit float as a 64-bit integer
  * `f32.demote/f64`: demote a 64-bit float to a 32-bit float
  * `f32.convert_s/i32`: convert a signed 32-bit integer to a 32-bit float
  * `f32.convert_s/i64`: convert a signed 64-bit integer to a 32-bit float
  * `f32.convert_u/i32`: convert an unsigned 32-bit integer to a 32-bit float
  * `f32.convert_u/i64`: convert an unsigned 64-bit integer to a 32-bit float
  * `f32.reinterpret/i32`: reinterpret the bits of a 32-bit integer as a 32-bit float
  * `f64.promote/f32`: promote a 32-bit float to a 64-bit float
  * `f64.convert_s/i32`: convert a signed 32-bit integer to a 64-bit float
  * `f64.convert_s/i64`: convert a signed 64-bit integer to a 64-bit float
  * `f64.convert_u/i32`: convert an unsigned 32-bit integer to a 64-bit float
  * `f64.convert_u/i64`: convert an unsigned 64-bit integer to a 64-bit float
  * `f64.reinterpret/i64`: reinterpret the bits of a 64-bit integer as a 64-bit float

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
invalid operator exception (e.g. when the floating point value is NaN or
outside the range which rounds to an integer in range) traps.

## Type-parameterized operators.

  * `select`: a ternary operator with a boolean (i32) condition and two
    additional operands, which must have the same type as each other. `select`
    returns the the first of these two operands if the condition operand is
    non-zero, or the second otherwise.

## Feature test

To support [feature testing](FeatureTest.md), an AST node would be provided:

  * `has_feature`: return whether the given feature is supported, identified by string

In the MVP, `has_feature` would always return false. As features were added post-MVP,
`has_feature` would start returning true. `has_feature` is a pure function, always
returning the same value for the same string over the lifetime of a single
instance and other related (as defined by the host environment) instances.
See also [feature testing](FeatureTest.md) and
[better feature testing](FutureFeatures.md#better-feature-testing-support).
