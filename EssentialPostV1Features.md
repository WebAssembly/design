# Essential Post-v.1 Features

This is a list of essential features we *know* we need but were not part of the
Minimum Viable Product that is [v.1](V1.md). Longer-term features will be added
in [future versions](FutureFeatures.md).


*This is just a skeleton list to start a discussion*

## 64-bit integers
* TODO

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
