# WebAssembly High-Level Goals

1. Define a portable, size- and load-time-efficient binary format to serve as a
   web compilation target.
2. Expose a format which can be compiled to execute at native speed and take
   full advantage of a CPU's capabilities.
3. Define a human-editable text format that is isomorphic and convertible
   to/from the binary format.
4. Support compiling various programming languages to the WebAssembly format.
5. Design to maintain the versionless, feature-testing and backwards-compatible
   evolution story of the web; engines should not need multiple, versioned
   decoders.
6. Design and implement incrementally:
    * v.1 will allow an effective (load-time and performance) [polyfill][] via
      client-side compilation to [asm.js][]. It will be a Minimum Viable
      Product: basically what you can do with [asm.js][].
	* Subsequent versions will expose further features, prioritized based on
      developer feedback, such as:
	  - Concurrency and parallelism through threads and SIMD.
	  - Dynamic loading.
	  - Finer-grained control on memory managment.
	  - Zero-cost exception handling.
	  - Garbage collection.
7. Design to integrate well with the existing web platform:
    * execute in the same semantic universe as JavaScript;
    * allow synchronous calls to and from JavaScript;
    * enforce the same-origin and permissions security policies; and
    * access browser functionality through the same Web APIs that are accessible
      to JavaScript.
8. Design to allow execution in non-browser environments.

  [polyfill]: https://github.com/WebAssembly/polyfill
  [asm.js]: http://asmjs.org
