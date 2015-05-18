# WebAssembly High-Level Goals

1. Define a portable, size- and load-time-efficient [binary format](V1.md#binary-format)
   to serve as a compilation target which can be compiled to execute at native
   speed by taking advantage of common hardware capabilities.
2. Specify and implement incrementally:
    * design [v.1](V1.md) of the standard as a Minimum Viable Product with roughly
      the same functionality as [asm.js](http://asmjs.org);
    * ship an effective [polyfill](V1.md#polyfill) library for v.1 that translates
      WebAssembly code into asm.js in the client so that WebAssembly v.1 can run on
      existing browsers at high speeds;
    * ship a follow-up to v.1 which adds several more
      [essential features](EssentialPostV1Features.md); and
    * continue to iteratively specify [additional features](FutureFeatures.md),
      prioritized by feedback and experience, including support for languages other
      than C/C++.
3. Design to execute within and integrate well with the *existing* Web platform:
    * maintain the versionless, feature-tested and 
      [backwards-compatible](BinaryEncoding.md#backwards-compatibility) evolution
      story of the Web;
    * execute in the same semantic universe as JavaScript;
    * allow synchronous calls to and from JavaScript;
    * enforce the same-origin and permissions security policies;
    * access browser functionality through the same Web APIs that are accessible
      to JavaScript; and
    * define a human-editable text format that is convertible to and from the
      binary format, supporting View Source functionality.
4. Design to support (non-browser environments)[V1.md#non-browser-embedding]
   as well.
5. Make a great platform:
    * build an [LLVM backend](https://github.com/WebAssembly/llvm) and
      accompanying [clang port](https://github.com/WebAssembly/clang);
    * promote other compilers and tools targeting WebAssembly; and
    * enable other useful [tooling](Tooling.md).
