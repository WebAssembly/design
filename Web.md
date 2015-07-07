# Browser Embedding

Unsurprisingly, one of WebAssembly's primary purposes is to run on the Web,
embedded in Web browsers (though this is [not its only purpose](NonWeb.md)).

This means integrating with the Web ecosystem, leveraging Web APIs, supporting
the Web's security model, preserving the Web's portability, and designing in
room for evolutionary development. Many of these goals are clearly
reflected in WebAssembly's [high-level goals](HighLevelGoals.md).

# Implementation Details

We've identified interesting implementation approaches which help convince us
that the design, especially that of the [MVP](MVP.md), are sensible:

* WebAssembly's security model should depend on [CORS][] and
  [subresource integrity][] to enable distribution, especially through content
  distribution networks and to implement
  [dynamic linking](FutureFeatures.md#dynamic-linking).
* A [module](MVP.md#modules) can be loaded in the same way as an ES6 module
  (`import` statements, `Reflect` API, `Worker` constructor, etc) and the result
  is reflected to JS as an ES6 module object.
  - Exports are the ES6 module object exports.
  - An import first passes the module name to the [module loader pipeline][] and
    resulting ES6 module (which could be implemented in JS or WebAssembly) is
    queried for the export name.
  - There is no special case for when one WebAssembly module imports another:
    they have separate [memory](MVP.md#linear-memory) and pointers cannot be passed
    between the two. Module imports encapsulate the importer and
    importee. [Dynamic linking](FutureFeatures.md#dynamic-linking) should be
    used to share memory and pointers across modules.
  - To synchronously call into JavaScript from C++, the C++ code would declare
    and call an undefined `extern` function and the target JavaScript function
    would be given the (mangled) name of the `extern` and put inside the
    imported ES6 module.
* Once [threads are supported](PostMVP.md#threads), a WebAssembly module would
  initially be distributed between workers via `postMessage()`.
  - This also has the effect of explicitly sharing code so that engines don't
    perform N fetches and compile N copies.
  - May later standardize a more direct way to create a thread from WebAssembly.
* Once [SIMD is supported](PostMVP.md#fixed-width-simd), a Web implementation of
  WebAssembly would:
  - Be statically typed analogous to [SIMD.js-in-asm.js][];
  - Reuse specification of operation semantics (with TC39);
  - Reuse backend implementation (same IR nodes).

  [CORS]: https://www.w3.org/TR/cors/
  [subresource integrity]: https://www.w3.org/TR/SRI/
  [module loader pipeline]: https://whatwg.github.io/loader
  [SIMD.js-in-asm.js]: http://discourse.specifiction.org/t/request-for-comments-simd-js-in-asm-js
  
