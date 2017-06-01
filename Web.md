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
In non-web embeddings, these APIs may not be present.

### Additional Web Embedding API

#### `WebAssembly.compileStreaming`

:cyclone: Added for milestone 2, developers must feature detect.

In Web embeddings, the following methods are added. 

Note that it is expected that `compileStreaming` and `instantiateStreaming` be either both present or both absent.

```
Promise<WebAssembly.Module> compileStreaming(source)
```

`source` is unconditionally passed through the built-in value
of `Promise.resolve`.
If the result is not a `Response` object, then the returned `Promise` is
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise)
with a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror).
This allows developers to pass either a promise that resolves
to a
[`Response`](https://fetch.spec.whatwg.org/#response-class)
object or a
[`Response`](https://fetch.spec.whatwg.org/#response-class)
object (which is automatically cast to a
promise) for the `source`.
Renderer-side
security checks about tainting for cross-origin content are tied to the types
of filtered responses defined in
[`Fetch`](https://fetch.spec.whatwg.org/#concept-fetch).

This function starts an asynchronous task to compile a `WebAssembly.Module`
as described in the [`WebAssembly.Module` constructor](#webassemblymodule-constructor).
On success, the `Promise` is [fulfilled](http://tc39.github.io/ecma262/#sec-fulfillpromise)
with the resulting `WebAssembly.Module` object. On failure, the `Promise` is
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise) with a
`WebAssembly.CompileError` or `TypeError`, depending on the type of failure.

The resolved `Response` is used as the source of the bytes to compile.
MIME type information is
[`extracted`](https://fetch.spec.whatwg.org/#concept-header-extract-mime-type)
from the `Response`, WebAssembly `source` data must have a MIME type of `application/wasm`,
extra parameters are not allowed (including empty `application/wasm;`).
A MIME type mismatch, a response whose
[type](https://fetch.spec.whatwg.org/#concept-response-type) is not "basic", "cors", or
"default", or a response whose status is not an
[ok status](https://fetch.spec.whatwg.org/#ok-status), must cause the Promise to be
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise) with a
`TypeError`.

#### `WebAssembly.instantiateStreaming`

:cyclone: Added for milestone 2, developers must feature detect.

In Web embeddings, the following methods are added.

```
dictionary WebAssemblyInstantiatedSource {
   required WebAssembly.Module module;
   required WebAssembly.Instance instance;
};

Promise<InstantiatedSource> instantiateStreaming(source [, importObject])
```

`source` is unconditionally passed through the built-in value
of `Promise.resolve`.
If the result is not a `Response` object, then the returned `Promise` is
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise)
with a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror).
This allows developers to pass either a promise that resolves
to a
[`Response`](https://fetch.spec.whatwg.org/#response-class)
object or a
[`Response`](https://fetch.spec.whatwg.org/#response-class)
object (which is automatically cast to a
promise) for the `source`.
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
`TypeError`, `WebAssembly.CompileError`, `WebAssembly.LinkError`, or `WebAssembly.RuntimeError`,
depending on the cause of failure.

The resolved `Response` is used as the source of the bytes to compile.
MIME type information is
[`extracted`](https://fetch.spec.whatwg.org/#concept-header-extract-mime-type)
from the `Response`, WebAssembly `source` data must have a MIME type of `application/wasm`,
extra parameters are not allowed (including empty `application/wasm;`).
A MIME type mismatch, a response whose
[type](https://fetch.spec.whatwg.org/#concept-response-type) is not "basic", "cors", or
"default", or a response whose status is not an
[ok status](https://fetch.spec.whatwg.org/#ok-status), must cause the Promise to be
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise) with a
`TypeError`.

## Developer-facing display conventions

Browsers, JavaScript engines, and offline tools have common ways of referring to
JavaScript artifacts and language constructs. For example, locations in
JavaScript source code are printed in stack traces or error messages, and are
represented naturally as decimal-format lines and columns in text files. Names
of functions and variables are taken directly from the sources. Therefore (for
example) even though the exact format of Error.stack strings does not always
match, the locations are easily understandable and the same across browsers.

To achive the same goal of a common representations for WebAssembly constructs, the
following conventions are adopted.

A WebAssembly location is a reference to a particular instruction in the binary, and may be
displayed by a browser or engine in similar contexts as JavaScript source locations.
It has the following format:

`${url}:wasm-function[${funcIndex}]:${pcOffset}`

Where
* `${url}` is the URL associated with the module, if applicable (see notes).
* `${funcIndex}` is an index in the [function index space](Modules.md#function-index-space).
* `${pcOffset}` is the offset in the module binary of the first byte
  of the instruction, printed in hexadecimal with lower-case digits,
  with a leading `0x` prefix.

Notes:
* The URL field may be interpreted differently depending on the
context. When the response-based
instantiation [API](#additional-web-embedding-api) is used in a
browser, the associated URL should be used; or when the
ArrayBuffer-based instantiation
[API](JS.md#webassemblyinstantiate) is used, the browser should represent
the location of the API call. This kind of instantiation is analagous to
executing JavaScript using `eval`; therefore if the browser has an existing
method to represent the location of the `eval` call it can use a similar
one for `WebAssembly.instantiate`. For example if the browser uses
`foo.js line 10 > eval` or `eval at bar (foo.js:10:3)` for `eval`, it could
use `foo.js line 10 > WebAssembly.instantiate` or
`WebAssembly.instantiate at bar (foo.js:10:3)`, respectively.
Offline tools may use a filename instead.
* Using hexadecimal for module offsets matches common conventions in native tools
such as objdump (where addresses are printed in hex) and makes them visually
distinct from JavaScript line numbers. Other numbers are represented in decimal.

While the `name` property of [exported WebAssembly functions](JS.md#exported-function-exotic-objects)
is specified by the JS API, synthesized function names are also
displayed in other contexts like devtool callstacks and `Error.stack`.
If a WebAssembly module contains a ["name" section](BinaryEncoding.md#name-section),
these names should be used to synthesize a function name as follows:
* If a function name subsection is present, the displayed name should
  be `${module_name}.${function_name}` or `${function_name}`, depending
  on whether the module name is present.
* Otherwise, the output can be context-dependent:
  * If the function name is shown alongside its location in a
    stack trace, then just the module name (if present) or an empty string
    can be used (because the function index is already in the location).
  * Otherwise, `${module_name}.wasm-function[${funcIndex}]` or 
    `wasm-function[${funcIndex}]` should be used to convey the function index.

Note that this document does not specify the full format of strings such as
stack frame representations; this allows engines to continue using their
existing formats for JavaScript (which existing code may already be depending
on) while still printing WebAssembly frames in a format consistent with
JavaScript.

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
