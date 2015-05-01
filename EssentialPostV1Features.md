# Essential Post-v.1 Features

This is a list of essential features we *know* we need but were not part of the
Minimum Viable Product that is [v.1](V1.md). Longer-term features will be added
in [future versions](FutureFeatures.md).

## 64-bit integers
* Provide access to efficient 64-bit arithmetic.
* Also allow heaps greater than 4gb and load/store op that take 64-bit operands.
* Some code will want to only use 64-bit integers when running on a 64-bit system.
  * For example, on a 32-bit system, a >4gb heap allocation will always OOM so there is no reason
    to use slow 64-bit ints for pointers.
  * Provide a "has native 64-bit integer" query.
  * Show we provide a uintptr_t (only 64-bit when the "has native 64-bit integer" query is true)?
    * This feature alone would not allow a C++ compiler to write size-polymorphic code since the word
      size is also baked into the code in a hundred other ways (consider `offsetof`).
    * The compiler *could* inflate all pointer types that are used in heap storage to 64-bit (so the
      uintptr_t type was only used for local variable/expression types). This could mostly work,
      though it would implicitly truncate on any load of a pointer from the heap which could cause
      subtle semantic bugs if the pointer was storing a non-pointer real-int64 value. It would also
      increase heap usage significantly (for pointer-heavy heaps).
    * So tentatively 'no'.

## Threads
* Shared memory
* Atomics
* Futex or [synchronic][]
* Thread-local storage

  [synchronic]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4195.pdf

## Fixed-width SIMD
* TODO

## Zero-cost Exception Handling
* Developer access to stack unwinding and inspection.
* This may be used to implement `setjmp`/`longjmp` (instead of the usual
  opposite approach). This can enable all of the defined behavior of
  `setjmp`/`longjmp`, namely unwinding the stack, but does not allow
  the undefined behavior case of jumping forward to a stack that
  was already unwound (which is sometimes used to implement coroutines;
  however, explicit coroutine support is being considered separately
  anyhow).

## Signature-restricted Proper Tail Calls
* This can also be used to support `goto` and irreducible control flow.
* TODO
