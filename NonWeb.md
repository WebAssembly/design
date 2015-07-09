# Non-Browser Embeddings

While WebAssembly is designed to run [within a browser](Web.md), it is
also desirable for it to be able to execute well in other environments,
including everything from minimal shells for testing to full-blown
application environments e.g. on servers in datacenters, on IoT devices,
or mobile/desktop apps. It may even be desirable to execute WebAssembly
embedded within larger programs.

Non-browser environments may provide different APIs than Web
environments, which
[feature testing](FeatureTest.md) and
[dynamic linking](FutureFeatures.md#dynamic-linking) will make discoverable and
usable.

Where there is overlap between the browser and popular non-browser environments,
shared specs could be proposed, but this would be separate from the WebAssembly
spec. A symmetric example in JavaScript would be the
[Loader](https://whatwg.github.io/loader) spec, intended to be implemented by
both browsers and node.js. This situation is expected to be first encountered
with POSIX features such as file I/O. In that respect, WebAssembly would err
towards standardizing existing practice through libraries, and let developers
choose which libraries to use.
Similarly, the SDL interface (exposed by emscripten) could be made into a
shared spec with non-browser mobile/desktop environments, which combined with
the existing WebGL spec would for example allow for cross platform browser
and non-browser games (and other things).

The WebAssembly spec will not try to define any large portable libc-like
library. However, certain features that are core to WebAssembly semantics that
are found in native libc *would* be part of the core WebAssembly spec as either
primitive opcodes or a special builtin module (e.g., `sbrk`, `mmap`).

In general, by keeping the non-browser path such that it doesn't require
browser APIs, WebAssembly could be used as a portable binary format on many
platforms, bringing great benefits in portability, tooling and
language-agnosticity (since it supports C/C++ level semantics).
