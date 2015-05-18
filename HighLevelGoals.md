# WebAssembly High-Level Goals

1. Define a portable, size- and load-time-efficient binary format to serve as a
   compilation target which can be compiled to execute at native speed by taking
   advantage of common hardware capabilities.
2. Define a human-editable text format that is convertible to and from the binary
   format.
3. Specify and implement incrementally:
    * Design [v.1](V1.md) of the standard as a Minimum Viable Product with roughly
      the same functionality as [asm.js](http://asmjs.org).
    * Plan to iteratively specify [additional](EssentialPostV1Features.md)
      [features](FutureFeatures.md) after v.1.
    * Plan to iteratively improve support for compilation from languages other
      than C/C++ (e.g., languages with [GC](FutureFeatures.md#gcdom-integration)).
4. Ship an effective [polyfill](V1.md#polyfill) library for v.1 that translates
   WebAssembly code into asm.js in the client so that WebAssembly can run on
   existing browsers at high speeds.
5. Design to execute within and integrate well with the *existing* Web platform:
    * maintain the versionless, feature-tested and 
      [backwards-compatible](BinaryEncoding.md#backwards-compatibility) evolution
      story of the Web;
    * execute in the same semantic universe as JavaScript;
    * allow synchronous calls to and from JavaScript;
    * enforce the same-origin and permissions security policies; and
    * access browser functionality through the same Web APIs that are accessible
      to JavaScript.
6. Design to allow execution in non-browser environments.
7. Enable great [tooling](Tooling.md).
