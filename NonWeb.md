# Non-Browser Embeddings

While WebAssembly is designed to run [within a browser](Web.md), it is
also desirable for it to be able to execute well in other environments,
including everything from minimal shells for testing to full-blown
application environments e.g. on servers in datacenters. It may even be
desirable to execute WebAssembly embedded within larger programs.

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

The WebAssembly spec will not try to define any large portable libc-like
library. However, certain features that are core to WebAssembly semantics that
are found in native libc *would* be part of the core WebAssembly spec as either
primitive opcodes or a special builtin module (e.g., `sbrk`, `mmap`).
