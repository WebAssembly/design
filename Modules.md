# Modules

The distributable, loadable, and executable unit of code in WebAssembly
is called a **module**. At runtime, a module can be loaded by a runtime
to produce an **instance** which encapsulates all the state directly
manipulated by running WebAssembly code. A WebAssembly instance's initial state
is determined by the module it was loaded from.

A module contains:
* a set of [imports and exports](Modules.md#imports-and-exports);
* a section defining [linear memory](Modules.md#linear-memory-section);
* a section containing [code](Modules.md#code-section);
* after the MVP, sections containing [debugging/symbol information](Tooling.md) or
  a reference to separate files containing them; and
* possibly other sections in the future.
Sections declare their type and byte-length. Sections with unknown types are
silently ignored.

An instance contains:
* the code of the module from which the instance was loaded;
* a [linear memory](AstSemantics.md#linear-memory);
* fully resolved imports;
* host-specific state (for example, the JavaScript function objects that reflect
  exported functions to JavaScript);
* (when [threading](PostMVP.md#threads) is added) TLS variable state;
* (when [dynamic linking](DynamicLinking.md) is added) the code of multiple modules
  that have been dynamically linked into the same instance;
* and other semantically-visible state added by other future features.

While WebAssembly modules are designed to interoperate with ES6 modules
in a Web environment (more details [below](Modules.md#integration-with-es6-modules)),
WebAssembly modules are defined independently of JavaScript and do not require
the host environment to include a JavaScript VM.

## Imports and Exports

A module defines a set of functions in its
[code section](Modules.md#code-section) and can declare and name a subset of
these functions to be **exports**. The meaning of exports (how and when they are
called) is defined by the host environment. For example, a minimal shell
environment might only probe for and call a `_start` export when given a module
to execute. Exports are exported by name, where the name is an arbitrary byte
string of a given length. The host may need to mangle these names.

A module can declare a set of **imports**. An import is a tuple containing a
module name, the name of an exported function to import from the named module,
and the signature to use for that import within the importing module. Within a
module, the import can be [directly called](AstSemantics.md#calls) like a
function (according to the signature of the import). When the imported
module is also WebAssembly, it would be an error if the signature of the import
doesn't match the signature of the export.

The WebAssembly spec does not define how imports are interpreted:
* the host environment can interpret the module name as a file path, a URL,
  a key in a fixed set of builtin modules or the host environment may invoke a
  user-defined hook to resolve the module name to one of these;
* the module name does not need to resolve to a WebAssembly module; it
  could resolve to a builtin module (implemented by the host environment) or a
  module written in another, compatible language; and
* the meaning of calling an imported function is host-defined.

The open-ended nature of module imports allow them to be used to expose
arbitrary host environment functionality to WebAssembly code, similar to a
native `syscall`. For example, a shell environment could define a builtin
`stdio` module with an export `puts`.

In C/C++, an undefined `extern` declaration (perhaps only when given the
magic `__attribute__` or declared in a separate list of imports) could be
compiled to an import and C/C++ calls to this `extern` would then be compiled
to calls to this import. This is one way low-level C/C++ libraries could call
out of WebAssembly in order to implement portable source-level interfaces
(e.g., POSIX, OpenGL or SDL) in terms of host-specific functionality.

### Integration with ES6 modules

While ES6 defines how to parse, link and execute a module, ES6 does not
define when this parsing/linking/execution occurs. An additional extension
to the HTML spec is required to say when a script is parsed as a module instead
of normal global code. This work is [ongoing](https://github.com/whatwg/loader/blob/master/roadmap.md).
Currently, the following entry points for modules are being considered:
* `<script type="module">`;
* an overload to the `Worker` constructor;
* an overload to the `importScripts` Worker API;

Additionally, an ES6 module can recursively import other modules via `import`
statements.

For WebAssembly/ES6 module integration, the idea is that all the above module
entry points could also load WebAssembly modules simply by passing the URL of a
WebAssembly module. The distinction of whether the module was WebAssembly or ES6
code could be made by namespacing or by content sniffing the first bytes of the
fetched resource (which, for WebAssembly, would be a non-ASCII&mdash;and thus
illegal as JavaScript&mdash;[magic number](https://en.wikipedia.org/wiki/Magic_number_%28programming%29)).
Thus, the whole module-loading pipeline (resolving the name to a URL, fetching
the URL, any other [loader hooks](http://whatwg.github.io/loader/)) would be
shared and only the final stage would fork into either the JavaScript parser or
the WebAssembly decoder.

Any non-builtin imports from within a WebAssembly module would be treated as
if they were `import` statements of an ES6 module. If an ES6 module `import`ed
a WebAssembly module, the WebAssembly module's exports would be linked as if
they were the exports of an ES6 module. Once parsing and linking phases
were complete, a WebAssembly module would have its `_start` function called in
place of executing the ES6 module top-level script. By default, multiple 
loads of the same module URL (in the same realm) reuse the same instance. It may
be worthwhile in the future to consider extensions to allow applications to
load/compile/link a module once and instantiate multiple times (each with a
separate linear memory).

This integration strategy should allow WebAssembly modules to be fairly
interchangeable with ES6 modules (ignoring 
[GC/Web API](FutureFeatures.md#gc/dom-integration) signature restrictions of the
WebAssembly MVP) and thus it should be natural to compose a single application
from both kinds of code. This goal motivates the 
[semantic design](AstSemantics.md#linear-memory) of giving each WebAssembly
module its own disjoint linear memory. Otherwise, if all modules shared a single
linear memory (all modules with the same realm? origin? window?&mdash;even the
scope of "all" is a nuanced question), a single app using multiple
independent libraries would have to hope that all the WebAssembly modules
transitively used by those libraries "played well" together (e.g., explicitly
shared `malloc` and coordinated global address ranges). Instead, the
[dynamic linking future feature](DynamicLinking.md) is intended
to allow *explicitly* injecting multiple modules into the same instance.

## Linear memory section

A module may contain an optional section declaring the use of linear memory
by the module. If the section is absent, the linear memory operators
`load`, `store`, `memory_size`, and `grow_memory` may not be used in the module.

The linear memory section declares the initial [memory size](AstSemantics.md#linear-memory)
(which may be subsequently increased by [`grow_memory`](AstSemantics.md#resizing)).

The initial contents of linear memory are zero by default. However, the memory
section contains a possibly-empty array of *segments* (analogous to `.data`)
which can specify the initial contents of fixed `(offset, length)` ranges of
memory.

The linear memory section may also contain an optional hint declaring the expected
maximum heap usage. This hint is not semantically visible but can help a
WebAssembly engine to optimize `grow_memory`.

## Code section

The WebAssembly spec defines the code section of a module in terms of an
[Abstract Syntax Tree](AstSemantics.md) (AST). Additionally, the spec defines
two concrete representations of the AST: a [binary format](BinaryEncoding.md)
which is natively decoded by the browser and a [text format](TextFormat.md)
which is intended to be read and written by humans. A WebAssembly environment
is only required to understand the binary format; the text format is defined so
that WebAssembly modules can be written by hand (and then converted to binary
with an offline tool) and so that developer tools have a well-defined text
projection of a binary WebAssembly module. This design separates the concerns
of specifying and reasoning about behavior, over-the-wire size and compilation
speed, and ergonomic syntax.

