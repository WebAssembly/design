# Dynamic linking

WebAssembly allows load-time and run-time (`dlopen`) dynamic linking in the
MVP. This is enabled by having multiple [instantiated modules](Modules.md)
share functions, [linear memory](AstSemantics.md#linear-memory) and
[tables](AstSemantics.md#tables) using module [imports](Modules.md#imports)
and [exports](Modules.md#exports). In particular, since all (non-local) state
that a module can access can be imported and exported and thus shared between
separate modules' instances, toolchains have the building blocks to implement
dynamic loaders.

Since the manner in which modules are loaded and instantiated is defined by the
host environment (e.g., the [JavaScript API](JS.md)), dynamic linking requires
use of host-specific functionality to link two modules. At a minimum, the host
environment must provide a way to dynamically instantiate modules while
connecting exports to imports.

The simplest load-time dynamic linking scheme between modules A and B can be 
achieved by having module A export functions, tables and memories that are
imported by B. A C++ toolchain can expose this functionality by using the
same function attributes currently used to export/import symbols from
native DSOs/DLLs:
```
#ifdef _WIN32
#  define EXPORT __declspec(dllexport)
#  define IMPORT __declspec(dllimport)
#else
#  define EXPORT __attribute__ ((visibility ("default")))
#  define IMPORT __attribute__ ((visibility ("default")))
#endif

typedef void (*PF)();

IMPORT PF imp();
EXPORT void exp() { imp()(); }
```
This code would generate a WebAssembly module with imports for:
* the function `imp`
* the table(s) used for indirectly-callable functions
* the linear memory used to implement the heap

and exports for:
* the function `exp`
* the imported table(s) and linear memory (so that other modules may
  import this one)

The host-specific loader can then simply connect the imports
and exports at instantantiation-time.

One extra detail is what
to use as the [module name](Modules.md#imports) for imports (since
WebAssembly has a two-level namespace). One option is to have a single default
module name for all C/C++ imports/exports (which then allows the toolchain to
put implementation-internal names in a separate module, avoiding the need for
`__`-prefix conventions).

To implement run-time dynamic linking (e.g., `dlopen` and `dlsym`):
* `dlopen` would compile and instantiate a new module, storing the compiled
  instance in a host-environment table, returning the index to the caller.
* `dlsym` would be given this index, pull the instance out of the table,
  search the instances's exports, append the found function to the function
  table (using host-defined functionality in the MVP, but directly from
  WebAssembly code in the
  [future](FutureFeatures.md#more-table-operators-and-types)) and return the
  table index of the appended element to the caller.

Note that the representation of a C function-pointer in WebAssembly is an index
into a function table, so the above scheme lines up perfectly with the
function-pointer return value of `dlsym`.

More complicated dynamic linking functionality (e.g., interposition, weak
symbols, etc) can be simulated efficiently by assigning a function table
index to each weak/mutable symbol, calling the symbol via `call_table` on that
index, and mutating the underlying element as needed.

After the MVP, we would like to standardize a single [ABI][] per source
language, allowing for WebAssembly libraries to interface with each other
regardless of compiler. Specifying an ABI requires that all ABI-related 
future features (like SIMD, multiple return values and exception handling)
have been implemented. While it is highly recommended for compilers targeting
WebAssembly to adhere to the specified ABI for interoperability, WebAssembly
runtimes will be ABI agnostic, so it will be possible to use a non-standard ABI
for specialized purposes.

  [ABI]: https://en.wikipedia.org/wiki/Application_binary_interface

TODO:

The [data](Modules.md#data-section) and [elements](Modules.md#elements-section)
segments currently have fixed integer offsets which is fundamentally
incompatible with general dynamic loading. Without extension, dynamic linking
implementations would either need to statically allocate global data and
elements (which isn't possible in the general case) or use an entirely different
(and less efficient) scheme that doesn't use these sections.

A better fix (before or after the MVP) is to allow importing/exporting constant
primitive values that can be used as the offsets of data/element segments
and generally read in WebAssembly code through some new `get_const` operator.
This would essentially be equivalent to the 
[Global Offset Table](https://en.wikipedia.org/wiki/Position-independent_code)
used in native implementations of position-independent code although an
engine could use direct code patching of the values as well.
