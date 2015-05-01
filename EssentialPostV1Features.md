# Essential Post-v.1 Features

This is a list of essential features we *know* we need but were not part of the
Minimum Viable Product that is [v.1](V1.md). Longer-term features will be added
in [future versions](FutureFeatures.md).

## 64-bit integers
* TODO

## Threads
* Provide low-level buildings blocks for pthreads-style shared memory: shared memory,
  atomics + futexes (or [synchronics](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4195.pdf)).
* Import [SharedArrayBuffer proposal](https://docs.google.com/document/d/1NDGA_gZJ7M7w1Bh8S0AoDyEqwDdRh4uSoTPSNn77PFk).
  * The goal is to reuse the specification of memory model, happens-before, etc (with TC39) and backend implementation
    (same IR nodes and semantic invariants preserved).
* Modules can have global variables that are either shared or thread-local.
  * While the heap could be used for shared global variables, global variables are not aliasable
    and thus allow more aggressive optimization.
* Initially, a WebAssembly module is distributed between workers via `postMessage()`.
  * This also has the effect of explicitly sharing code so that engines don't
    perform N fetches and compile N copies.
  * May later standardize a more direct way to create a thread from WebAssembly.

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
