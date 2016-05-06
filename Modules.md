# Modules

The distributable, loadable, and executable unit of code in WebAssembly
is called a **module**. At runtime, a module can be **instantiated** 
to produce an **instance**, which is an immutable tuple referencing all
the state accessible to the running module. Multiple module instances can
access the same shared state which is the basis for 
[dynamic linking](DynamicLinking.md) in WebAssembly. WebAssembly modules
are also designed to [integrate with ES6 modules](#integration-with-es6-modules).

A module may contain the following sections:
* [imports](#imports)
* [exports](#exports)
* [start](#module-start-function)
* [linear memory](#linear-memory-section)
* [data](#data-section)
* [table](#table-section)
* [elements](#elements-section)
* [code](#code-section)

An instance contains:
* the code of the instantiated module; and
* a set of [index spaces](#index-spaces) of linear memories, tables and
  functions that were imported or defined by the module.

## Index Spaces

Any number of functions, memories and tables can be both imported and
defined in a given module. Imports and definitions are merged into index
spaces (one for each of memories, tables and functions) which allows uniform
treatment of imports and definitions thereafter (via index).

Specifically, the index space of functions is defined by first giving each of
the M function imports an index \[0, M\) (ordered by their sequence in the
imports section) and then giving each of the N function definitions an index 
\[M, M+N\) (ordered by their sequence in the module). The memory and table
index spaces are defined in the same way.

Thus, any use of a *function index* (e.g., a [`call`](AstSemantics.md#calls))
can refer to either a function import or definition and similarly for
and *table index* or *memory index*.

## Imports

A module can declare a sequence of **imports** which are provided, at
instantiation time, by the host environment. There are several kinds of imports:
* **function imports**, which can be called inside the module by the
  [`call`](AstSemantics.md#calls) operator;
* **linear memory imports**, which can be accessed inside the module by the
  [memory operators](AstSemantics.md#linear-memory); and
* **table imports**, which can be accessed inside the module by 
  [call_table](AstSemantics.md#calls) and other
  table operators in the 
  [future](FutureFeatures.md#more-table-operators-and-types).

In the future, other kinds of imports may be added. Imports are designed to
allow modules to share code and data while still allowing separate compilation
and caching.

All imports include two opaque names: a *module name* and an *export name*. The
interpretation of these names is up to the host environment but designed to
allow a host environments, like the [Web](Web.md), to support a two-level
namespace.

Each specific kind of import defines additional fields:

A *function import* includes a signature to use for the imported
function *inside* the module. The checking of the signature against the
imported function *outside* the module is defined by the host environment.
However, if the imported function is a WebAssembly function, the host
environment must raise an instantiation-time error if there is a signature
mismatch.

A *linear memory import* includes the same set of fields defined in the
[Linear Memory section](#linear-memory-section): *initial length* and
optional *maximum length*. The host environment must only allow imports
of WebAssembly linear memories that have initial length *greater-or-equal* than
the initial length declared in the import and that have maximum length
*less-or-equal* than the maximum length declared in the import. This ensures
that separate compilation can assume: memory accesses below the declared initial
length are always in-bounds, accesses above the declared maximum length are
always out-of-bounds and if initial equals maximum, the length is fixed.

A *table import* includes the same set of fields defined in the 
[Table section](#table-section): *element type*, *initial length*
and optional *maximum length*. As with the linear memory section,
the host environment must ensure only WebAssembly tables are imported
with exactly-matching element type, greater-or-equal initial length,
and less-or-equal maximum length.

Since the WebAssembly spec does not define how import names are interpreted:
* the [Web environment](Web.md#names) defines names to be UTF8-encoded strings;
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

## Exports

A module can declare a sequence of **exports** which are provided, at
instantiation time, to the host environment. There are several kinds of exports:
* **function exports**, which allow the exported function to be called by the
  host environment (or other code running in the host environment);
* **linear memory exports**, which allow the exported linear memory to be
  aliased by the host environment (or other code running int he host
  environment); and
* **table exports**, which allow the elements of the table to be read, written
  or called by the host environment (or other code running in the host
  environment).

Exports additionally contain a name and an index into the associated
export's type's [index space](#index-spaces). As with imports, the meaning
of an export name is defined by the host.

## Integration with ES6 modules

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
were complete, a WebAssembly module would have its start function, defined
by the start module option, called in place of executing the ES6 module
top-level script. By default, multiple loads of the same module URL (in
the same realm) reuse the same instance. It may be worthwhile in the future
to consider extensions to allow applications to load/compile/link a module
once and instantiate multiple times (each with a separate linear memory).

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

## Module start function

If the module has a start node defined, the function it refers should be called
by the loader after the instance is initialized and before the exported functions
are called.

* The start function must not take any arguments or return anything
* The function is identified by [function index](#index-spaces) and can also be
  exported
* There can only be at most one start node per module

For example, a start node in a module will be:

```(start $start_function)```

or

```(start 42)```

In the first example, the environment is expected to call the function $start_function
before calling any other module function. In the second case, the environment is
expected to call the module function indexed 42. This number is the function index starting from 0 (same as for `export`).

A module can:
* Only have at most a start node
* If a module contains a start node, the function must be defined in the module
* The start function will be called after module loading and before any call to the module
    function is done

## Linear memory section

The *linear memory section* may contain zero or more definitions of distinct
[linear memories](AstSemantics.md#linear-memory) which are added to the 
[linear memory index space](#index-spaces). Each linear memory section declares
an initial [memory size](AstSemantics.md#linear-memory) (which may be
subsequently increased by [`grow_memory`](AstSemantics.md#resizing)) and an
optional maximum memory size.

[`grow_memory`](AstSemantics.md#resizing) is guaranteed to fail if attempting to
grow past the declared maximum. When declared, implementations *should*
(non-normative) attempt to reserve virtual memory up to the maximum size. While
failure to allocate the *initial* memory size is a runtime error, failure to
reserve up to the *maximum* is not. When a maximum memory size is *not* declared,
on architectures with limited virtual address space, engines should allocate
only the initial size and reallocate on demand.

## Data section

The initial contents of linear memory are zero. The *data section* contains a
possibly-empty array of *data segments* which specify the initial contents
of fixed `(offset, length)` ranges of a given memory, specified by its [linear
memory index](#index-space). This section is analogous to the `.data` section
of native executables.

## Table section

A *table section* may contain zero or more definitions of distinct 
[tables](AstSemantics.md#tables) which are added to the 
[table index space](#index-spaces). Each table section declares an *element
type*, *initial length*, and optional *maximum length*.

In the MVP, tables can only be resized via host-defined APIs (such as
the JavaScript [`WebAssembly.Table.prototype.grow`](JS.md#webassemblytableprototypegrow)).
A `grow_table` may be added in the [future](FutureFeatures.md#more-table-operators-and-types).
In either case, table growth is guaranteed to fail if attempting to grow past
the declared maximum. As with linear memory, when a maximum is declared,
implementations *should* (non-normative) attempt to reserve virtual memory up to
the maximum size. While failure to allocate the *initial* memory size is a
runtime error, failure to reserve up to the *maximum* is not. When a maximum
memory size is *not* declared, on architectures with limited virtual address
space, engines should allocate only the initial size and reallocate on demand.

## Elements section

For function tables, the intial contents of the tables' elements are sentinel
values that throw if called. The *elements section* allows a module to
initialize (at instantiation time) the elements of any imported or defined
table with any function in the module. This is symmetric to how the 
[Data section](#data-section) allows a module to initialize the bytes
of any imported or defined memory.

Specifically, the elements section contains a possibly-empty array of
*element segments*. Each element segment contains a 
[table index](#index-spaces), indicating which table to initialize,
an *offset* (where in the table to start initializing) and then
an array of [function indices](#index-spaces) whose corresponding 
functions will be stored into the table starting at the offset.

## Code section

The code section contains a sequence of functions definitions which are added to
the [function index space](#index-spaces). Functions are split into
a sequence of [signature declarations](BinaryEncoding.md#function-section)
and [bodies](BinaryEncoding.md#code-section) as defined in the binary encoding.
