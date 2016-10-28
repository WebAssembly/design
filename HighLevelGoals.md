# WebAssembly High-Level Goals

1. Define a [portable](Portability.md), size- and load-time-efficient
   [binary format](MVP.md#binary-format) to serve as a compilation target which
   can be compiled to execute at native speed by taking advantage of common
   hardware capabilities available on a wide range of platforms, including
   [mobile](https://en.wikipedia.org/wiki/Mobile_device) and
   [IoT](https://en.wikipedia.org/wiki/Internet_of_Things).
2. Specify and implement incrementally:
    * a [Minimum Viable Product (MVP)](MVP.md) for the standard with
      roughly the same functionality as [asm.js](http://asmjs.org), primarily
      aimed at [C/C++](CAndC++.md);
    * a [follow-up to the MVP](PostMVP.md) which adds several more
      essential features; and
    * [additional features :unicorn:][future features], specified iteratively and
      prioritized by feedback and experience, including support for languages
      other than C/C++.
3. Design to execute within and integrate well with the *existing*
   [Web platform](Web.md):
    * maintain the versionless, [feature-tested](FeatureTest.md) and
      [backwards-compatible](BinaryEncoding.md#backwards-compatibility)
      evolution story of the Web;
    * execute in the same semantic universe as JavaScript;
    * allow synchronous calls to and from JavaScript;
    * enforce the same-origin and permissions security policies;
    * access browser functionality through the same Web APIs that are accessible
      to JavaScript; and
    * define a human-editable text format that is convertible to and from the
      binary format, supporting View Source functionality.
4. Design to support [non-browser embeddings](NonWeb.md) as well.
5. Make a great platform:
    * build a new LLVM backend for WebAssembly and an accompanying
      clang port ([why LLVM first?](FAQ.md#which-compilers-can-i-use-to-build-webassembly-programs));
    * promote other compilers and tools targeting WebAssembly; and
    * enable other useful [tooling](Tooling.md).

[future features]: FutureFeatures.md
