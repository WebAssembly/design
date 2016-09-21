# Abstract Syntax Tree Semantics

This document describes WebAssembly semantics. The description here is written
in terms of an Abstract Syntax Tree (AST), however it is also possible to
understand WebAssembly semantics in terms of a stack machine. (In practice,
implementations need not build an actual AST or maintain an actual stack; they
need only behave [as if](https://en.wikipedia.org/wiki/As-if_rule) they did so.)

This document explains the high-level design of the AST: its types, constructs, and
semantics. For full details consult [the formal Specification](https://github.com/WebAssembly/spec),
for file-level encoding details consult [Binary Encoding](BinaryEncoding.md),
and for the human-readable text representation consult [Text Format](TextFormat.md).

Each function body consists of a list of expressions. All expressions and
operators are typed, with no implicit conversions or overloading rules.

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

Call stack space is limited by unspecified and dynamically varying constraints
and is a source of [nondeterminism](Nondeterminism.md). If program call stack usage
exceeds the available call stack space at any time, a trap occurs.

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

A *linear memory* is a contiguous, byte-addressable range of memory spanning
from offset `0` and extending up to a varying *memory size*. This size is always 
a multiple of the WebAssembly page size, which is fixed to 64KiB (though large
page support may be added in an opt-in manner in the 
[future](FutureFeatures.md#large-page-support)). The initial state of a linear
memory is defined by the module's [linear memory](Modules.md#linear-memory-section) and
[data](Modules.md#data-section) sections. The memory size can be dynamically
increased by the [`grow_memory`](AstSemantics.md#resizing) operator.

A linear memory can be considered to be an untyped array of bytes, and it is
unspecified how embedders map this array into their process' own [virtual
memory][]. Linear memory is sandboxed; it does not alias other linear memories,
the execution engine's internal data structures, the execution stack, local
variables, or other process memory.

  [virtual memory]: https://en.wikipedia.org/wiki/Virtual_memory

Every WebAssembly [instance](Modules.md) has one specially-designated *default* 
linear memory which is the linear memory accessed by all the 
[memory operators below](#linear-memory-access). In the MVP, there are *only*
default linear memories but [new memory operators](FutureFeatures.md#multiple-tables-and-memories)
may be added after the MVP which can also access non-default memories.

Linear memories (default or otherwise) can either be [imported](Modules.md#imports)
or [defined inside the module](Modules.md#linear-memory-section). After import
or definition, there is no difference when accessing a linear memory whether it
was imported or defined internally.

In the MVP, linear memory cannot be shared between threads of execution.
The addition of [threads](PostMVP.md#threads) will allow this.

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

Store operators do not produce a value.

The above operators operate on the [default linear memory](#linear-memory).

### Addressing

Each linear memory access operator has an address operand and an unsigned
integer byte offset immediate. The infinite-precision unsigned sum of the
address operand's value with the offset's value is called the *effective
address*, which is interpreted as an unsigned byte index into the linear memory.

Linear memory operators access the bytes starting at the effective address and
extend for the number of bytes implied by the storage size. If any of the
accessed bytes are beyond the current memory size, the access is considered
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
of 2 alignment attribute. An alignment value which is the same as the memory
attribute size is considered to be a *natural* alignment. The alignment applies
to the effective address and not merely the address operand, i.e. the immediate
offset is taken into account when considering alignment.

The alignment has same type (determined by wasm32/wasm64, as described above) as
the address and offset operands.

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

When a linear memory has a declared [maximum memory size](Modules.md#linear-memory-section),
`grow_memory` must fail if it would grow past the maximum. However,
`grow_memory` may still fail before the maximum if it was not possible to
reserve the space up front or if enabling the reserved memory fails.
When there is no maximum memory size declared, `grow_memory` is expected
to perform a system allocation which may fail.

The current size of the linear memory can be queried by the following operator:

  * `current_memory` : return the current memory size in units of pages.

As stated [above](AstSemantics.md#linear-memory), linear memory is contiguous,
meaning there are no "holes" in the linear address space. After the
MVP, there are [future features](FutureFeatures.md#finer-grained-control-over-memory)
proposed to allow setting protection and creating mappings within the
contiguous linear memory.

In the MVP, memory can only be grown. After the MVP, a memory shrinking
operator may be added. However, due to normal fragmentation, applications are
instead expected release unused physical pages from the working set using the
[`discard`](FutureFeatures.md#finer-grained-control-over-memory) future feature.

The above operators operate on the [default linear memory](#linear-memory).

## Table

A *table* is similar to a linear memory whose elements, instead of being bytes,
are opaque values of a particular *table element type*. This allows the table to
contain values—like GC references, raw OS handles, or native pointers—that are
accessed by WebAssembly code indirectly through an integer index. This feature
bridges the gap between low-level, untrusted linear memory and high-level
opaque handles/references at the cost of a bounds-checked table indirection.

The table's element type constrains the type of elements stored 
in the table and allows engines to avoid some type checks on table use. 
When a WebAssembly value is stored in a table, the value's type must precisely
match the element type. Just like linear memory, updates to a table are
observed immediately by all instances that reference the table. Depending on the
operator/API used to store the value, this check may be static or dynamic. Host
environments may also allow storing non-WebAssembly values in tables in which
case, as with [imports](Modules.md#imports), the meaning of using the value is
defined by the host environment.

Every WebAssembly [instance](Modules.md) has one specially-designated *default*
table which is indexed by [`call_indirect`](#calls) and other future
table operators. Tables can either be [imported](Modules.md#imports) or 
[defined inside the module](Modules.md#table-section). After import or
definition, there is no difference when calling into a table whether it was
imported or defined internally.

In the MVP, the primary purpose of tables is to implement indirect function
calls in C/C++ using an integer index as the pointer-to-function and the table
to hold the array of indirectly-callable functions. Thus, in the MVP:
* tables may only be accessed from WebAssembly code via [`call_indirect`](#calls);
* the only allowed table element type is `anyfunc` (function with any signature);
* tables may not be directly mutated or resized from WebAssembly code;
  this can only be done through the host environment (e.g., the
  the `WebAssembly` [JavaScript API](JS.md#webassemblytable-objects)).

These restrictions may be relaxed in the 
[future](FutureFeatures.md#more-table-operators-and-types). 

## Local variables

Each function has a fixed, pre-declared number of *local variables* which occupy a single
index space local to the function. Parameters are addressed as local variables. Local
variables do not have addresses and are not aliased by linear memory. Local
variables have [value types](#types) and are initialized to the appropriate zero value for their
type at the beginning of the function, except parameters which are initialized to the values
of the arguments passed to the function.

  * `get_local`: read the current value of a local variable
  * `set_local`: set the current value of a local variable
  * `tee_local`: like `set_local`, but also returns the set value

The details of index space for local variables and their types will be further clarified,
e.g. whether locals with type `i32` and `i64` must be contiguous and separate from
others, etc.

## Global variables

A *global variable* stores a single value of a fixed [value type](#types) and may be
declared either *mutable* or *immutable*. This provides WebAssembly with memory
locations that are disjoint from any [linear memory](#linear-memory) and thus
cannot be arbitrarily aliased as bits.

Global variables are accessed via an integer index into the module-defined 
[global index space](Modules.md#global-index-space). Global variables can 
either be [imported](Modules.md#imports) or [defined inside the module](Modules.md#global-section).
After import or definition, there is no difference when accessing a global.

  * `get_global`: get the current value of a global variable
  * `set_global`: set the current value of a global variable

It is a validation error for a `set_global` to index an immutable global variable.

In the MVP, the primary use case of global variables is to represent
instantiation-time immutable values as a useful building block for
[dynamic linking](DynamicLinking.md).

After the MVP, when [reference types](GC.md) are added to the set of [value types](#types),
global variables will be necessary to allow sharing reference types between
[threads](PostMVP.md#threads) since shared linear memory cannot load or store
references.

## Control flow structures

WebAssembly offers basic structured control flow with the following constructs.
Since all AST nodes are expressions in WebAssembly, control constructs may yield
a value and may appear as children of other expressions.

 * `nop`: an empty operator that does not yield a value 
 * `block`: a fixed-length sequence of expressions with a label at the end
 * `loop`: a block with an additional label at the beginning which may be used to form loops
 * `if`: if expression with a list of *then* expressions and a list of *else* expressions
 * `br`: branch to a given label in an enclosing construct
 * `br_if`: conditionally branch to a given label in an enclosing construct
 * `br_table`: a jump table which jumps to a label in an enclosing construct
 * `return`: return zero or more values from this function

### Branches and nesting

The `br` and `br_if` constructs express low-level branching.
Branches may only reference labels defined by an outer *enclosing construct*,
which can be a `block` (with a label at the `end`), `loop` (with a label at the
beginning), `if` (with a label at the `end` or `else`), `else` (with a label at
the `end`), or the function body (with a label at the `end`). This means that,
for example, references to a `block`'s label can only occur within the
`block`'s body.

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

The `nop`, `br`, `br_if`, `br_table`, and `return` constructs do not yield values.
Other control constructs may yield values if their subexpressions yield values:

* `block`: yields either the value of the last expression in the block or the result of an inner branch that targeted the label of the block
* `loop`: yields the value of the last expression in the loop
* `if`: yields either the value of the last *then* expression or the last *else* expression or the result of an inner branch that targeted the label of one of these.

In all constructs containing block-like sequences of expressions, all expressions but the last must not yield a value.
The `drop` operator can be used to explicitly discard unwanted expression results.

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

Direct calls to a function specify the callee by an index into the 
[function index space](Modules.md#function-index-space).

  * `call`: call function directly

A direct call to a function with a mismatched signature is a module verification error.

Indirect calls to a function indicate the callee with an `i32` index into
a [table](#table). The *expected* signature of the target function (specified
by its index in the [types section](BinaryEncoding.md#type-section)) is given as
a second immediate.

  * `call_indirect`: call function indirectly

Unlike `call`, which checks that the caller and callee signatures match
statically as part of validation, `call_indirect` checks for signature match
*dynamically*, comparing the caller's expected signature with the callee function's
signature and and trapping if there is a mismatch. Since the callee may be in a
different module which necessarily has a separate [types section](BinaryEncoding.md#type-section),
and thus index space of types, the signature match must compare the underlying 
[`func_type`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/types.ml#L5).
As noted [above](#table), table elements may also be host-environment-defined
values in which case the meaning of a call (and how the signature is checked)
is defined by the host-environment, much like calling an import.

In the MVP, the single `call_indirect` operator accesses the [default table](#table).

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

 - If the fraction fields of all NaN inputs to the instruction all consist
   of 1 in the most significant bit and 0 in the remaining bits, or if there are
   no NaN inputs, the result is a NaN with a nondeterministic sign bit, 1 in the
   most significant bit of the fraction field, and all zeros in the remaining
   bits of the fraction field.

 - Otherwise the result is a NaN with a nondeterministic sign bit, 1 in the most
   significant bit of the fraction field, and nondeterminsitic values in the
   remaining bits of the fraction field.

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
   bit of the fraction field of the operand, followed by all 0s.

If the operand of demotion is a NaN, the result is nondeterministically chosen
between the following:
 - a NaN with a nondeterministic sign bit and a fraction field with 1 in the
   most significant bit and 0 in the remaining bits.
 - a NaN with the sign bit of the operand and a fraction field consisting of
   1 in the most significant bit, followed by all but the most significant bit
   of the fraction field of the operand, discarding the least significant bits
   that don't fit.

Reinterpretations always succeed.

Conversions from integer to floating point always succeed, and use
round-to-nearest ties-to-even rounding.

Truncation from floating point to integer where IEEE 754-2008 would specify an
invalid operator exception (e.g. when the floating point value is NaN or
outside the range which rounds to an integer in range) traps.

## Type-parametric operators.

  * `drop`: a unary operator that discards the value of its operand.
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
