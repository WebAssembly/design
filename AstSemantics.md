# AST Semantics

The AST has a basic division between statements and expressions.

Each function body consists of exactly one statement.

The operations available in the AST are defined here in language-independent
way but closely match operations in many programming languages and are
efficiently implementable on all modern computers.
Floating point arithmetic follows the IEEE 754 standard and unless otherwise
specified uses the round-to-nearest mode.

## Addressing local variables

All local variables occupy a single index space local to the function.
Parameters are addressed as local variables.
Local variables do not have addresses and are not aliased in the globals
or the heap.
Each function has a fixed, pre-declared number of local variables.
Local variables are typed and initialized to the appropriate zero value for
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

Each heap access is annotated with the type of the location being accessed.
The legal set of types for heap accesses is larger than that for local variables
and AST nodes. In particular accesses to the heap may specify integer types
smaller than 32 bits. Loads are either sign extended or zero extended to 32-bit
integers, and stores of integers smaller than 32-bit include implicit truncations.

Indexes into the heap are always byte indexes.
Heap accesses can be unaligned.

  * LoadHeap - load a value from the heap at a given index
  * StoreHeap - store a given value to the heap at a given index

These AST nodes index the heap with 32-bit unsigned integers with the assumption the
specification will be extended with new AST nodes that allow 64-bit integers
as indexes.

The specification will add atomicity annotations in the future. Currently
all heap accesses can be considered "non-atomic".

The semantics of out-of-bounds heap accesses needs clarification. A polyfill
to asm.js and typed arrays would suggest ignoring out-of-bounds-writes and
returning an appropriate zero value for out-of-bounds reads as the default
semantics. Other possible semantics which are efficiently implementable 
include throwing an exception or allowing out-of-bound reads to return an
unspecified value.

To enable more aggresive hoisting of bounds checks, heap accesses may also include
an offset:

  * LoadHeapWithOffset - load a value from the heap at a given index plus a given immediate offset
  * StoreHeapWithOffset - store a given value to the heap at a given index plus a given immediate offset

The addition of the offset and index is specified to use infinite precision 
such that an out-of-bounds access never wraps around to an in-bounds access.
Bounds checking before the final offset addition allows the offset addition
to easily be folded into the hardware load instruction *and* for groups of loads
with the same base and different offsets to easily share a single bounds check.

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
that overflows comforms to the standard wrap-around semantics.
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

Note that greater-than and greater-than-or-equal operations are not required,
since "a < b" == "b > a" and "a <= b" == "b >= a". Such equalities also hold for
floating point comparisons, even considering NaN.

Additional 32-bit integer Operations under consideration:

  * Int32SMulHigh - signed multiplication (upper 32-bits)
  * Int32UMulHigh - unsigned multiplication (upper 32-bits)
  * Int32Clz - count leading zeroes
  * Int32Popcnt - count number of ones
  * Int32Not - signed-less one's complement
  * Int32SMin - signed minimum
  * Int32SMax - signed maximum
  * Int32UMin - unsigned minimum
  * Int32UMax - unsigned maximum

The behavior of division-by-zero and (INT32_MIN/-1) needs clarification. 
An efficient polyfill to asm.js would suggest division-by-zero results in 
0 although it's possible for the asm.js polyfill to simply be wrong in this 
corner case. Other options include throwing an exception or producing an 
unspecified 32-bit integer value.

## 64-bit Floating point operations

All 64-bit floating point operations conform to the IEEE-754 standard.

  * Float64Add - addition
  * Float64Sub - subtraction
  * Float64Mul - multiplication
  * Float64Div - division
  * Float64Abs - absolute value
  * Float64Ceil - ceiling operation
  * Float64Floor - floor operation
  * Float64Eq - compare equal
  * Float64Lt - less than
  * Float64Le - less than or equal

Operations under consideration:

  * Float64Sqrt - square root

## 32-bit Floating point operations

All 32-bit floating point operations conform to the IEEE-754 standard.

  * Float32Add - addition
  * Float32Sub - subtraction
  * Float32Mul - multiplication
  * Float32Div - division
  * Float32Abs - absolute value
  * Float32Ceil - ceiling operation
  * Float32Floor - floor operation
  * Float32Eq - compare equal
  * Float32Lt - less than
  * Float32Le - less than or equal

Operations under consideration:

  * Float32Sqrt - square root


Note that the IEEE 754 standard does not require extended operations
like transcendental functions to have a specified precision.
It does require them to define and use a consistent rounding quantum.

The rounding behavior of the operations under consideration needs clarification.

## Datatype conversions and truncations

Datatype conversions are mostly used to convert floating point numbers to
integers and vice versa. The exact details for out-of-range values and rounding
need further clarification.

  * Int32FromFloat64 - truncate a 64-bit float to a signed integer
  * Int32FromFloat32 - truncate a 32-bit float to a signed integer
  * Uint32FromFloat64 - truncate a 64-bit float to an unsigned integer
  * Uint32FromFloat32 - truncate a 32-bit float to an unsigned integer
  * Int32FromFloat32Bits - reinterpret the bits of a 32-bit float as a 32-bit integer
  * Float64FromFloat32 - convert a 32-bit float to a 64-bit float
  * Float64FromInt32 - convert a signed integer to a 64-bit float
  * Float64FromUInt32 - convert an unsigned integer to a 64-bit float
  * Float32FromFloat64 - truncate a 64-bit float to a 32-bit float
  * Float32FromInt32 - convert a signed integer to a 32-bit float
  * Float32FromUInt32 - convert an unsigned integer to a 32-bit float
  * Float32FromInt32Bits - reinterpret the bits of a 32-bit integer as a 32-bit float

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

