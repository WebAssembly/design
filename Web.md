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

This section historically contained the description of WebAssembly's Web API.

For the current description, see the [normative documentation](https://webassembly.github.io/spec/web-api/index.html).

## Modules

WebAssembly's [modules](Modules.md) allow for natural [integration with
the ES6 module system](https://github.com/WebAssembly/esm-integration).

## Security

WebAssembly's [security](Security.md) model depend on the
[same-origin policy], with [cross-origin resource sharing (CORS)] and
[subresource integrity] to enable distribution through content
distribution networks and to implement [dynamic linking](DynamicLinking.md).

## WebIDL

There are various proposals in flight which may support future work toward
WebIDL bindings for WebAssembly, including [JS String builtins],
[source-phase imports], and the [component model].

There are also tools to provide this functionality today by generating
JS wrapper code, for example [Emscripten's WebIDL Binder],
[the wasm-webidl-bindings Rust crate], and
[jco's experimental WebIDL Imports support].

[same-origin policy]: https://www.w3.org/Security/wiki/Same_Origin_Policy
[cross-origin resource sharing (CORS)]: https://www.w3.org/TR/cors/
[subresource integrity]: https://www.w3.org/TR/SRI/
[JS String builtins]: https://github.com/WebAssembly/js-string-builtins/
[source-phase imports]: https://github.com/tc39/proposal-source-phase-imports
[component model]: https://github.com/WebAssembly/component-model

[Emscripten's WebIDL Binder]: https://emscripten.org/docs/porting/connecting_cpp_and_javascript/WebIDL-Binder.html
[the wasm-webidl-bindings Rust crate]: https://github.com/rustwasm/wasm-webidl-bindings
[jco's experimental WebIDL Imports support]: https://github.com/bytecodealliance/jco/blob/main/docs/src/transpiling.md#experimental-webidl-imports
