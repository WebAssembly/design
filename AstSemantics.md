# AST Semantics

The AST has a basic division between statements and expressions.

Each function body consists of exactly one statement.

The operations available in the AST are defined here in language-independent
way but closely match operations in many programming languages and are
efficiently implementable on all modern computers.

Some operations may *trap* under some conditions, as noted below. In v.1,
trapping means that execution in the WebAssembly module is terminated and
abnormal termination is reported to the outside environment. In a browser
environment, this would translate into a JS exception being thrown. If
developer tools are active, attaching a debugger before the termination
would be sensible.

Callstack space is limited by unspecified and dynamically varying constraints.
If program callstack usage exceeds the available callstack space at any time,
a trap occurs.

## Local and Memory types

Individual storage locations in WebAssembly are typed, including global
variables, local variables, and parameters. The heap itself is not typed, but
all accesses to the heap are annotated with a type. The legal types for
global variables and heap accesses are called *Memory types*.

  * Int8 - signed 8-bit integer
  * Int16 - signed 16-bit integer
  * Int32 - signed 32-bit integer
  * Uint8 - unsigned 8-bit integer
  * Uint16 - unsigned 16-bit integer
  * Uint32 - unsigned 32-bit integer
  * Float32 - 32-bit floating point
  * Float64 - 64-bit floating point

The legal types for parameters and local variables, called *Local types*
are a subset of the Memory types:

  * Int32 - 32-bit integer
  * Float32 - 32-bit floating point
  * Float64 - 64-bit floating point

All IR operations except loads and stores deal with local types. Loads implicitly
convert Memory types to Local types according to the follow rules:

  * Load[Int8] - sign-extend to Int32
  * Load[Int16] - sign-extend to Int32
  * Load[Int32] - (no conversion)
  * Load[Uint8] - zero-extend to Int32
  * Load[Uint16] - zero-extend to Int32
  * Load[Uint32] - reinterpret as Int32
  * Load[Float32] - (no conversion)
  * Load[Float64] - (no conversion)

Note that the local type Int32 does not technically have a sign; the sign bit
is interpreted differently by the operations below.

Similar to loads, stores implicitly truncate Local types to Memory types according to the
following rules:

  * Store[Int8] - truncate Int32 to Int8
  * Store[Int16] - truncate Int32 to Int16
  * Store[Int32] - (no truncation)
  * Store[Uint8] - truncate Int32 to Uint8
  * Store[Uint16] - truncate Int32 to Uint16
  * Store[Uint32] - reinterpret Int32 as Uint32
  * Store[Float32] - (no truncation)
  * Store[Float64] - (no truncation)

Truncation of integers simply discards any upper bits; i.e. truncation does not perform saturation,
trap on overflow, etc.

## Addressing local variables

All local variables occupy a single index space local to the function.
Parameters are addressed as local variables.
Local variables do not have addresses and are not aliased in the globals
or the heap.
Each function has a fixed, pre-declared number of local variables.
Local variables have *Local types* and are initialized to the appropriate zero value for
their type at the beginning of the function, except parameters which are
initialized to the values of the arguments passed to the function.

  * GetLocal - read the current value of a local variable
  * SetLocal - set the current value of a local variable

The details of index space for local variables and their types needs clarification,
e.g. whether locals with type int32 must be contiguous and separate from others,
etc.

## Control flow structures

WebAssembly offers basic structured control flow. All control flow structures
are statements.

  * Block - a fixed-length sequence of statements
  * If - if statement
  * Do-While - do while statement, basically a loop with a
    conditional branch (back to the top of the loop)
  * Forever - infinite loop statement (like while(1)), basically an
    unconditional branch (back to the top of the loop)
  * Continue - continue to start of nested loop
  * Break - break to end from nested loop or block
  * Return - return zero or more values from this function
  * Switch - switch statement with fallthrough

Break and continue statements can only target blocks or loops in which they are
nested. This guarantees that all resulting control flow graphs are reducible
and that producing asm.js from WebAssembly does not require running the relooper
algorithm.

## Accessing the heap

Each heap access is annotated with a *Memory type* and
the presumed alignment of the incoming pointer. As discussed previously, loads may
include implicit zero- or sign-extension and stores may include implicit truncation.

Indexes into the heap are always byte indexes.

  * LoadHeap - load a value from the heap at a given index with given alignment
  * StoreHeap - store a given value to the heap at a given index with given alignment

To enable more aggressive hoisting of bounds checks, heap accesses may also include
an offset:

  * LoadHeapWithOffset - load a value from the heap at a given index plus a given immediate offset
  * StoreHeapWithOffset - store a given value to the heap at a given index plus a given immediate offset

The addition of the offset and index is specified to use infinite precision 
such that an out-of-bounds access never wraps around to an in-bounds access.
Bounds checking before the final offset addition allows the offset addition
to easily be folded into the hardware load instruction *and* for groups of loads
with the same base and different offsets to easily share a single bounds check.

In v.1, the indices are 32-bit unsigned integers. With 
[64-bit integers](EssentialPostV1Features.md#64-bit-integers) and
[>4GiB heaps](FutureFeatures.md#heaps-bigger-than-4gib), these nodes would also
accept 64-bit unsigned integers.

In v.1, heaps are not shared between threads. When
[threads](EssentialPostV1Features.md#threads) are added as a feature, the basic
`LoadHeap`/`StoreHeap` nodes will have the most relaxed semantics specified in
the memory model and new heap-access nodes will be added with atomic and
ordering guarantees.

Two important semantic cases are **misaligned** and **out-of-bounds** accesses:

### Alignment

If the incoming index argument to a heap access is misaligned with respect to
the alignment operand of the heap access, the heap access must still work
correctly (as if the alignment operand was 1). However, on some platforms, such
misaligned accesses may incur a *massive* performance penalty (due to trap
handling). Thus, it is highly recommend that every WebAssembly producer provide
accurate alignment. Note: on platforms without unaligned accesses, smaller-than-natural
alignment may result in slower code generation (due to the whole access being
broken into smaller aligned accesses).

Either tooling or an explicit opt-in "debug mode" in the spec should allow
execution of a module in a mode that threw exceptions on misaligned access.
(This mode would incur some runtime cost for branching on most platforms which
is why it isn't the specified default.)

### Out of bounds

The ideal semantics is for out-of-bounds accesses to trap. A module may
optionally define that "out of bounds" includes low-memory accesses.

There are several possible variations on this design being discussed and
experimented with. More measurement is required to understand the optimization
opportunities provided by each.

  * After an out-of-bounds access, the module can no longer execute code and any
    outstanding JS ArrayBuffers aliasing the heap are detached.
    * This would primarily allow hoisting bounds checks above effectful
      operations.
    * This can be viewed as a mild security measure under the assumption that
      while the sandbox is still ensuring safety, the module's internal state
      is incoherent and further execution could lead to Bad Things (e.g., XSS
      attacks).
  * To allow for potentially more-efficient heap sandboxing, the semantics could
    allow for a non-deterministic choice between one of the following when an
    out-of-bounds access occurred.
    * The ideal trap semantics.
    * Loads return an unspecified value.
    * Stores are either ignored or store to an unspecified location in the heap.
    * Either tooling or an explicit opt-in "debug mode" in the spec should allow
      execution of a module in a mode that threw exceptions on out-of-bounds
      access.

## Accessing globals

Global accesses always specify a single global variable with a constant index.
Every global has exactly one type. Global variables are not aliased to the heap.

  * LoadGlobal - load the value of a given global variable
  * StoreGlobal - store a given value to a given global variable

The specification will add atomicity annotations in the future. Currently
all global accesses can be considered "non-atomic".

## Calls

Direct calls to a function specify the callee by index into a function table.

  * CallDirect - call function directly

Each function has a signature in terms of local types, and calls must match the
function signature exactly. [Imported functions](V1.md#code-loading-and-imports) 
also have signatures and are added to the same function table and are thus also
callable via `CallDirect`.

Indirect calls may be made to a value of function-pointer type. A function-
pointer value may be obtained for a given function as specified by its index
in the function table.

  * CallIndirect - call function indirectly
  * AddressOf - obtain a function pointer value for a given function

Function-pointer values are comparable for equality and the `AddressOf` operator
is monomorphic. Function-pointer values can be explicitly coerced to and from
integers (which, in particular, is necessary when loading/storing to the heap
since the heap only provides integer types). For security and safety reasons,
the integer value of a coerced function-pointer value is an abstract index and
does not reveal the actual machine code address of the target function.

In v.1 function pointer values are
local to a single module. The [dynamic linking](FutureFeatures.md#dynamic-linking)
feature is necessary for two modules to pass function pointers back and forth.

Multiple return value calls will be possible, though possibly not in v.1. The
details of multiple-return-value calls needs clarification. Calling a function
that returns multiple values will likely have to be a statement that specifies
multiple local variables to which to assign the corresponding return values.

## Literals

All basic data types allow literal values of that data type. See the
[binary encoding section](BinaryEncoding.md#constant-pool).

## Expressions with control flow

Expression trees offer significant size reduction by avoiding the need for
`SetLocal`/`GetLocal` pairs in the common case of an expression with only one,
immediate use.  The following primitives provide AST nodes that express
control flow and thus allow more opportunities to build bigger expression trees
and further reduce `SetLocal`/`GetLocal` usage (which constitute 30-40% of total 
bytes in the polyfill prototype). Additionally, these primitives are useful 
building blocks for WebAssembly-generators (including the asm.js polyfill).

  * Comma - evaluate and ignore the result of the first operand, evaluate and return the second operand
  * Conditional - basically ternary ?: operator

New operands should be considered which allow measurably greater expression-tree-building
opportunities.

## 32-bit Integer operations

Most operations available on 32-bit integers are sign-independent.
Signed integers are always represented as two's complement and arithmetic
that overflows conforms to the standard wrap-around semantics.
All comparison operations yield 32-bit integer results with 1 representing true
and 0 representing false.

  * Int32Add - signed-less addition
  * Int32Sub - signed-less subtraction
  * Int32Mul - signed-less multiplication (lower 32-bits)
  * Int32SDiv - signed division
  * Int32UDiv - unsigned division
  * Int32SRem - signed remainder
  * Int32URem - unsigned remainder
  * Int32And - signed-less logical and
  * Int32Ior - signed-less inclusive or
  * Int32Xor - signed-less exclusive or
  * Int32Shl - signed-less shift left
  * Int32Shr - unsigned shift right
  * Int32Sar - signed arithmetic shift right
  * Int32Eq  - signed-less compare equal
  * Int32Slt - signed less than
  * Int32Sle - signed less than or equal
  * Int32Ult - unsigned less than
  * Int32Ule - unsigned less than or equal

Division or remainder by zero traps.
Signed division overflow (e.g. INT32_MIN/-1) traps.

Shifts interpret their shift count operand as an unsigned value. When the
shift count is at least the bitwidth of the shift, Shl and Shr return 0,
and Sar returns 0 if the value being shifted is non-negative, and -1 otherwise.

Note that greater-than and greater-than-or-equal operations are not required,
since "a < b" == "b > a" and "a <= b" == "b >= a". Such equalities also hold for
floating point comparisons, even considering NaN.

Additional 32-bit integer Operations under consideration:

  * Int32SMulHigh - signed multiplication (upper 32-bits)
  * Int32UMulHigh - unsigned multiplication (upper 32-bits)
  * Int32Clz - count leading zeroes (defined for all values, including 0)
  * Int32Ctz - count trailing zeroes (defined for all values, including 0)
  * Int32Popcnt - count number of ones
  * Int32BSwap - reverse bytes (endian conversion)
  * Int32Rotr - bitwise rotate right
  * Int32Rotl - bitwise rotate left
  * Int32Not - signed-less one's complement
  * Int32SMin - signed minimum
  * Int32SMax - signed maximum
  * Int32UMin - unsigned minimum
  * Int32UMax - unsigned maximum

## Floating point operations

Floating point arithmetic follows the IEEE-754 standard, except that:
 - The sign bit and significand of any NaN value returned from a floating point
   arithmetic operation other than Neg, Abs, and Copysign are computed from an
   unspecified function of the implementation, the opcode, and the operands.
 - Except where otherwise specified, floating point operations use the
   round-to-nearest ties-to-even rounding attribute.
 - Floating point exceptions are not reported (and consequently, no distinction
   is made between quiet and signalling NaN). However, infinity, -infinity, and
   NaN are still produced as result values to indicate overflow, invalid, and
   divide-by-zero conditions, as specified by IEEE-754.

  * Float32Add - addition
  * Float32Sub - subtraction
  * Float32Mul - multiplication
  * Float32Div - division
  * Float32Abs - absolute value
  * Float32Neg - negation
  * Float32Copysign - copysign
  * Float32Ceil - ceiling operation
  * Float32Floor - floor operation
  * Float32Eq - compare equal
  * Float32Lt - less than
  * Float32Le - less than or equal
  * Float32Sqrt - square root

  * Float64Add - addition
  * Float64Sub - subtraction
  * Float64Mul - multiplication
  * Float64Div - division
  * Float64Abs - absolute value
  * Float64Neg - negation
  * Float64Copysign - copysign
  * Float64Ceil - ceiling operation
  * Float64Floor - floor operation
  * Float64Eq - compare equal
  * Float64Lt - less than
  * Float64Le - less than or equal
  * Float64Sqrt - square root

Operations under consideration:

  * Float32Min - minimum; if either operand is NaN, returns NaN
  * Float32Max - maximum; if either operand is NaN, returns NaN
  * Float32MinNum - minimum; if exactly one operand is NaN, returns the other operand
  * Float32MaxNum - maximum; if exactly one operand is NaN, returns the other operand
  * Float32Trunc - round to nearest integer towards zero
  * Float32NearestInt - round to nearest integer, ties to even

  * Float64Min - minimum; if either operand is NaN, returns NaN
  * Float64Max - maximum; if either operand is NaN, returns NaN
  * Float64MinNum - minimum; if exactly one operand is NaN, returns the other operand
  * Float64MaxNum - maximum; if exactly one operand is NaN, returns the other operand
  * Float64Trunc - round to nearest integer towards zero
  * Float64NearestInt - round to nearest integer, ties to even

Min, Max, MinNum, and MaxNum operations would treat -0 as being effectively less than 0.

## Datatype conversions, truncations, reinterpretations, promotions, and demotions

  * Sint32FromFloat64 - truncate a 64-bit float to a signed integer
  * Sint32FromFloat32 - truncate a 32-bit float to a signed integer
  * Uint32FromFloat64 - truncate a 64-bit float to an unsigned integer
  * Uint32FromFloat32 - truncate a 32-bit float to an unsigned integer
  * Int32FromFloat32Bits - reinterpret the bits of a 32-bit float as a 32-bit integer
  * Float64FromFloat32 - promote a 32-bit float to a 64-bit float
  * Float64FromSint32 - convert a signed integer to a 64-bit float
  * Float64FromUint32 - convert an unsigned integer to a 64-bit float
  * Float32FromFloat64 - demote a 64-bit float to a 32-bit float
  * Float32FromSint32 - convert a signed integer to a 32-bit float
  * Float32FromUint32 - convert an unsigned integer to a 32-bit float
  * Float32FromInt32Bits - reinterpret the bits of a 32-bit integer as a 32-bit float

Promotion and demotion of floating point values always succeeds.
Demotion of floating point values uses round-to-nearest ties-to-even rounding,
and may overflow to infinity or negative infinity as specified by IEEE-754.
If the operand of promotion or demotion is NaN, the sign bit and significand
of the result are computed from an unspecified function of the implementation,
the opcode, and the operand.

Reinterpretations always succeed.

Conversions from integer to floating point always succeed, though they may
overflow to infinity or negative infinity as specified by IEEE-754.

Conversion from floating point to integer where IEEE-754 would specify an
invalid operation exception (e.g. when the floating point value is NaN or
outside the range which rounds to an integer in range) traps.

## Post-v.1 intrinsics

The following list of intrinsics is being considered for addition after v.1. The
rationale is that, for v.1, these operations can be statically linked into the
WebAssembly module by the code generator at small size cost and this avoids a
non-trivial specification burden of their semantics/precision. Adding these
intrinsics post-v.1 would allow for better high-level backend optimization of
these intrinsics that require builtin knowledge of their semantics. On the other
hand, a code generator may continue to statically link in its own implementation
since this provides greater control over precision/performance tradeoffs.

  * Float64Sin - trigonometric sine
  * Float64Cos - trigonometric cosine
  * Float64Tan - trigonometric tangent
  * Float64ASin - trigonometric arcsine
  * Float64ACos - trigonometric arccosine
  * Float64ATan - trigonometric  arctangent
  * Float64ATan2 - trigonometric arctangent with two arguments
  * Float64Exp - exponentiate e
  * Float64Ln - natural logarithm
  * Float64Pow - exponentiate
  * Float32Sin - trigonometric sine
  * Float32Cos - trigonometric cosine
  * Float32Tan - trigonometric tangent
  * Float32ASin - trigonometric arcsine
  * Float32ACos - trigonometric arccosine
  * Float32ATan - trigonometric  arctangent
  * Float32ATan2 - trigonometric arctangent with two arguments
  * Float32Exp - exponentiate e
  * Float32Ln - natural logarithm
  * Float32Pow - exponentiate

The rounding behavior of these operations would need clarification.
