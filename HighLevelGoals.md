# WebAssembly High-Level Goals

1. Define a [portable](Portability.md), size- and load-time-efficient
   [binary format](MVP.md#binary-format) to serve as a compilation target which
   can be compiled to execute at native speed by taking advantage of common
   hardware capabilities available on a wide range of platforms, including
   [mobile](http://en.wikipedia.org/wiki/Mobile_device) and
   [IoT](http://en.wikipedia.org/wiki/Internet_of_Things).
2. Specify and implement incrementally:
    * A [Minimum Viable Product (MVP)](MVP.md) for the standard with
      roughly the same functionality as [asm.js](http://asmjs.org);
    * An effective and efficient [polyfill](Polyfill.md) library for the
      MVP that translates WebAssembly code into JavaScript in the client so that
      WebAssembly MVP can run on existing browsers;
    * A [follow-up to the MVP](PostMVP.md) which adds several more
      essential features; and
    * [Additional features](FutureFeatures.md), specified iteratively and
      prioritized by feedback and experience, including support for languages
      other than C/C++.
3. Design to execute within and integrate well with the *existing*
   [Web platform](Web.md):
    * Maintain the versionless, [feature-tested](FeatureTest.md) and
      [backwards-compatible](BinaryEncoding.md#backwards-compatibility)
      evolution story of the Web;
    * Execute in the same semantic universe as JavaScript;
    * Allow synchronous calls to and from JavaScript;
    * Enforce the same-origin and permissions security policies;
    * Access browser functionality through the same Web APIs that are accessible
      to JavaScript; and
    * Define a human-editable text format that is convertible to and from the
      binary format, supporting View Source functionality.
4. Design to support [non-browser embeddings](NonWeb.md) as well.
5. Make a great platform:
    * Build an [LLVM backend](https://github.com/WebAssembly/llvm) and
      accompanying [clang port](https://github.com/WebAssembly/clang);
    * Promote other compilers and tools targeting WebAssembly; and
    * Enable other useful [tooling](Tooling.md).
