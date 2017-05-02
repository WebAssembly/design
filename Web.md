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

## JavaScript API

A [JavaScript API](JS.md) is provided which allows JavaScript to compile
WebAssembly modules, perform limited reflection on compiled modules, store
and retrieve compiled modules from offline storage, instantiate compiled modules
with JavaScript imports, call the exported functions of instantiated modules,
alias the exported memory of instantiated modules, etc.

The Web embedding includes additional methods useful in that context.
Non-web embeddings are not required to support these additional methods.

### Additional Web Embedding API

#### `WebAssembly.compile`

:cyclone: Added for milestone 2, developers must feature detect.

In Web embeddings, the following overloads are added (in addition to the core
JS API method of the same name).

```
Promise<WebAssembly.Module> compile(Response source)

Promise<WebAssembly.Module> compile(Promise<Response> source)
```

Developers can set the argument `source` with either a promise that resolves
with a
[`Response`](https://fetch.spec.whatwg.org/#response-class)
object or a
[`Response`](https://fetch.spec.whatwg.org/#response-class)
object (which is automatically cast to a
promise).
If when unwrapped that `Promise` is not a `Response` object, then the returned `Promise` is
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise)
with a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror).
Renderer-side
security checks about tainting for cross-origin content are tied to the types
of filtered responses defined in
[`Fetch`](https://fetch.spec.whatwg.org/#concept-fetch).

This function starts an asynchronous task to compile a `WebAssembly.Module`
as described in the [`WebAssembly.Module` constructor](#webassemblymodule-constructor).
On success, the `Promise` is [fulfilled](http://tc39.github.io/ecma262/#sec-fulfillpromise)
with the resulting `WebAssembly.Module` object. On failure, the `Promise` is
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise) with a
`WebAssembly.CompileError`.

The `Promise<Response>` is used as the source of the bytes to compile.
MIME type information is
[`extracted`](https://fetch.spec.whatwg.org/#concept-header-extract-mime-type)
from the `Response`, WebAssembly `source` data must have a MIME type of `application/wasm`,
extra parameters are not allowed (including empty `application/wasm;`).
MIME type mismatch or `opaque` response types
[reject](http://tc39.github.io/ecma262/#sec-rejectpromise) the Promise with a
`WebAssembly.CompileError`.

#### `WebAssembly.instantiate`

:cyclone: Added for milestone 2, developers must feature detect.

In Web embeddings, the following overloads are added (in addition to the core
JS API method of the same name).

```
Promise<{module:WebAssembly.Module, instance:WebAssembly.Instance}>
  instantiate(Response source [, importObject])

Promise<{module:WebAssembly.Module, instance:WebAssembly.Instance}>
  instantiate(Promise<Response> source [, importObject])
```

Developers can set the argument `source` with either a promise that resolves
with a
[`Response`](https://fetch.spec.whatwg.org/#response-class)
object or a
[`Response`](https://fetch.spec.whatwg.org/#response-class)
object (which is automatically cast to a
promise).
If when unwrapped that `Promise` is not a `Response` object, then the returned `Promise` is
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise)
with a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror).
Renderer-side
security checks about tainting for cross-origin content are tied to the types
of filtered responses defined in
[`Fetch`](https://fetch.spec.whatwg.org/#concept-fetch).

This function starts an asynchronous task that first compiles a `WebAssembly.Module`
based on bytes from `source` as described in
the [`WebAssembly.Module` constructor](#webassemblymodule-constructor)
and then instantiate the resulting `Module` with `importObject` as described in the
[`WebAssembly.Instance` constructor](#webassemblyinstance-constructor).
On success, the `Promise` is [fulfilled](http://tc39.github.io/ecma262/#sec-fulfillpromise)
with a plain JavaScript object pair `{module, instance}` containing the resulting
`WebAssembly.Module` and `WebAssembly.Instance`. The 2 properties `module` and `instance` of the returned pair are  configurable, enumerable and writable.

On failure, the `Promise` is
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise) with a
`WebAssembly.CompileError`, `WebAssembly.LinkError`, or `WebAssembly.RuntimeError`, depending on the cause of failure.

The `Promise<Response>` is used as the source of the bytes to compile.
MIME type information is
[`extracted`](https://fetch.spec.whatwg.org/#concept-header-extract-mime-type)
from the `Response`, WebAssembly `source` data must have a MIME type of `application/wasm`,
extra parameters are not allowed (including empty `application/wasm;`).
MIME type mismatch or `opaque` response types
[reject](http://tc39.github.io/ecma262/#sec-rejectpromise) the Promise with a
`WebAssembly.CompileError`.

## Modules

WebAssembly's [modules](Modules.md) allow for natural [integration with
the ES6 module system](Modules.md#integration-with-es6-modules).

### Names

A WebAssembly module can have imports and exports, which are identified using
UTF-8 byte sequences. The most natural Web representation of a mapping of export
names to exports is a JS object in which each export is a property with a name
encoded in UTF-16. A WebAssembly module fails validation on the Web if it has
imports or exports whose names do not transcode cleanly to UTF-16 according to
the following conversion algorithm, assuming that the WebAssembly name is in a
`Uint8Array` called `array`:

```
function convertToJSString(array)
{
  var string = "";
  for (var i = 0; i < array.length; ++i)
    string += String.fromCharCode(array[i]);
  return decodeURIComponent(escape(string));
}
```

This performs the UTF8 decoding (`decodeURIComponent(escape(string))`) using
a [common JS idiom](http://monsur.hossa.in/2012/07/20/utf-8-in-javascript.html).
Transcoding failure is detected by `decodeURIComponent`, which may throw
`URIError`. If it does, the WebAssembly module will not validate. This validation
rule is only mandatory for Web embedding.

## Source Maps for WebAssembly files on the web

### Background
[Source maps](https://docs.google.com/document/d/1U1RGAehQwRypUTovF1KRlpiOFze0b-_2gc6fAH0KY0k)
are a debug-information format used on the web, generated by toolchains that target
JavaScript, either from other languages (e.g. TypeScript, Emscripten) or from
JavaScript itself (e.g. Closure, minifiers). A browser that supports source maps
can display the original source in its developer tools instead of the compiled
JavaScript that the VM actually executes. It can support setting breakpoints
specified as source lines, single-stepping through source lines, and showing
source lines in stack traces; however it cannot support getting or setting
variables from the original source code because the format is not "full" debug
info: it only maps locations from the compiled code back to locations in the
source files (where a generated-code location consists of a line and column
number, and a source location consists of file, line, and column). Generated
files use a specially-formatted comment to specify a URL for a source map that
describes the file.

Source maps can be made to serve the same purpose for WebAssembly binary files,
allowing parity with asm.js. By re-using an existing standard, browsers can
take advantage of their existing source map support without having to make
significant changes to their developer tools.


### Alternatives

An alternative method is to specify location mapping information in some way
other than source maps. For example, it could be encoded directly into the wasm
binary the way function and local names are written in the "name" section. This
is more convenient if the VM itself uses the information (for example to
generate a stack trace). However existing developer tools implementations are
designed around the source map model of passing a URL from the VM to the dev
tools code, which fetches and interprets the information. Therefore it is
epxected that it will be much simpler to use "real" source maps than to design
another format.

Source maps also have known limitations (e.g. source variables cannot be
described). A complete debug info format will eventually be needed; however the
intention of this specification is to allow for easy integration with existing
developer tools implementations in browsers.


### Spec

With few exceptions, most aspects of the source map
[spec](https://docs.google.com/document/d/1U1RGAehQwRypUTovF1KRlpiOFze0b-_2gc6fAH0KY0k/)
may be applied directly to wasm.
Because wasm is a binary format (and thus does not have lines and columns, or
comments per se), some simple reinterpretation of concepts is needed.

#### Locations

Source maps specify how to map a (zero-based) line and column position in
generated code to a file, line, and column location in the source. For
wasm binary locations, the line number is always 0, and the column number
is interpreted as a byte offset into the wasm binary content (this results
in a more efficient encoding than using the line number instead).
Source locations are interpreted as in the source map spec.

#### Linking generated code to source maps

When the generated code is JavaScript, it includes a specially-formatted line
at the end, which is the URL of the associated source map. For wasm, a custom
section named `"sourceMappingURL"` contains the URL.
As with source maps,
the URL is defined as in [RFC3986](https://tools.ietf.org/html/rfc3986) (e.g.
it must be percent-encoded if necessary) and it may be a data URI. The URL
is also resolved according to the source map spec, and may also be specified
with the `SourceMap:` HTTP header.



## Security

WebAssembly's [security](Security.md) model should depend on the
[same-origin policy][], with [cross-origin resource sharing (CORS)][] and
[subresource integrity][] to enable distribution through content
distribution networks and to implement [dynamic linking](DynamicLinking.md).

## SIMD

Once [SIMD is supported](FutureFeatures.md#fixed-width-simd) WebAssembly would:

* Be statically typed analogous to [SIMD.js-in-asm.js][];
* Reuse specification of operation semantics (with TC39);
* Reuse backend implementation (same IR nodes).

## GC

Once [GC is supported](GC.md), WebAssembly code would be able to reference
and access JavaScript, DOM, and general WebIDL-defined objects.

  [same-origin policy]: https://www.w3.org/Security/wiki/Same_Origin_Policy
  [cross-origin resource sharing (CORS)]: https://www.w3.org/TR/cors/
  [subresource integrity]: https://www.w3.org/TR/SRI/
  [SIMD.js-in-asm.js]: http://discourse.specifiction.org/t/request-for-comments-simd-js-in-asm-js
