# Non-Web Embeddings

While WebAssembly is designed to run [on the Web](Web.md), it is
also desirable for it to be able to execute well in other environments,
including everything from minimal shells for testing to full-blown
application environments e.g. on servers in datacenters, on IoT devices,
or mobile/desktop apps. It may even be desirable to execute WebAssembly
embedded within larger programs.

Non-Web environments may provide different APIs than Web
environments.

Non-Web environments include JavaScript VMs (e.g. [node.js]), however
WebAssembly is also capable of being executed without a JavaScript VM present.
The table in the [features page] of the website lists several such
implementations.

[node.js]: https://nodejs.org
[features page]: https://webassembly.org/features/

[WASI] is a set of standards-track APIs being developed by the [WASI Subgroup]
intended for use in many environments, including in non-browser and non-JS
engines.

[WASI]: https://wasi.dev
[WASI Subgroup]: https://github.com/WebAssembly/wasi

The WebAssembly spec itself will not try to define any large portable libc-like
library. However, certain features that are core to WebAssembly semantics that
are similar to functions found in native libc *would* be part of the core
WebAssembly spec as primitive operators (e.g., the `memory.grow` operator, which
is similar to the `sbrk` function on many systems, and in the future, perhaps
operators supporting `dlopen` functionality).

Where there is overlap between the Web and popular non-Web environments,
shared specs could be proposed, but these would be separate from the WebAssembly
spec. A symmetric example in JavaScript would be the in-progress 
[Loader](https://whatwg.github.io/loader) spec, which is proposed for both
Web and node.js environments and is distinct from the JavaScript spec.

However, for most cases it is expected that, to achieve portability at the
source code level, communities would build libraries that mapped from a 
source-level interface such as POSIX or SDL to the host environment's builtin
capabilities, either at build time or runtime, possibly making use of
[feature testing](FeatureTest.md), [dynamic linking](DynamicLinking.md),
static linking, or polyfilling.

In general, by preserving spec layering, and the core WebAssembly spec
independent of the host environment and linking specs, WebAssembly can be
used as a portable binary format on many platforms, in many environments, for
many use cases, bringing great benefits in portability, tooling and
language-agnosticity.

[shared-everything linking]: https://github.com/WebAssembly/component-model/blob/main/design/mvp/examples/SharedEverythingDynamicLinking.md
