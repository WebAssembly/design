# Minimum Viable Product

As stated in the [high-level goals](HighLevelGoals.md), the first release aims
at being a Minimum Viable Product (MVP). This means that there are important
features we *know* we want and need, but are post-MVP; these are in a separate
essential [post-MVP](PostMVP.md) features document. The MVP will contain
features which are available today in modern web browsers and which perform well
even on mobile devices, which leads to roughly the same functionality as
[asm.js](http://asmjs.org).

This document explains the contents of the MVP at a high-level. There are also
separate docs with more precise descriptions of:

 * [Modules](Modules.md)
 * [Polyfill to JavaScript](Polyfill.md);
 * [AST semantics](AstSemantics.md);
 * [Binary encoding](BinaryEncoding.md);
 * [Text format](TextFormat.md);
 * Implementation [in the browser](Web.md) and [outside the browser](NonWeb.md).

**Note**: This content is still in flux and open for discussion.

## Linear Memory

* In the MVP, when a WebAssembly module is loaded, it creates a new linear memory which
  isn't directly accessible from other modules.
* The [dynamic linking](FutureFeatures.md#dynamic-linking) feature will be
  necessary for two WebAssembly modules to share the same linear memory.
* Modules can specify memory size and initialization data (`data`, `rodata`,
  `bss`) in the [memory-initialization section](MVP.md#module-structure).
* Modules can optionally export memory, allowing it to be aliased by the
  embedder, such as JavaScript:
  * JavaScript sees the exported memory as an `ArrayBuffer`.
  * To keep an `ArrayBuffer`'s length immutable, resizing a module's memory
    detaches any existent `ArrayBuffer`.
* See the [AST Semantics linear memory section](AstSemantics.md#linear-memory)
  for more details.

## Binary format

* A [binary format](BinaryEncoding.md) provides efficiency: it reduces download
  size and accelerates decoding, thus enabling even very large codebases to have
  quick startup times. Towards that goal, the binary format will be natively
  decoded by browsers.
* The binary format has an equivalent and isomorphic
  [text format](MVP.md#text-format).  Conversion from one format to the other is
  both straightforward and causes no loss of information in either direction.

## Text format

The [text format](TextFormat.md) provides readability to developers, and is
isomorphic to the [binary format](BinaryEncoding.md).
 
## Security

WebAssembly MVP will be no looser from a security point-of-view than if the
module was JavaScript or asm.js.
