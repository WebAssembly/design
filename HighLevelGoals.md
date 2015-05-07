# WebAssembly High-Level Goals

1. Define a portable, size- and load-time-efficient binary format to serve as a
   web compilation target which can be compiled to execute at native speed by
   taking advantage of common hardware capabilities.
2. Define a human-editable text format that is isomorphic and convertible
   to/from the binary format.
3. Design to serve as a compilation target for a growing set of source languages
   over time, starting with C/C++.
4. Design to maintain the versionless, feature-testing and 
   [backwards-compatible](BinaryEncoding.md#backwards-compatibility) evolution story of the web; 
   engines should not need multiple, versioned decoders.
5. Design and implement incrementally:
    * [v.1](V1.md) of the standard will be a Minimum Viable Product with a [polyfill][] via client-side compilation to [asm.js][] and thus run on existing browsers.
      In semantic corner cases (such as out-of-bounds and alignment, which evoke Undefined Behavior in C and C++), where asm.js does not have ideal semantics,
      the default polyfill would diverge for performance reasons (it would also be possible to run a more precise but slower polyfill, but almost all
      real-world applications are not expected to need to do so).
    * subsequent versions will add more [features](EssentialPostV1Features.md) prioritized by feedback. Some of those features might not be polyfillable
      to asm.js in a practical way.
6. Design to execute within and integrate well with the existing web platform:
    * execute in the same semantic universe as JavaScript;
    * allow synchronous calls to and from JavaScript;
    * enforce the same-origin and permissions security policies; and
    * access browser functionality through the same Web APIs that are accessible
      to JavaScript.
7. Design to allow execution in non-browser environments.
8. Create a format which enables great [tooling](Tooling.md) people want to use.

  [polyfill]: https://github.com/WebAssembly/polyfill
  [asm.js]: http://asmjs.org
