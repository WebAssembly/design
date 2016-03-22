# Web Embedding

Unsurprisingly, one of WebAssembly's primary purposes is to run on the Web,
for example embedded in Web browsers (though this is
[not its only purpose](NonWeb.md)).

This means integrating with the Web ecosystem, leveraging Web APIs, supporting
the Web's security model, preserving the Web's portability, and designing in
room for evolutionary development. Many of these goals are clearly
reflected in WebAssembly's [high-level goals](HighLevelGoals.md). In
particular, WebAssembly MVP will be no looser from a security point of view
than if the module was JavaScript.

More concretely, the following is a list of points of contact between WebAssembly
and the rest of the Web platform that have been considered:

## The `Wasm` object

In addition to
[integration with the ES6 Modules system](Modules.md#integration-with-es6-modules),
WebAssembly is exposed to the Web through a `Wasm` object whose API is designed
to provide a powerful, idiomatic set of methods and properties to instantiate
and introspect WebAssembly modules directly from JavaScript.

## Modules

WebAssembly's [modules](Modules.md) allow for natural [integration with
the ES6 module system](Modules.md#integration-with-es6-modules) and allow
synchronous calling to and from JavaScript.

## Aliasing linear memory from JS

If [allowed by the module](Modules.md#linear-memory-section), JavaScript can
alias a loaded module's linear memory through an exported `ArrayBuffer`.
Module instances would additionally expose methods to JS to copy ranges of
bytes into and out of linear memory as separate (unaliased) `ArrayBuffer`s.

Since JS semantics and implementations require the `byteLength` of an
`ArrayBuffer` to be constant, [resizing](AstSemantics.md#resizing) the
linear memory cannot simply resize the exported `ArrayBuffer`. Instead,
the `ArrayBuffer` would be [detached](http://people.mozilla.org/~jorendorff/es6-draft.html#sec-detacharraybuffer)
and a new `ArrayBuffer` (with a new `byteLength`) would be exported in
its place.

When [threads](PostMVP.md#threads) are added, a
[`SharedArrayBuffer`](https://github.com/lars-t-hansen/ecmascript_sharedmem)
would need to be exported instead of an `ArrayBuffer`. However, the
detach-on-resize strategy would pose significant usability and implementation 
hazards, since resizing can happen concurrently. One solution would be
to simply not export a `SharedArrayBuffer` when a module declared use of
threads and resizable memory (the copy in/out methods would need to be used
instead).

Similarly, various [linear memory operations](FutureFeatures.md#finer-grained-control-over-memory)
like `mprotect` conflict with the JS semantics of `ArrayBuffer` and
would inhibit export. In general, `ArrayBuffer` could be viewed as an
optimization of copy in/out that was only available when linear memory
behaved like an `ArrayBuffer` (or `SharedArrayBuffer`).

## 64-bit integers
Currently Javascript doesn't support 64-bit integers so they will be disallowed
as a return value in JS binding. Once JS has [Typed Objects](http://wiki.ecmascript.org/doku.php?id=harmony:typed_objects)
i64 values can be communicated to browser environment.

## Security

WebAssembly's security model should depend on [CORS][] and
[subresource integrity][] to enable distribution, especially through content
distribution networks and to implement
[dynamic linking](DynamicLinking.md).

## Threads

Once [threads are supported](PostMVP.md#threads), a WebAssembly module would
be shared (including its heap) between workers via `postMessage()`.
* This also has the effect of explicitly sharing code so that engines don't
perform N fetches and compile N copies.
* WebAssembly may later standardize a more direct way to create a thread that
doesn't involve creating a new Worker.

## SIMD

Once [SIMD is supported](PostMVP.md#fixed-width-simd) WebAssembly would:
* Be statically typed analogous to [SIMD.js-in-asm.js][];
* Reuse specification of operation semantics (with TC39);
* Reuse backend implementation (same IR nodes).

## GC

Once [GC is supported](GC.md), WebAssembly code would be able to reference
and access JavaScript, DOM, and general WebIDL-defined objects.

  [CORS]: https://www.w3.org/TR/cors/
  [subresource integrity]: https://www.w3.org/TR/SRI/
  [SIMD.js-in-asm.js]: http://discourse.specifiction.org/t/request-for-comments-simd-js-in-asm-js
