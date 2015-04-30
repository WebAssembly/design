# WebAssembly High-Level Goals

1. Define a portable, size- and load-time-efficient binary format to serve as a
   web compilation target which can be compiled to execute at native speed and
   take full advantage of a CPU's capabilities.
2. Define a human-editable text format that is isomorphic and convertible
   to/from the binary format.
3. Design to serve as a compilation target for a growing set of source languages
   over time, starting with C/C++.
4. Design to maintain the versionless, feature-testing and backwards-compatible
   evolution story of the web; engines should not need multiple, versioned
   decoders.
5. Design and implement incrementally: the initial version will be a Minimum
   Viable Product by being an effective [polyfill][] via client-side compilation
   to [asm.js][], with a feature set similar to [asm.js][]'s current
   features. Subsequent versions will be more [featureful][].
6. Design to integrate well with the existing web platform:
    * execute in the same semantic universe as JavaScript;
    * allow synchronous calls to and from JavaScript;
    * enforce the same-origin and permissions security policies; and
    * access browser functionality through the same Web APIs that are accessible
      to JavaScript.
7. Design to allow execution in non-browser environments.

  [polyfill]: https://github.com/WebAssembly/polyfill
  [asm.js]: http://asmjs.org
  [featureful]: https://github.com/WebAssembly/spec/blob/master/UpcomingFeatures.md
