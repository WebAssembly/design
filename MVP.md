# Minimum Viable Product

As stated in the [high-level goals](HighLevelGoals.md), the first release aims
at being a Minimum Viable Product (MVP), with roughly the same functionality as
[asm.js](http://asmjs.org). This means that there are important features we
*know* we want and need, but are post-MVP; these are in a separate [essential
post-MVP features doc](EssentialPostMVPFeatures.md).

This document explains the contents of the MVP at a high-level. There are also
separate docs with more precise descriptions of:

 * the [polyfill to JavaScript](Polyfill.md)
 * the [AST semantics](AstSemantics.md) 
 * the [binary encoding](BinaryEncoding.md)

**Note**: This content is still in flux and open for discussion.

## Modules

* The primary unit of loadable, executable code is a **module**.
* A module can declare a subset of its functions and global variables to be
  **exports**. The meaning of exports (how and when they are called) is defined
  by the host environment.
  * `_start` can be the only meaningful export.
  * An `init` method could always be called after loading a module and before
    any other exports are called.
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
* **TODO**: there is more to discuss here concerning APIs.

## Module structure

* At the top level, a module is ELF-like: a sequence of sections which declare
  their type and byte-length.
 * Sections with unknown types would be skipped without error. 
 * Standardized section types:
  * module import section (see [Module imports](MVP.md#module-imports) below);
  * globals section (constants, signatures, variables);
  * code section (see [Code section](MVP.md#code-section) below);
  * heap initialization section (see [Heap](MVP.md#heap) below).

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
* The purpose of this format is to support:
  * View Source on a WebAssembly module, thus fitting into the Web (where every source can
    be viewed) in a natural way.
  * Presentation in browser devtools when source maps aren't present (which is necessarily the case with the MVP).
  * Writing WebAssembly code directly for reasons including pedagogical, experimental, debugging, or
    optimization.
* The text format is equivalent and isomorphic to the
  [binary format](MVP.md#binary-format), see notes there.
* The text format will be standardized, but only for tooling purposes:
  * Compilers will support this format for `.S` and inline assembly.
  * Debuggers and profilers will present binary code using this textual format.
  * Browsers will not parse the textual format on regular web content in order
    to implement WebAssembly semantics.
* Given that the code representation is actually an AST, the syntax would contain nested
  statements and expressions (instead of the linear list of instructions most assembly languages have).
* There is no requirement to use JS syntax; this format is not intended to be evaluated or translated 
  directly into JS.
* TODO: there is no real proposal yet

## Heap
 * In the MVP, when a WebAssembly module is loaded, it creates a new heap.
   * The [dynamic linking](FutureFeatures.md#dynamic-linking) feature will be necessary for two
     WebAssembly modules to share the same heap (but sharing with JS is possible in the MVP, see below).
 * Modules can specify heap size and initialization data (data, rodata, bss) in the 
   [heap-initialization section](MVP.md#module-structure).
 * Modules can specify whether the heap is growable (via `sbrk`).
 * Modules can optionally export the heap, allowing it to be aliased by JS.
   * JS sees the exported heap as an ArrayBuffer.
   * To keep an ArrayBuffer's length immutable, resizing a module's heap detaches any existent ArrayBuffers.
 * See the [AST Semantics heap section](AstSemantics.md#accessing-the-heap) for
   more details.
 
## Non-browser embedding
 * Host environments can define builtin modules that are implemented natively but can otherwise
   be imported like [other modules](MVP.md#code-loading-and-imports).
  * For example, a WebAssembly shell might define a builtin `stdio` library with an export `puts`.
  * Another example, in the browser, would be the WebIDL support mentioned in [future features](FutureFeatures.md).
 * Where there is overlap between the browser and popular non-browser environments, a shared spec could be 
   proposed, but this would be separate from the WebAssembly spec.
  * A symmetric example in JS would be the [Loader](http://whatwg.github.io/loader) spec, intended to be
    implemented by both browsers and node.js.
 * However, one might find a fair amount of variance between the browser and other environments
   on core APIs like network and file I/O.
 * To allow writing portable POSIX-like code (that ran in both browser and other environments), the
   WebAssembly community would develop a shared repository of WebAssembly code that mapped between a 
   POSIX-like interface and the host's builtin modules using compile-time #ifdefs or, after
   [dynamic linking](FutureFeatures.md#dynamic-linking) was added, client-side dynamic feature testing.
  * A symmetric example in JS would be the [ES6 Module Loader polyfill](https://github.com/ModuleLoader/es6-module-loader) library.
 * The WebAssembly spec would thus not try to define any large portable libc-like library.
  * However, certain features that are core to WebAssembly semantics that are found in native libc 
    *would* be part of the core WebAssembly spec as either primitive opcodes or a special builtin 
    module (e.g., `sbrk`, `mmap`).

## Security
 * No different from a security pov than if the WebAssembly module was asm.js.
