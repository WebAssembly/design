# JIT and Optimization Library

WebAssembly's [Just-in-Time compilation (JIT)][future jit control]
interface will likely be fairly low-level, exposing general-purpose primitives
rather than higher-level functionality. Still, there is a need for higher-level
functionality, and for greater flexibility than the WebAssembly spec can provide.
There is also a need for experimentation, particularly in the area of
applications wishing to dynamically generate new code, to determine which features
and interfaces are most appropriate. JIT and Optimization libraries that would run
inside WebAssembly and provide support and higher-level features would fit this
need very well.

Such libraries wouldn't be part of the WebAssembly spec itself, but the concept
is relevant to discuss here because features that we can expect to address in
libraries are features that we may not need to add to the spec. This strategy
can help keep the spec itself simple and reduce the surface area of features
required of every spec implementation.

And, libraries will facilitate light-weight experimentation with new features
that we may eventually want to add to WebAssembly itself. In a library layer,
we can quickly iterate, experiment, and gain real-world insight, before adding
features to the spec itself and freezing all the details. And as new features
are standardized, libraries will become the polyfills which will help those
features gain adoption.

This raises the question of how we should decide which features belong in the
spec, and which belong in a library. Some of the fundamental advantages of
putting functionality in a library rather than in the spec and in implementations
themselves include:

 * A library can freely choose to offer greater degrees of undefined behavior,
   implementation-defined behavior, unspecified behavior, and so on. This means
   it can perform much more aggressive optimizations, including many that are
   extremely common in optimizing compilers and might otherwise seem missing in
   the WebAssembly spec itself:
    * Constant folding, strength reduction, and code motion of math functions
      such as `sin`, `cos`, `exp`, `log`, `pow`, and `atan2`.
    * Performing aggressive expression simplifications that depend on assuming
      that integer arithmetic doesn't overflow.
    * Performing GVN with redundant load elimination, and other optimizations
      based on aliasing rules that incur undefined behavior if they are violated.
    * Vectorization that utilizes both floating point reassociation and
      awareness of the underlying platform through
      [feature testing](FeatureTest.md).

 * A library can support higher-level features, and features that are tailored
   to certain applications, whereas the WebAssembly spec itself is limited to
   general-purpose primitives. Possible examples of this are:
    * A richer type system, which could include things like complex, rational,
      arbitrary bitwidth integers, non-power-of-2 SIMD types, interval
      arithmetic, etc.
    * A higher-level type system, which could include basic polymorphism of
      various kinds (either with true dynamism or with monomorphisation).
    * Richer control flow constructs.
    * A broader set of operators, such as string-handling operators,
      data type serialization, testing facilities, and linear algebra
      operators, all of which can benefit from being integrated at the
      language level.
   Since every feature required in the spec itself will need to be implemented
   by all implementations, domain-specific features run the risk of making
   people "pay for what they don't use". With features libraries, people need
   only pay for the features they choose to use.

 * A library can evolve over time to meet the changing needs of higher-level
   languages. In practice, compiler IRs such as LLVM IR evolve to add new
   features, change existing features, and sometimes remove features, and these
   kinds of changes are much harder to do in a spec.

The library approach also means that applications using a particular version
of a library can get consistent behavior and performance, because of the
determinism of the underlying WebAssembly platform.

A significant range of approaches are possible:

  * "Customized WebAssembly". This might involve a library whose input format
    is conceptually WebAssembly but with some additional features. The library
    could optimize and then lower those features leaving standard WebAssembly
    to present to the underlying implementation.

  * "Bring Your Own Compiler" There's nothing stopping one from bundling
    full-fledged AOT-style compilers that compile an independent source language
    or IR into WebAssembly right there in WebAssembly itself. Obviously this
    will involve tradeoffs in terms of download size and startup time, but it
    would allow a unique degree of flexibility.

  * And many things in between.

[future jit control]: FutureFeatures.md#platform-independent-just-in-time-jit-compilation
