# Abstract Syntax Tree Semantics

WebAssembly code is represented as an Abstract Syntax Tree (AST) where each node
represents an expression. Each function body consists of exactly one expression. 
All expressions and operators are typed, with no implicit conversions or overloading rules.

This document explains the high-level design of the AST: its types, constructs, and
semantics. For full details consult [the formal Specification](https://github.com/WebAssembly/spec),
for file-level encoding details consult [Binary Encoding](BinaryEncoding.md),
and for the human-readable text representation consult [Text Format](TextFormat.md).

Verification of WebAssembly code requires only a single pass with constant-time
type checking and well-formedness checking.

WebAssembly offers a set of language-independent operators that closely
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

Each parameter and local variable has exactly one [value type](AstSemantics.md#types). Function signatures
consist of a sequence of zero or more parameter types and a sequence of zero or more return
types. (Note: in the MVP, a function can have at most one return type).

Note that the value types `i32` and `i64` are not inherently signed or
unsigned. The interpretation of these types is determined by individual
operators.

## Linear Memory

The main storage of a WebAssembly instance, called the *linear memory*, is a
contiguous, byte-addressable range of memory spanning from offset `0` and
extending for `memory_size` bytes which can be dynamically grown by
[`grow_memory`](AstSemantics.md#resizing). The linear memory can be considered
to be an untyped array of bytes, and it is unspecified how embedders map this
array into their process' own [virtual memory][]. The linear memory is
sandboxed; it does not alias the execution engine's internal data structures,
the execution stack, local variables, or other process memory.

The initial state of linear memory is specified by the
[module](Modules.md#linear-memory-section). The initial and maximum memory size
are required to be a multiple of the WebAssembly page size, which is 64KiB on
all engines (though large page support may be added in the
[future](FutureFeatures.md#large-page-support)).

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

In the MVP, linear memory can be resized by a `grow_memory` operator. The
operand to this operator is in units of the WebAssembly page size,
which is defined to be 64KiB (though large page support may be added in 
the [future](FutureFeatures.md#large-page-support)).

 * `grow_memory` : grow linear memory by a given unsigned delta of pages.
    Return the previous memory size in units of pages or -1 on failure.

When a maximum memory size is declared in the [memory section](Module.md#linear-memory-section),
`grow_memory` must fail if it would grow past the maximum. However,
`grow_memory` may still fail before the maximum if it was not possible to
reserve the space up front or if enabling the reserved memory fails.
When there is no maximum memory size declared, `grow_memory` is expected
to perform a system allocation which may fail.

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
Since all AST nodes are expressions in WebAssembly, control constructs may yield
a value and may appear as children of other expressions.

 * `nop`: an empty operator that does not yield a value 
 * `block`: a fixed-length sequence of expressions with a label at the end
 * `loop`: a block with an additional label at the beginning which may be used to form loops
 * `if`: if expression with a *then* expression
 * `if_else`: if expression with *then* and *else* expressions
 * `br`: branch to a given label in an enclosing construct
 * `br_if`: conditionally branch to a given label in an enclosing construct
 * `br_table`: a jump table which jumps to a label in an enclosing construct
 * `return`: return zero or more values from this function

### Branches and nesting

The `br` and `br_if` constructs express low-level branching.
Branches may only reference labels defined by an outer *enclosing construct*.
This means that, for example, references to a `block`'s label can only occur 
within the `block`'s body.

In practice, outer `block`s can be used to place labels for any given branching
pattern, except for one restriction: one can't branch into the middle of a loop
from outside it. This restriction ensures all control flow graphs are well-structured
in the exact sense as in high-level languages like Java, JavaScript, Rust and Go. To
further see the parallel, note that a `br` to a `block`'s label is functionally
equivalent to a labeled `break` in high-level languages in that a `br` simply
breaks out of a `block`.

Branches that exit a `block`, `loop`, or `br_table` may take a subexpression
that yields a value for the exited construct. If present, it is the first operand
before any others.

### Yielding values from control constructs

The `nop`, `if`, `br`, `br_if`, and `return` constructs do not yield values.
Other control constructs may yield values if their subexpressions yield values:

* `block`: yields either the value of the last expression in the block or the result of an inner `br` that targeted the label of the block
* `loop`: yields either the value of the last expression in the loop or the result of an inner `br` that targeted the end label of the loop
* `if_else`: yields either the value of the true expression or the false expression


### `br_table`

A `br_table` consists of a zero-based array of labels, a *default* label,
and an index operand. A `br_table` jumps to the label indexed in the array
or the default label if the index is out of bounds.


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
  * `i32.and`: sign-agnostic bitwise and
  * `i32.or`: sign-agnostic bitwise inclusive or
  * `i32.xor`: sign-agnostic bitwise exclusive or
  * `i32.shl`: sign-agnostic shift left
  * `i32.shr_u`: zero-replicating (logical) shift right
  * `i32.shr_s`: sign-replicating (arithmetic) shift right
  * `i32.rotl`: sign-agnostic rotate left
  * `i32.rotr`: sign-agnostic rotate right
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
  * `i32.eqz`: compare equal to zero (return 1 if operand is zero, 0 otherwise)

Shifts counts are wrapped to be less than the log-base-2 of the number of bits
in the value to be shifted, as an unsigned quantity. For example, in a 32-bit
shift, only the least 5 significant bits of the count affect the result. In a
64-bit shift, only the least 6 significant bits of the count affect the result.

Rotate counts are treated as unsigned.  A count value greater than or equal
to the number of bits in the value to be rotated yields the same result as
if the count was wrapped to its least significant N bits, where N is 5 for
an i32 value or 6 for an i64 value.

All comparison operators yield 32-bit integer results with `1` representing
`true` and `0` representing `false`.

## 64-bit integer operators

The same operators are available on 64-bit integers as the those available for
32-bit integers.

## Floating point operators

Floating point arithmetic follows the IEEE 754-2008 standard, except that:
 - The IEEE 754-2008 section 6.2 recommendation that operations propagate NaN
   bits from their operands is permitted but not required.
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

When the result of any arithmetic operation other than `neg`, `abs`, or
`copysign` is a NaN, the sign bit and the fraction field (which does not include
the implicit leading digit of the significand) of the NaN are computed as
follows:

 - If the operation has any NaN input values, implementations may select any of
   them to be the result value, but with the most significant bit of the
   fraction field overwritten to be 1.

 - If the implementation does not choose to use an input NaN as a result value,
   or if there are no input NaNs, the result value has a nondeterministic sign
   bit, a fraction field with 1 in the most significant bit and 0 in the
   remaining bits.

32-bit floating point operations are as follows:

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

If the operand of promotion is a NaN, the result is nondeterministically chosen
between the following:
 - a NaN with a nondeterministic sign bit and a fraction field with 1 in the
   most significant bit and 0 in the remaining bits.
 - a NaN with the sign bit of the operand and a fraction field consisting of
   1 in the most significant bit, followed by all but the most significant
   bits of the fraction field of the operand, followed by all 0s.

If the operand of demotion is a NaN, the result is nondeterministically chosen
between the following:
 - a NaN with a nondeterministic sign bit and a fraction field with 1 in the
   most significant bit and 0 in the remaining bits.
 - a NaN with the sign bit of the operand and a fraction field consisting of
   1 in the most significant bit, followed by as many of all but the most
   significant bit of the fraction field of the operand as fit.

Reinterpretations always succeed.

Conversions from integer to floating point always succeed, and use
round-to-nearest ties-to-even rounding.

Truncation from floating point to integer where IEEE 754-2008 would specify an
invalid operator exception (e.g. when the floating point value is NaN or
outside the range which rounds to an integer in range) traps.

## Type-parametric operators.

  * `select`: a ternary operator with two operands, which have the same type as
    each other, plus a boolean (i32) condition. `select` returns the first
    operand if the condition operand is non-zero, or the second otherwise.

## Unreachable

  * `unreachable`: An expression which can take on any type, and which, if
    executed, always traps. It is intended to be used for example after
    calls to functions which are known by the producer not to return (otherwise
    the producer would have to create another expression with an unused value
    to satisfy the type check). This trap is intended to be impossible for user
    code to catch or handle, even in the future when it may be possible to
    handle some other kinds of traps or exceptions.
