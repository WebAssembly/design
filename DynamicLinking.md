# Dynamic linking

Dynamic loading of code is in [the MVP](MVP.md) in the form of
[modules](Modules.md), but all loaded modules have their own separate
[linear memory](AstSemantics.md#linear-memory) and cannot share
[function pointers](AstSemantics.md#calls). Dynamic linking will allow
developers to share memory and function pointers between WebAssembly
dynamic libraries.

WebAssembly will support both load-time and run-time (`dlopen`) dynamic linking
of libraries.

One important requirement of dynamic linking is to allow the linked module
to have its own position-independent global data segment. This could be achieved
by specifying a new kind of link-time-initialized immutable global variable
which would be initialized with the address (in linear memory) of the modules'
global data segment. These immutable globals could also be used to provide
a linked module with the offsets of its function pointers in the instance's
function pointer tables. An important aspect of immutable globals is that they
could either be patched directly as constant values or implemented through a
[Global Offset Table](https://en.wikipedia.org/wiki/Position-independent_code)
in position-independent code.

Dynamic linking is especially useful when combined with a Content Distribution
Network (CDN) such as [hosted libraries][] because the library is only ever
downloaded and compiled once per user device. It can also allow for smaller
differential updates, which could be implemented in collaboration with
[service workers][].

We would like to standardize a single [ABI][] per source language, allowing for
WebAssembly libraries to interface with each other regardless of compiler. While
it is highly recommended for compilers targeting WebAssembly to adhere to the
specified ABI for interoperability, WebAssembly runtimes will be ABI agnostic,
so it will be possible to use a non-standard ABI for specialized purposes.

Although dynamic linking is not part of the MVP, it has significant implications
on many aspects of the design that do impact the MVP, such as the way linear
memory is managed, how module imports and exports are specified, and how globals
and function pointers work.  Therefore we want to have some viable ideas to
ensure we don't standardize a design that unnecessarily complicates the design
or implementation of dynamic linking.

  [hosted libraries]: https://developers.google.com/speed/libraries/
  [service workers]: https://www.w3.org/TR/service-workers/
  [ABI]: https://en.wikipedia.org/wiki/Application_binary_interface
