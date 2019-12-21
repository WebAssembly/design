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

In Web embeddings, the following methods are added.

Note that it is expected that `compileStreaming` and `instantiateStreaming` be either both present or both absent.

#### `WebAssembly.compileStreaming`

:cyclone: Added for milestone 2, developers must feature detect.

```
Promise<WebAssembly.Module> compileStreaming(source)
```

_This function accepts a [`Response`](https://fetch.spec.whatwg.org/#response-class) object, or a
promise for one, and compiles the resulting bytes of the response. This compilation can be performed
in the background and in a streaming manner. If the `Response` is not
[CORS-same-origin](https://html.spec.whatwg.org/multipage/urls-and-fetching.html#cors-same-origin),
does not represent an [ok status](https://fetch.spec.whatwg.org/#ok-status), or does not match the
`` `application/wasm` `` MIME type, the returned promise will be rejected with a `TypeError`; if
compilation fails, the returned promise will be rejected with a `WebAssembly.CompileError`._

1. Return the result of [processing a potential WebAssembly
   response](#process-a-potential-webassembly-response), given _source_, with the following steps:
   - Processing steps, given _bytes_:
      1. Compile _bytes_, in the same fashion as specified for the [`WebAssembly.Module`
         constructor](https://github.com/WebAssembly/design/blob/master/JS.md#webassemblymodule-constructor).
      1. If compilation completes successfully, return a success with the validated `Ast.module`
         compilation result.
      1. Otherwise, if compilation fails, return a failure with the failure reason.
   - Success steps, given _module_
      1. Return a new `WebAssembly.Module` instance, with its [[Module]] set to _module_.
   - Failure steps, given _reason_
      1. Return a new `WebAssembly.CompileError` containing the compilation failure information
         stored in _reason_.

#### `WebAssembly.instantiateStreaming`

:cyclone: Added for milestone 2, developers must feature detect.

```
dictionary WebAssemblyInstantiatedSource {
   required WebAssembly.Module module;
   required WebAssembly.Instance instance;
};

Promise<InstantiatedSource> instantiateStreaming(source [, importObject])
```

_This function accepts a [`Response`](https://fetch.spec.whatwg.org/#response-class) object, or a
promise for one, and compiles and instantiates the resulting bytes of the response. This compilation
can be performed in the background and in a streaming manner. If the `Response` is not
[CORS-same-origin](https://html.spec.whatwg.org/multipage/urls-and-fetching.html#cors-same-origin),
does not represent an [ok status](https://fetch.spec.whatwg.org/#ok-status), or does not match the
`` `application/wasm` `` MIME type, the returned promise will be rejected with a `TypeError`; if
compilation or instantiation fails, the returned promise will be rejected with a
`WebAssembly.CompileError`, `WebAssembly.LinkError`, or `WebAssembly.RuntimeError` depending on the
cause of failure._

1. Return the result of [processing a potential WebAssembly
   response](#process-a-potential-webassembly-response), given _source_, with the following steps:
   - Processing steps, given _bytes_:
      1. Compile _bytes_, in the same fashion as specified for the [`WebAssembly.Module`
         constructor](https://github.com/WebAssembly/design/blob/master/JS.md#webassemblymodule-constructor).
      1. If compilation completes successfully:
         1. Let _module_ be the `Ast.module` compilation result.
         1. Instantiate _module_ in the in the same fashion as specified for the
            [`WebAssembly.Instance`
            constructor](https://github.com/WebAssembly/design/blob/master/JS.md#webassemblyinstance-constructor).
         1. If instantiation completes successfully, return a success with a
            [tuple](https://infra.spec.whatwg.org/#tuple) containing _module_, the resulting
            instance, and the resulting exports.
         1. Otherwise, if instantiation fails, return a failure with the failure reason.
      1. Otherwise, if compilation fails, return a failure with the failure reason.
   - Success steps, given a tuple (_module_, _instance_, _exports_)
      1. Create _exportsObject_ from _exports_, in the same fashion as specified for the
         [`WebAssembly.Instance`
         constructor](https://github.com/WebAssembly/design/blob/master/JS.md#webassemblyinstance-constructor).
      1. Create _instanceObject_ from _exportsObject_ and _instance_, in the same fashion as specified for the
         [`WebAssembly.Instance`
         constructor](https://github.com/WebAssembly/design/blob/master/JS.md#webassemblyinstance-constructor).
      1. Let _moduleObject_ be a new `WebAssembly.Module` instance, with its [[Module]] set to _module_.
      1. Let _result_ be [ObjectCreate](https://tc39.github.io/ecma262/#sec-objectcreate)([%ObjectPrototype%](https://tc39.github.io/ecma262/#sec-properties-of-the-object-prototype-object)).
      1. Perform
         [CreateDataProperty](https://tc39.github.io/ecma262/#sec-createdataproperty)(_result_,
         "`module`", _moduleObject_).
      1. Perform
         [CreateDataProperty](https://tc39.github.io/ecma262/#sec-createdataproperty)(_result_,
         "`instance`", _instanceObject_).
      1. Return _result_.
   - Failure steps, given _reason_
      1. Return a new `WebAssembly.CompileError`, `WebAssembly.LinkError`, or
         `WebAssembly.RuntimeError` as appropriate for the failure information stored in _reason_.

#### Process a potential WebAssembly response

The above two functions both reuse much of the same infrastructure for extracting bytes from an
appropriate [`Response`](https://fetch.spec.whatwg.org/#response-class) object, differing only in
what they do with those bytes in the end. As such we define the following shared spec-level
procedure:

Process a potential WebAssembly response accepts an input _argument_ and three sets of steps
_processingSteps_, _successSteps_, and _failureSteps_:

- _argument_ is an arbitrary author-supplied JavaScript value.
- _processingSteps_ will occur [in
  parallel](https://html.spec.whatwg.org/multipage/infrastructure.html#in-parallel) (i.e. "off the
  main thread"), accepts a [byte sequence](https://infra.spec.whatwg.org/#byte-sequence), and must
  return a Realm-agnostic success or failure value.
- _successSteps_ and _failureSteps_ will occur in tasks posted back to the main event loop; they
  accept the Realm-agnostic values returned from _processingSteps_, and must transform them into
  JavaScript objects that can be used to appropriately fulfill or reject the returned promise.

Given these values, to process a potential WebAssembly response:

1. Let _returnValue_ be [a new
   promise](https://www.w3.org/2001/tag/doc/promises-guide#a-new-promise).
1. Let _sourceAsPromise_ be [a promise resolved
   with](https://www.w3.org/2001/tag/doc/promises-guide#a-promise-resolved-with) _argument_.
1. [Upon fulfillment](https://www.w3.org/2001/tag/doc/promises-guide#upon-fulfillment) of
   _sourceAsPromise_ with value _unwrappedSource_:
   1. If _unwrappedSource_ is not a [`Response`](https://fetch.spec.whatwg.org/#response-class)
      object, [reject](https://www.w3.org/2001/tag/doc/promises-guide#reject-promise) _returnValue_
      with a `TypeError` exception and abort these substeps.
   1. Let _response_ be _unwrappedSource_'s
      [response](https://fetch.spec.whatwg.org/#concept-response-response).
   1. Let _mimeType_ be the result of
      [extracting a MIME type](https://fetch.spec.whatwg.org/#concept-header-extract-mime-type) from
      _response_'s [header list](https://fetch.spec.whatwg.org/#concept-response-header-list).
   1. If _mimeType_ is not `` `application/wasm` ``, reject _returnValue_ with a `TypeError` and
      abort these substeps. (NOTE: extra parameters are not allowed, including the empty
      `` `application/wasm;` ``.)
   1. If _response_ is not
      [CORS-same-origin](https://html.spec.whatwg.org/multipage/urls-and-fetching.html#cors-same-origin),
      reject _returnValue_ with a `TypeError` and abort these substeps.
   1. If _response_'s [status](https://fetch.spec.whatwg.org/#concept-response-status) is not an
      [ok status](https://fetch.spec.whatwg.org/#ok-status), reject _returnValue_ with a `TypeError`
      and abort these substeps.
   1. [Consume](https://fetch.spec.whatwg.org/#concept-body-consume-body) _response_'s body as an
      `ArrayBuffer`, and let _bodyPromise_ be the result. (NOTE: although it is specified here that
      the response is consumed entirely before _processingSteps_ proceeds, that is purely for ease of
      specification; implementations are likely to instead perform processing in a streaming
      fashion. The different is unobservable, and thus the simpler model is specified.)
      <!-- Using consume is a bit silly as it creates an ArrayBuffer but then we just want the
      underlying bytes. This is because of how streams is specced in terms of promises and JS
      objects whereas we want to operate more directly on the underlying concept. We can revisit
      this if things change in the Streams/Fetch specs. -->
   1. [Upon fulfillment](https://www.w3.org/2001/tag/doc/promises-guide#upon-fulfillment) of
      _bodyPromise_ with value _bodyArrayBuffer_:
      1. Let _bytes_ be the [byte sequence](https://infra.spec.whatwg.org/#byte-sequence) underlying
         _bodyArrayBuffer_.
      1. [In parallel](https://html.spec.whatwg.org/multipage/infrastructure.html#in-parallel),
         perform _processingSteps_, given _bytes_.
      1. If _processingSteps_ succeeds with value _processingSuccessValue_, [queue a
         task](https://html.spec.whatwg.org/multipage/webappapis.html#queue-a-task) on the
         [networking task
         source](https://html.spec.whatwg.org/multipage/webappapis.html#networking-task-source) to
         run _successSteps_ given _processingSuccessValue_, and let _successResult_ be the result of
         those steps. [Resolve](https://www.w3.org/2001/tag/doc/promises-guide#resolve-promise)
         _returnValue_ with _successResult_.
      1. Otherwise, if _processingSteps_ fails with reason _processingFailureReason_, [queue a
         task](https://html.spec.whatwg.org/multipage/webappapis.html#queue-a-task) on the
         [networking task
         source](https://html.spec.whatwg.org/multipage/webappapis.html#networking-task-source) to
         run _failureSteps_ given _processingFailureReason_, and let _failureResult_ be the result
         of those steps. [Reject](https://www.w3.org/2001/tag/doc/promises-guide#reject-promise)
         _returnValue_ with _failureResult_.
   1. [Upon rejection](https://www.w3.org/2001/tag/doc/promises-guide#upon-rejection) of
      _bodyPromise_ with reason _reason_:
      1. [Reject](https://www.w3.org/2001/tag/doc/promises-guide#reject-promise) _returnValue_ with
         _reason_.
1. [Upon rejection](https://www.w3.org/2001/tag/doc/promises-guide#upon-rejection) of
   _sourceAsPromise_ with reason _reason_:
   1. [Reject](https://www.w3.org/2001/tag/doc/promises-guide#reject-promise) _returnValue_ with
      _reason_.
1. Return _returnValue_.

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

Once [SIMD is supported][future simd] WebAssembly would:

* Be statically typed analogous to [SIMD.js-in-asm.js][];
* Reuse specification of operation semantics (with TC39);
* Reuse backend implementation (same IR nodes).

## GC

Once [GC is supported][future garbage collection], WebAssembly code would be able to reference
and access JavaScript, DOM, and general WebIDL-defined objects.

  [same-origin policy]: https://www.w3.org/Security/wiki/Same_Origin_Policy
  [cross-origin resource sharing (CORS)]: https://www.w3.org/TR/cors/
  [subresource integrity]: https://www.w3.org/TR/SRI/
  [SIMD.js-in-asm.js]: http://discourse.specifiction.org/t/request-for-comments-simd-js-in-asm-js

[future simd]: https://github.com/WebAssembly/design/issues/1075
[future garbage collection]: https://github.com/WebAssembly/proposals/issues/16
