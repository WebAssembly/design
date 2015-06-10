# Non-Browser Embeddings

WebAssembly is designed to run primarily [within a browser](Web.md). It is
nonetheless desirable to be able to execute it inside standalone shells for
testing and as a host environment for untrusted code (e.g. in a datacenter). It
may even be desirable to execute WebAssembly embedded within larger programs.

Non-browser environments may have access to different APIs, which
[feature testing](FeatureTest.md) and
[dynamic linking](FutureFeatures.md#dynamic-linking) will make discoverable and
usable.
  
Where there is overlap between the browser and popular non-browser environments,
a shared spec could be proposed, but this would be separate from the WebAssembly
spec. A symmetric example in JavaScript would be the
[Loader](http://whatwg.github.io/loader) spec, intended to be implemented by
both browsers and node.js. This situation is expected to be first encountered
with POSIX features such as file I/O. In that respect, WebAssembly would err
towards standardizing existing practice through libraries, and let developers
choose which libraries to use.

The WebAssembly spec will not try to define any large portable libc-like
library. However, certain features that are core to WebAssembly semantics that
are found in native libc *would* be part of the core WebAssembly spec as either
primitive opcodes or a special builtin module (e.g., `sbrk`, `mmap`).
