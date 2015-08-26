# Dynamic linking

Dynamic loading of code is in [the MVP](MVP.md) in the form of
[modules](Modules.md), but all loaded modules have their own separate
[linear memory](AstSemantics.md#linear-memory) and cannot share
[function pointers](ASTSemantics.md#calls). Dynamic linking will allow
developers to share memory and function pointers between WebAssembly modules.

WebAssembly will support both load-time and run-time (`dlopen`) dynamic linking
of both WebAssembly modules and non-WebAssembly modules (e.g., on the Web, ES6
ones containing JavaScript).

Dynamic linking is especially useful when combined with a Content Distribution
Network (CDN) such as [hosted libraries][] because the library is only ever
downloaded and compiled once per user device. It can also allow for smaller
differential updates, which could be implemented in collaboration with
[service workers][].

We would like to standardize a single [ABI][] per source language, allowing for
WebAssembly modules to interface with each other regardless of compiler. While
it is highly recommended for compilers targeting WebAssembly to adhere to the
specified ABI for interoperability, WebAssembly runtimes will be ABI agnostic,
so it will be possible to use a non-standard ABI for specialized purposes.

Although dynamic linking is not part of the MVP, it is important and has
significant implications on many aspects of the design that do impact the MVP,
such as the way linear memory is managed, how module imports and exports are
specified, and how globals and function pointers work.  Therefore we want to
have some viable ideas to ensure we don't standardize a design that
unnecessarily complicates the design or implication of dynamic linking.

  [hosted libraries]: https://developers.google.com/speed/libraries/
  [service workers]: https://www.w3.org/TR/service-workers/
  [ABI]: https://en.wikipedia.org/wiki/Application_binary_interface
