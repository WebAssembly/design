# Polyfill to JavaScript

Even before browsers ship native support for WebAssembly, developers can
ship applications on the Web using
a [polyfill](https://remysharp.com/2010/10/08/what-is-a-polyfill) which
converts WebAssembly to JavaScript. Aside from packaging the application in
a forward-looking way — so that when browsers do support WebAssembly natively,
the application will immediately benefit — this can provide additional
value in other ways:

* Small download size due to the [binary encoding](BinaryEncoding.md);
* Minor impact on startup performance (decoding the binary into JavaScript is fast);
* The same high throughput as in existing approaches of compiling C and C++ to JavaScript such as [Emscripten](http://emscripten.org) and [asm.js](http://asmjs.org).

This polyfill further allows us to experiment on the early binary encoding and
get developer feedback before finalizing the format and supporting it natively
as part of [the MVP](MVP.md).

A working prototype to unpack the WebAssembly binary format into JavaScript,
as well as to convert [asm.js](http://asmjs.org) into WebAssembly (useful
for existing applications), is in the [polyfill repo](https://github.com/WebAssembly/polyfill).
We also leave open the possibility of multiple polyfills existing to meet different developers' needs.

## Implementation Details

An **effective** polyfill should reuse much of the Web platform's existing
capabilities.

We've identified interesting polyfill implementation approaches which help
convince us that the design, especially that of the MVP, are sensible:

* A [module](MVP.md#Modules) can be loaded in the same way as an ES6 module
  (`import` statements, `Reflect` API, `Worker` constructor, etc) and the result
  is reflected to JS as an ES6 module object.
  * Exports are the ES6 module object exports.
  * An import first passes the module name to the
    [module loader pipeline](http://whatwg.github.io/loader) and resulting ES6
    module (which could be implemented in JS or WebAssembly) is queried for the
    export name.
  * There is no special case for when one WebAssembly module imports another:
    they have separate [heaps](MVP.md#heap) and pointers cannot be passed
    between the two. Module imports encapsulate the importer and importee.
  * To synchronously call into JavaScript from C++, the C++ code would declare
    and call an undefined `extern` function and the target JavaScript function
    would be given the (mangled) name of the `extern` and put inside the
    imported ES6 module.


## Polyfill Deviations

An **efficient** polyfill may purposely diverge from the specified WebAssembly
semantics: a polyfill doesn't need to be 100% correct with respect to the
WebAssembly specification to be useful in practice. There are corner cases
(often undefined behavior in C/C++) where JavaScript and asm.js don't have ideal
semantics to maintain correctness efficiently.

If needed, a polyfill could provide an option to ensure full correctness at the
expense of performance, though this is not expected to be necessary for portable
C/C++ code.

Some divergences that we've identified as potentially desirable:

* [Misaligned heap access](AstSemantics.md#alignment)
  * Since misaligned loads/stores are guaranteed to produce correct results and
    heap accesses in asm.js force alignment (e.g., `HEAP32[i>>2]` masks off the
    low two bits), the asm.js polyfill would need to translate *all*
    loads/stores into byte accesses (regardless of specified alignment) to be
    correct. To achieve competitive performance, the polyfill defaults
    to incorrect behavior by emitting full-size accesses as if the index was
    never misaligned. Providing correct alignment information is important for
    portable WebAssembly performance in general; that information also
    guarantees that the polyfill is both correct and fast.
* [Out of bounds heap access](AstSemantics.md#out-of-bounds)
  * Regardless of semantics chosen for out of bounds access in WebAssembly, the
    polyfill will follow standard asm.js behavior:
    * Out of bound stores are ignored (treated as no-op).
    * Out of bound loads return 0 for integer loads or NaN for floating point.
* [32-bit Integer operations](AstSemantics.md#32-bit-integer-operations)
  * Regardless of WebAssembly behavior, the asm.js polyfill will follow its
    standard behavior:
    * Division by zero returns zero.
    * INT32_MIN/-1 returns INT32_MIN.
    * Shift counts are implicitly masked.
* [Datatype Conversions](AstSemantics.md#datatype-conversions-truncations-reinterpretations-promotions-and-demotions)
  * Regardless of WebAssembly behavior, the asm.js polyfill will follow its
    standard behavior:
    * Return zero when conversion from floating point to integer fails.
    * Optionally canonicalize NaN values.
