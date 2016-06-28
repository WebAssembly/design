# Essential Post-MVP Features

Some features are known to be essential and needed as soon as possible but aren't
in the [Minimum Viable Product (MVP)](MVP.md) because there isn't yet a
portably-efficient [polyfill](Polyfill.md) via JavaScript. There is a much
bigger [list of features](FutureFeatures.md) that will be added after these
essential features.

Post-MVP features will be available under [feature tests](FeatureTest.md).

## Threads

Provide low-level buildings blocks for pthreads-style shared memory: shared
memory between threads, atomics and futexes (or [synchronic][]).

New atomic memory operators, including loads/stores annotated with their atomic
ordering property, will follow the [C++11 memory model][], similarly to the
[PNaCl atomic support][] and the [SharedArrayBuffer][] proposal. Regular loads
and stores will be bound by a happens-before relationship to atomic operators
in the same thread of execution, which themselves synchronize-with atomics in
other threads. Following these rules, regular load/store operators can still be
elided, duplicated, and split up. This guarantees that data-race free code
executes as if it were sequentially consistent. Even when there are data races,
WebAssembly will ensure that the [nondeterminism](Nondeterminism.md) remains
limited and local.

Modules can have thread-local variables that are disjoint from linear memory
and can thus be represented efficiently by the engine.

  [synchronic]: http://wg21.link/n4195
  [C++11 memory model]: http://www.hboehm.info/c++mm/
  [PNaCl atomic support]: https://developer.chrome.com/native-client/reference/pnacl-c-cpp-language-support#memory-model-and-atomics
  [SharedArrayBuffer]: https://github.com/tc39/ecmascript_sharedmem

## Fixed-width SIMD

Support fixed-width SIMD vectors, initially only for 128-bit wide vectors as
demonstrated in [PNaCl's SIMD][] and [SIMD.js][].

SIMD adds new local types (e.g., `f32x4`) so it has to be part of the core
semantics. SIMD operators (e.g., `f32x4.add`) could be either builtin
operators (no different from `i32.add`) or exports of a builtin SIMD module.

  [PNaCl's SIMD]: https://developer.chrome.com/native-client/reference/pnacl-c-cpp-language-support#portable-simd-vectors
  [SIMD.js]: https://github.com/tc39/ecmascript_simd

## Zero-cost Exception Handling

The WebAssembly MVP (compilers and polyfills) may support four no-exception
modes for C++:
* Compiler transforms `throw` to `abort()`.
* Compiler-enforced `-fno-exceptions` mode (note [caveats][]).
* Compiler conversion of exceptions to branching at all callsites.
* In a Web environment exception handling can be emulated using JavaScript
  exception handling, which can provide correct semantics but isn't fast.

These modes are suboptimal for code bases which rely on C++ exception handling,
but are perfectly acceptable for C code, or for C++ code which avoids
exceptions. This doesn't prevent developers from using the C++ standard library:
their code will function correctly (albeit slower at times) as long as it
doesn't encounter exceptional cases.

Post-MVP, WebAssembly will gain support for developer access to stack unwinding,
inspection, and limited manipulation. These are critical to supporting zero-cost
exception handling by exposing [low-level capabilities][].

In turn, stack unwinding, inspection, and limited manipulation will be used to
implement `setjmp`/`longjmp`. This can enable all of the defined behavior of
`setjmp`/`longjmp`, namely unwinding the stack without calling C++
destructors. It does not, however, allow the undefined behavior case of jumping
forward to a stack that was already unwound which is sometimes used to implement
coroutines. Coroutine support is being
[considered separately](FutureFeatures.md#coroutines).

  [caveats]: https://blog.mozilla.org/nnethercote/2011/01/18/the-dangers-of-fno-exceptions
  [low-level capabilities]: https://extensiblewebmanifesto.org

## Feature Testing

Post-MVP, some form of feature-testing will be required. We don't yet have the
experience writing polyfills to know whether `has_feature` is the right
primitive building block so we're not defining it (or something else) until we
gain this experience. In the interim, it's possible to do a crude feature test
(as people do in JavaScript) by just `eval`-ing WebAssembly code and catching
validation errors.

See [Feature test](FeatureTest.md) for a more detailed sketch.
