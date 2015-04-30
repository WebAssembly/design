# Essential Post-v.1 Features

This is a list of essential features we *know* we need but were not part of the
Minimum Viable Product that is [v.1](V1.md). These will be developed following
the [high-level goals](HighLevelGoals.md), and prioritized based on developer
feedback.


*This is just a skeleton list to start a discussion*

## 64-bit integers
* TODO

## Threads
* Atomics
* Futex or [synchronic][]
* Thread-local storage

  [synchronic]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4195.pdf

## Fixed-width SIMD
* TODO

## Dynamic loading
* TODO

## Platform-independent Just-in-Time compilatione
* TODO
* Potentially through dynamic loading.

## Fine-grained memory management
* Shared memory
* `mmap`/`munmap`/`madvise`

## Zero-cost Exception Handling
* Developer access to stack unwinding and inspection.

## Garbage collection
* TODO

## Signature-restricted Proper Tail Calls
* TODO

## Irreducible control flow
* TODO

## Extra math
The following features may not work the same on all platforms:
* Fused multiply-add.
* Reciprocal square-root approximate.
* 16-bit floating-point values.
