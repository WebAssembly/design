# MVP

As stated in the [high-level goals](HighLevelGoals.md), MVP is designed to be a
Minimum Viable Product, basically on par with [asm.js](http://asmjs.org/) in terms
of functionality. This means that there are important features we *know* we want 
and need, but are post-MVP; these are in a separate [essential post-MVP features doc](EssentialPostMVPFeatures.md).

This document explains the contents of the MVP at a high-level.  There are also separate docs with more
precise descriptions of:
 * the [AST semantics](AstSemantics.md) 
 * the [binary encoding](BinaryEncoding.md)
 
## Modules
* The primary unit of loadable, executable code is a *module*.
  * In a host environment with ES6 modules (browser, node.js), a WebAssembly
    module is loaded in the same way as an ES6 module (`import` statements,
    `Reflect` API, `Worker` constructor, etc) and the result is reflected to
    JS as an ES6 module object.
* A module can declare a subset of its functions and global variables to be
  *exports*. The meaning of exports (how and when they are called) is defined by
  the host environment.
  * In an environment with ES6 modules, the WebAssembly exports would be the
    ES6 module object exports.
  * A minimal shell environment might define `main` to be the only
    meaningful export.
  * We may want to define an `init` method in the spec that is always called
    after loading a module and before any other exports are called.
* A module can declare a set of *imports*. An import is a tuple containing a 
  module name, export name, and the type to use for the import within the
  module. The host environment controls the mapping from module name to which module
  is loaded.
  * In an environment with ES6 modules, an import first passes the
    module name to the [module loader pipeline](http://whatwg.github.io/loader)
    and resulting ES6 module (which could be implemented in JS or WebAssembly)
    is queried for the export name.
* The spec defines the semantics of loading and calling exports of a
  *single* module. The meaning of a call to an import is defined by
  the host environment.
  * In an environment with ES6 modules, there is no special case for when one
    WebAssembly module imports another: they have separate [heaps](MVP.md#heap)
    and pointers cannot be passed between the two. Module imports encapsulate
    the importer and importee.
  * In a minimal shell environment, imports could be limited to 
    builtin modules (implemented by the shell) and/or shell scripts.
  * The [dynamic linking](FutureFeatures.md#dynamic-linking) post-MVP feature
    would extend the semantics to include multiple modules and thus allow heap
    and pointer sharing. Dynamic linking would be semantically distinct from
    importing, though.
* When compiling from C++, imports would be generated for unresolved
 `extern` functions and calls to those `extern` functions would call the import.
  * Thus, in an environment with ES6 modules, to synchronously call into JS from
    C++, the C++ code would declare and call an undefined `extern` function and
    the target JS function would be given the (mangled) name of the `extern` and
    put inside the imported ES6 module.
* TODO: there is more to discuss here concerning APIs.

## Module structure
 * At the top level, a module is ELF-like: a sequence of sections which declare their type and byte-length.
 * Sections with unknown types would be skipped without error. 
 * Standardized section types:
  * module import section (see [Module imports](MVP.md#module-imports) below)
  * globals section (constants, signatures, variables)
  * code section (see [Code section](MVP.md#code-section) below)
  * heap initialization section (see [Heap](MVP.md#heap) below)

## Code section
 * The code section begins with a table of functions containing the signatures and 
   offsets of each function followed by the list of function bodies.
  * This allows parallel and streaming decoding, validation and compilation.
 * A function body consists of a set of typed variable bindings and an AST closed under these bindings.
  * The AST is composed of two primary kinds of nodes: statements and expressions.
  * Expressions are typed; validation consists of simple, bottom-up, O(1) type checking.
  * Why not a stack-, register- or SSA-based bytecode?
    * Smaller binary encoding: [JSZap](http://research.microsoft.com/en-us/projects/jszap), 
      [Slim Binaries](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.108.1711)
    * [Polyfill prototype](https://github.com/WebAssembly/polyfill) shows simple and efficient translation 
      to asm.js.
 * Control flow is structured (no goto)
  * Simple and size-efficient binary encoding and compilation.
  * Any control flow - even irreducible - can be transformed into structured control flow with the 
    [Relooper](https://github.com/kripken/emscripten/raw/master/docs/paper.pdf)
    [algorithm](http://dl.acm.org/citation.cfm?id=2048224&CFID=670868333&CFTOKEN=46181900), with
    guaranteed low code size overhead, and typically minimal throughput overhead (except for
    pathological cases of irreducible control flow). Alternative approaches can generate reducible
    control flow via node splitting, which can reduce throughput overhead, at the cost of
    increasing code size (potentially very significantly in pathological cases).
  * The [signature-restricted proper tail-call](https://github.com/WebAssembly/spec/blob/master/EssentialPostMVPFeatures.md#signature-restricted-proper-tail-calls)
    feature would allow efficient compilation of arbitrary irreducible control flow.
 * See the [AST Semantics](AstSemantics.md) for descriptions of individual AST nodes.

## Binary format
* The reason we chose a binary format is efficiency: to reduce download size and accelerate
  decoding, thus enabling even very large codebases to have quick startup times.
  * Towards that goal, the binary format will be natively decoded by the browser.
* The binary format has an equivalent and isomorphic [text format](MVP.md#text-format).
  Conversion from one format to the other is both straightforward and causes
  no loss of information in either direction.
* Do not try to compete with a generic compression algorithm by trying to suck out every last bit;
  assume a generic compression algorithm is applied on top of the binary encoding.
* "Given that text is so compressible and it is well known that it is hard to beat gzipped source,
  is there any win from having a binary format over a text format?"
   * Large reductions in payload size can still significantly decrease the compressed file size.
     * Experimental results from the [polyfill prototype](https://github.com/WebAssembly/polyfill) show 
       the gzipped binary format to be about 20-30% smaller than the corresponding gzipped asm.js.
   * A binary format that represents the names of variables and functions with raw indices instead of strings
     is much faster to decode: array indexing vs. dictionary lookup.
      * Experimental results from the [polyfill prototype](https://github.com/WebAssembly/polyfill) show that
        decoding the binary format is about 23x faster than parsing the corresponding asm.js source
        (using [this demo](https://github.com/lukewagner/AngryBotsPacked), comparing *just* parsing
         in SpiderMonkey (no validation, IR generation) to *just* decoding in the polyfill (no asm.js code generation).
* See the [Binary Encoding](BinaryEncoding.md) for more precise description.

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

## Polyfill
 * A working prototype is in the [polyfill repo](https://github.com/WebAssembly/polyfill).
 * Even before browsers ship native support for WebAssembly, users can derive
   value from the polyfill due to the decreased download size of the
   [binary encoding](BinaryEncoding.md) and minimal impact on startup performance.
 * To maintain good polyfill performance, the [polyfill library will diverge](Polyfill.md#polyfill-deviations)
   from the specified WebAssembly semantics in certain corner case scenarios.
 
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
