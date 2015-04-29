# WebAssembly High-Level Design Goals

1. Define a portable, size- and load-time-efficient binary format to serve as a web compilation target.
2. Define a human-editable text format that is isomorphic and convertible to/from the binary format.
3. Design v.1 to allow an effective (load-time and performance) [polyfill](https://github.com/WebAssembly/polyfill) via client-side compilation to [asm.js](http://asmjs.org).
4. Design v.1 as a Minimum Viable Product: basically what you can do with [asm.js](http://asmjs.org).
5. Design to integrate well with the existing web platform:
    * execute in the same semantic universe as JavaScript;
    * allow synchronous calls to and from JavaScript;
    * enforce the same-origin security policy; and 
    * access browser functionality through the same Web APIs that are accessible to JavaScript.
6. Design to maintain the versionless, feature-testing and backwards-compatible evolution story 
   of the web; engines should not need multiple, versioned decoders.
7. Design to allow execution in non-browser environments.


