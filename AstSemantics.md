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
  * While - while statement
  * For - for statement
  * Continue - continue to start of nested loop
  * Break - break to end from nested loop or block
  * Return - return zero or more values from this function

Break and continue statements can only target blocks or loops in which they are
nested. This guarantees that all resulting control flow graphs are reducible
and that producing asm.js from WebAssembly does not require running the relooper
algorithm.

## Accessing the heap

Each heap access is annotated with the type of the location being accessed and
the presumed alignment of the incoming pointer. The legal set of types for heap
accesses is larger than that for local variables and AST nodes. In particular
accesses to the heap may specify integer types smaller than 32 bits. Loads are
either sign extended or zero extended to 32-bit integers, and stores of integers
smaller than 32-bit include implicit truncations.

Indexes into the heap are always byte indexes.

  * LoadHeap - load a value from the heap at a given index with given alignment
  * StoreHeap - store a given value to the heap at a given index with given alignment

To enable more aggresive hoisting of bounds checks, heap accesses may also include
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
handling). Thus, a WebAssembly code generator should *always* provide accurate
alignment. Note: on platforms without unaligned accesses, smaller-than-natural
alignment may result in slower code generation (due to the whole access being
broken into smaller aligned accesses).

Since misaligned loads/stores are guaranteed to produce correct results and
heap accesses in asm.js force alignment (e.g., `HEAP32[i>>2]` masks off the
low two bits), the asm.js polyfill would need to translate *all* loads/stores
into byte accesses (regardless of specified alignment) to be correct. However,
to achieve competitive performance, the polyfill defaults to incorrect behavior
by emitting full-size accesses as if the index was never misaligned. This 
follows from the [high-level goals](HighLevelGoals.md). Thus, code generators
have *two* strong reasons to always emit accurate alignment.

Either tooling or an explicit opt-in "debug mode" in the spec should allow
execution of a module in a mode that threw exceptions on misaligned access.
(This mode would incur some runtime cost for branching on most platforms which
is why it isn't the specified default.)

### Out of bounds

The ideal semantics is for out-of-bounds accesses to throw. A module may
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
    * The ideal throw semantics.
    * Loads return an unspecified value.
    * Stores are either ignored or store to an unspecified location in the heap.
    * Either tooling or an explicit opt-in "debug mode" in the spec should allow
      execution of a module in a mode that threw exceptions on out-of-bounds
      access.

Lastly, regardless of the above semantics, the asm.js polyfill would be
intentionally incorrect for performance reasons (see
[high-level design goals](HighLevelDesignGoals.md)) and do what it does now,
which is to ignore out-of-bound stores and return 0/NaN for out-of-bound load.

## Accessing globals

Global accesses always specify a single global variable with a constant index.
Every global has exactly one type. Global variables are not aliased to the heap.

  * LoadGlobal - load the value of a given global variable
  * StoreGlobal - store a given value to a given global variable

The specification will add atomicity annotations in the future. Currently
all global accesses can be considered "non-atomic".

## Calls

Calls to functions can be direction (by index), or indirect. Each function
has a signature in terms of local types, and calls must match the function
signature exactly. Multiple return value calls will be possible.

The details of indirect calls need clarification.

  * CallDirect - call function directly
  * CallIndirect - call function indirectly

The details of multiple-return-value calls needs clarification.
Calling a function that returns multiple values will likely have to be a
statement that specifies multiple local variables to which to assign the
corresponding return values.

Calls to external (stdlib) functions can be expressed as direct calls to
an "external" function in the function table. The linking of that function
will be specified through the module system. A call to an unlinked function
should be a runtime error.

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

Additional 32-bit integer Operations under consideration:

  * Int32SMulHigh - signed multiplication (upper 32-bits)
  * Int32UMulHigh - unsigned multiplication (upper 32-bits)
  * Int32Clz - count leading zeroes
  * Int32Not - signed-less one's complement
  * Int32SMin - signed minimum
  * Int32SMax - signed maximum
  * Int32UMin - unsigned minimum
  * Int32UMax - unsigned maximum

The behavior of division-by-zero and other operations that cannot be represented
in 32-bit integers needs clarification. An efficient polyfill to asm.js would
suggest division-by-zero results in 0. Other options include throwing an
exception or producing an unspecified 32-bit integer value.

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
  * Float64Sin - trigonometric sine
  * Float64Cos - trigonometric cosine
  * Float64Tan - trigonometric tangent
  * Float64ASin - trigonometric arcsine
  * Float64ACos - trigonometric arccosine
  * Float64ATan - trigonometric  arctangent
  * Float64ATan2 - trigonometric arctangent
  * Float64Exp - exponentiate e
  * Float64Ln - natural logarithm
  * Float64Pow - exponentiate

## 32-bit Floating point operations

All 32-bit floating point operations conform to the IEEE-754 standard.

  * Float32Add - addition
  * Float32Sub - subtraction
  * Float32Mul - multiplication
  * Float32Div - division
  * Float32Eq - compare equal
  * Float32Lt - less than
  * Float32Le - less than or equal

Operations under consideration:

  * Float32Abs - absolute value
  * Float32Ceil - ceiling operation
  * Float32Floor - floor operation
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
  * Float64FromFloat32 - convert a 32-bit float to a 64-bit float
  * Float64FromInt32 - convert a signed integer to a 64-bit float
  * Float64FromUInt32 - convert an unsigned integer to a 64-bit float
  * Float32FromFloat32 - truncate a 32-bit float to a 32-bit float
  * Float32FromInt32 - convert a signed integer to a 32-bit float
  * Float32FromUInt32 - convert an unsigned integer to a 32-bit float

Note that greater-than and greater-than-or-equal operations are not required,
since "a < b" == "b > a" and "a <= b" == "b >= a". Such equalities also hold for
floating point comparisons, even considering NaN.

