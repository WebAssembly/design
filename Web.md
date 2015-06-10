# Browser Embedding

WebAssembly's [Minimum Viable Product](MVP.md) explicitly targets browser
embeddings, and treats WebAssembly as an integral part of the Web platform.

A key part of operating on the web is supporting
[feature testing](FeatureTest.md).

# Implementation Details

We've identified interesting implementation approaches which help convince us
that the design, especially that of the [MVP](MVP.md), are sensible:

* WebAssembly's security model should depend on [CORS][] and
  [subresource integrity][] to enable distribution, especially though content
  distribution networks and to implement
  [dynamic linking](FutureFeatures.md#dynamic-linking).
* A [module](MVP.md#Modules) can be loaded in the same way as an ES6 module
  (`import` statements, `Reflect` API, `Worker` constructor, etc) and the result
  is reflected to JS as an ES6 module object.
  * Exports are the ES6 module object exports.
  * An import first passes the module name to the [module loader pipeline][] and
    resulting ES6 module (which could be implemented in JS or WebAssembly) is
    queried for the export name.
  * There is no special case for when one WebAssembly module imports another:
    they have separate [heaps](MVP.md#heap) and pointers cannot be passed
    between the two. Module imports encapsulate the importer and
    importee. [Dynamic linking](FutureFeatures.md#dynamic-linking) should be
    used to share heaps and pointers across modules.
  * To synchronously call into JavaScript from C++, the C++ code would declare
    and call an undefined `extern` function and the target JavaScript function
    would be given the (mangled) name of the `extern` and put inside the
    imported ES6 module.

  [CORS]: http://www.w3.org/TR/cors/
  [subresource integrity]: http://www.w3.org/TR/SRI/
  [module loader pipeline]: http://whatwg.github.io/loader
