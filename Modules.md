# Modules

The distributable, loadable, and executable unit of code in WebAssembly
is called a **module**. At runtime, a module can be **instantiated** 
with a set of import values to produce an **instance**, which is an immutable
tuple referencing all the state accessible to the running module. Multiple
module instances can access the same shared state which is the basis for 
[dynamic linking](DynamicLinking.md) in WebAssembly. WebAssembly modules
are also designed to [integrate with ES6 modules](#integration-with-es6-modules).

A module contains the following sections:

* [import](#imports)
* [export](#exports)
* [start](#module-start-function)
* [global](#global-section)
* [memory](#linear-memory-section)
* [data](#data-section)
* [table](#table-section)
* [elements](#elements-section)
* [function and code](#function-and-code-sections)

A module also defines several *index spaces* which are statically indexed by
various operators and section fields in the module:

* the [function index space](#function-index-space)
* the [global index space](#global-index-space)
* the [linear memory index space](#linear-memory-index-space)
* the [table index space](#table-index-space)

## Imports

A module can declare a sequence of **imports** which are provided, at
instantiation time, by the host environment. There are several kinds of imports:

* **function imports**, which can be called inside the module by the
  [`call`](Semantics.md#calls) operator;
* **global imports**, which can be accessed inside the module by the
  [global operators](Semantics.md#global-variables);
* **linear memory imports**, which can be accessed inside the module by the
  [memory operators](Semantics.md#linear-memory); and
* **table imports**, which can be accessed inside the module by 
  [call_indirect](Semantics.md#calls) and other
  table operators in the 
  [future :unicorn:][future types].

In the future, other kinds of imports may be added. Imports are designed to
allow modules to share code and data while still allowing separate compilation
and caching.

All imports include two opaque names: a *module name* and an *export name*,
which are required to be [valid UTF-8]. The
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

A *global variable import* includes the *value type* and *mutability*
of the global variable. These fields have the same meaning as in the
[Global section](#global-section). In the MVP, global variable imports must be
*immutable*.

A *linear memory import* includes the same set of fields defined in the
[Linear Memory section](#linear-memory-section):  *initial
length* and optional *maximum length*. The host environment must only allow
imports of WebAssembly linear memories that have initial length
*greater-or-equal* than the initial length declared in the import and that have
maximum length *less-or-equal* than the maximum length declared in the import.
This ensures that separate compilation can assume: memory accesses below the
declared initial length are always in-bounds, accesses above the declared
maximum length are always out-of-bounds and if initial equals maximum, the
length is fixed. In the MVP, every memory is a [default memory](Semantics.md#linear-memory)
and thus there may be at most one linear memory import or linear memory
definition.

A *table import* includes the same set of fields defined in the 
[Table section](#table-section): *element type*, *initial
length* and optional *maximum length*. As with the linear memory section, the
host environment must ensure only WebAssembly tables are imported with
exactly-matching element type, greater-or-equal initial length, and
less-or-equal maximum length. In the MVP, every table is a [default table](Semantics.md#table)
and thus there may be at most one table import or table definition.

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

A module can declare a sequence of **exports** which are returned at
instantiation time to the host environment. Each export has three fields:
a *name*, which is required to be [valid UTF-8],
whose meaning is defined by the host environment, a *type*,
indicating whether the export is a function, global, memory or table, and
an *index* into the type's corresponding [index space](Modules.md).

All definitions are exportable: functions, globals, linear memories and tables.
The meaning an exported definition is defined by the host environment. However,
if another WebAssembly instance imports the definition, then both instances
will share the same definition and the associated state (global variable value,
linear memory bytes, table elements) is shared.

Export names must be unique.

In the MVP, only *immutable* global variables can be exported.

## Module start function

If the module has a start node defined, the function it refers should be called
by the loader after the instance is initialized, including its Memory and Table
though Data and Element sections, and before the exported functions are callable.

* The start function must not take any arguments or return anything
* The function is identified by [function index](#function-index-space), can be
  an import, and can also be exported
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

## Global section

The *global section* provides an internal definition of zero or more
[global variables](Semantics.md#global-variables).

Each global variable internal definition declares its *type*
(a [value type](Semantics.md#types)), *mutability* (boolean flag) and
*initializer* (an [initializer expression](#initializer-expression)).

## Linear memory section

The *linear memory section* provides an internal definition of one
[linear memory](Semantics.md#linear-memory). In the MVP, every memory is a
default memory and there may be at most one linear memory import or linear
memory definition.

Each linear memory section declares an *initial* [memory size](Semantics.md#linear-memory)
(which may be subsequently increased by [`grow_memory`](Semantics.md#resizing)) and an
optional *maximum memory size*.

[`grow_memory`](Semantics.md#resizing) is guaranteed to fail if attempting to
grow past the declared maximum. When declared, implementations *should*
(non-normative) attempt to reserve virtual memory up to the maximum size. While
failure to allocate the *initial* memory size is a runtime error, failure to
reserve up to the *maximum* is not. When a maximum memory size is *not* declared,
on architectures with limited virtual address space, engines should allocate
only the initial size and reallocate on demand.

## Data section

The initial contents of linear memory are zero. The *data section* contains a
possibly-empty array of *data segments* which specify the initial contents
of fixed `(offset, length)` ranges of a given memory, specified by its 
[linear memory index](#linear-memory-index-space). This section is analogous to
the `.data` section of native executables. The `length` is an integer constant
value (defining the length of the given segment). The `offset` is an
[initializer expression](#initializer-expression).

## Table section

The *table section* contains zero or more definitions of distinct 
[tables](Semantics.md#table). In the MVP, every table is a 
default table and there may be at most one table import or table definition.

Each table definition declares an *element type*, *initial length*, and
optional *maximum length*.

In the MVP, the only valid element type is `"anyfunc"`, but in the
[future :unicorn:][future types],
more element types may be added.

In the MVP, tables can only be resized via host-defined APIs (such as
the JavaScript [`WebAssembly.Table.prototype.grow`](JS.md#webassemblytableprototypegrow)).
A `grow_table` may be added in the [future :unicorn:][future types].
In either case, table growth is guaranteed to fail if attempting to grow past
the declared maximum. As with linear memory, when a maximum is declared,
implementations *should* (non-normative) attempt to reserve virtual memory up to
the maximum size. While failure to allocate the *initial* memory size is a
runtime error, failure to reserve up to the *maximum* is not. When a maximum
memory size is *not* declared, on architectures with limited virtual address
space, engines should allocate only the initial size and reallocate on demand.

## Elements section

The intial contents of a tables' elements are sentinel values (that would trap
if called). The *elements section* allows a module to initialize (at
instantiation time) the elements of any imported or internally-defined table
with any other definition in the module. This is symmetric to how the 
[Data section](#data-section) allows a module to initialize the bytes
of any imported or defined memory.

The elements section contains a possibly-empty array of *element segments* which
specify the initial contents of fixed `(offset, length)` ranges of a given
table, specified by its [table index](#table-index-space). The `length` is an
integer constant value (defining the length of the given segment). The `offset`
is an [initializer expression](#initializer-expression). Elements are specified
by their index into the corresponding [index space](Modules.md).

## Function and Code sections

A single logical function definition is defined in two sections: 

 * the *function* section declares the signatures of each internal function
   definition in the module;
 * the *code* section contains the [function body](BinaryEncoding.md#function-bodies)
   of each function declared by the function section.

This split aids in streaming compilation by putting the function bodies,
which constitute most of the byte size of the module, near the end so that all
metadata necessary for recursive module loading and parallel compilation is
available before compilation begins.

## Function Index Space

The *function index space* indexes all imported and internally-defined
function definitions, assigning monotonically-increasing indices based on the
order of definition in the module (as defined by the [binary encoding](BinaryEncoding.md)).
Thus, the index space starts at zero with the function imports (if any) followed
by the functions defined within the module.

The function index space is used by:

* [Calls](Semantics.md#calls), to identify the callee of a direct call.
* [Elements](Modules.md#elements-section).
* [Exports](BinaryEncoding.md#export-section), to determine which functions are exposed to the embedder.
* [Start function](Modules.md#module-start-function), to identify which function is called when the instance is fully initialized.

## Global Index Space

The *global index space* indexes all imported and internally-defined
global definitions, assigning monotonically-increasing indices based on the
order of definition in the module (as defined by the [binary encoding](BinaryEncoding.md)).
Thus, the index space starts at zero with the global imports (if any) followed
by the globals defined within the module.

The global index space is used by:

* [global variable access operators](Semantics.md#global-variables), to
  identify the global variable to read/write
* [data segments](#data-section), to define the offset of a data segment
  (in linear memory) as the value of a global variable

## Linear Memory Index Space

The *linear memory index space* indexes all imported and internally-defined
linear memory definitions, assigning monotonically-increasing indices based on the
order of definition in the module (as defined by the [binary encoding](BinaryEncoding.md)).
Thus, the index space starts at zero with the memory imports (if any) followed
by the memories defined within the module.

The linear memory index space is only used by the 
[data section](#data-section). In the MVP, there is at most one linear memory so
this index space is just a placeholder for when there can be 
[multiple memories :unicorn:][future multiple tables].

## Table Index Space

The *table index space* indexes all imported and internally-defined
table definitions, assigning monotonically-increasing indices based on the
order of definition in the module (as defined by the [binary encoding](BinaryEncoding.md)).
Thus, the index space starts at zero with the table imports (if any) followed
by the tables defined within the module.

The table index space is only used by the [elements section](#elements-section).
In the MVP, there is at most one table so this index space is just
a placeholder for when there can be 
[multiple tables :unicorn:][future multiple tables].

## Initializer Expression

Initializer expressions are evaluated at instantiation time and are currently
used to:

 * define the initial value of [global variables](#global-section)
 * define the offset of a [data segment](#data-section) or 
   [elements segment](#elements-section)

An initializer expression is a pure WebAssembly expression that is encoded with
the same [binary encoding](BinaryEncoding.md) as WebAssembly expressions. Not
all WebAssembly operators can or should be supported in initializer expressions;
initializer expressions represent a minimal pure subset of WebAssembly
expressions.

In the MVP, to keep things simple while still supporting the basic needs
of [dynamic linking](DynamicLinking.md), initializer expressions are restricted
to the following nullary operators:

 * the four [constant operators](Semantics.md#constants); and
 * `get_global`, where the global index must refer to an immutable import.

In the future, operators like `i32.add` could be added to allow more expressive
`base + offset` load-time calculations.

[future types]: FutureFeatures.md#more-table-operators-and-types
[future dom]: FutureFeatures.md#gc/dom-integration
[future multiple tables]: FutureFeatures.md#multiple-tables-and-memories
[valid UTF-8]: https://encoding.spec.whatwg.org/#utf-8-decode-without-bom-or-fail
