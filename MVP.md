# Minimum Viable Product

As stated in the [high-level goals](HighLevelGoals.md), the first release aims
at being a Minimum Viable Product (MVP), with roughly the same functionality as
[asm.js](http://asmjs.org). This means that there are important features we
*know* we want and need, but are post-MVP; these are in a separate [essential
post-MVP features doc](EssentialPostMVPFeatures.md).

This document explains the contents of the MVP at a high-level. There are also
separate docs with more precise descriptions of:

 * [Polyfill to JavaScript](Polyfill.md);
 * [AST semantics](AstSemantics.md);
 * [Binary encoding](BinaryEncoding.md);
 * Implementation [in the browser](Web.md) and [outside the browser](NonWeb.md).

**Note**: This content is still in flux and open for discussion.

## Modules

* The primary unit of loadable, executable code is a **module**.
* A module can declare a subset of its functions and global variables to be
  **exports**. The meaning of exports (how and when they are called) is defined
  by the host environment. For example, `_start` and `init` can be the only
  meaningful exports.
* A module can declare a set of **imports**. An import is a tuple containing a
  module name, export name, and the type to use for the import within the
  module. The host environment controls the mapping from module name to which
  module is loaded.
* The spec defines the semantics of loading and calling exports of a *single*
  module. The meaning of a call to an import is defined by the host environment.
  * In a minimal shell environment, imports could be limited to builtin modules
    (implemented by the shell) and/or shell scripts.
  * The [dynamic linking](FutureFeatures.md#dynamic-linking) post-MVP feature
    would extend the semantics to include multiple modules and thus allow heap
    and pointer sharing. Dynamic linking would be semantically distinct from
    importing, though.
* When compiling from C++, imports would be generated for unresolved `extern`
 functions and calls to those `extern` functions would call the import.
* Host environments can define builtin modules that are implemented natively but
  can otherwise be imported like [other modules](MVP.md#Modules). As examples:
  * A WebAssembly shell might define a builtin `stdio` library with an export
    `puts`.
  * In the browser, the WebIDL support mentioned in
    [future features](FutureFeatures.md).
* **TODO**: there is more to discuss here concerning APIs.

## Module structure

* At the top level, a module is ELF-like: a sequence of sections which declare
  their type and byte-length.
 * Sections with unknown types would be skipped without error. 
 * Standardized section types:
  * module import section;
  * globals section (constants, signatures, variables);
  * code section;
  * heap initialization section.

## Code section

* The code section begins with a table of functions containing the signatures
   and offsets of each function followed by the list of function bodies. This
   allows parallel and streaming decoding, validation and compilation.
 * A function body consists of a set of typed variable bindings and an AST
   closed under these bindings.
  * The [Abstract Syntax Tree](AstSemantics.md) is composed of two primary kinds
    of nodes: statements and expressions.
 * [Control flow](AstSemantics.md#Control_flow_structures) is structured (no
   `goto`).

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

## Heap

* In the MVP, when a WebAssembly module is loaded, it creates a new heap which
  isn't directly accessible from other modules.
   * The [dynamic linking](FutureFeatures.md#dynamic-linking) feature will be
     necessary for two WebAssembly modules to share the same heap.
 * Modules can specify heap size and initialization data (`data`, `rodata`,
   `bss`) in the [heap-initialization section](MVP.md#module-structure).
 * Modules can specify whether the heap is growable (via `sbrk`).
 * Modules can optionally export the heap, allowing it to be aliased by
   the embedder, such as JavaScript.
   * JavaScript sees the exported heap as an `ArrayBuffer`.
   * To keep an `ArrayBuffer`'s length immutable, resizing a module's heap
     detaches any existent `ArrayBuffer`.
 * See the [AST Semantics heap section](AstSemantics.md#accessing-the-heap) for
   more details.
 
## Security

WebAssembly MVP will be no looser from a security point-of-view than if the
module was JavaScript or asm.js.
