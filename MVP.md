# Minimum Viable Product

As stated in the [high-level goals](HighLevelGoals.md), the first release aims
at being a Minimum Viable Product (MVP). This means that there are important
features we *know* we want and need, but are post-MVP; these are in a separate
essential [post-MVP :unicorn:][future general] features document. The MVP will contain
features which are available today in modern web browsers and which perform well
even on mobile devices, which leads to roughly the same functionality as
[asm.js](http://asmjs.org).

The major design components of the MVP have been broken up into separate
documents:

* The distributable, loadable and executable unit of code in WebAssembly
  is called a [module](Modules.md).
* The behavior of WebAssembly code in a module is specified in terms of 
  [instructions](Semantics.md) for a structured stack machine.
* The WebAssembly binary format, which is designed to be natively decoded by 
  WebAssembly implementations, is specified as a 
  [binary encoding](BinaryEncoding.md) of a module's structure and code.
* The WebAssembly text format, which is designed to be read and written when
  using tools (e.g., assemblers, debuggers, profilers), is specified as a
  [textual projection](TextFormat.md) of a module's structure and code.
* WebAssembly is designed to be implemented both [by web browsers](Web.md)
  and [completely different execution environments](NonWeb.md).

[future general]: FutureFeatures.md
