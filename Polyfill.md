# Polyfill to JavaScript

Even before browsers ship native support for WebAssembly, developers can ship
applications on the Web using a [polyfill][] which converts WebAssembly to
JavaScript. Aside from packaging the application in a forward-looking way—so
that when browsers do support WebAssembly natively, the application will
immediately benefit—this can provide additional value in other ways:

* Small download size due to the [binary encoding](BinaryEncoding.md);
* Minor impact on startup performance (decoding the binary into JavaScript is
  fast);
* The same high throughput as in existing approaches of compiling C/C++
  applications to browsers such as [Emscripten][], [asm.js][] and [PNaCl][].

This polyfill further allows us to experiment on the early binary encoding and
get developer feedback before finalizing the format and supporting it natively
as part of [the MVP](MVP.md).

A working prototype to unpack a tentative binary format into JavaScript, as well
as to convert [asm.js][] into WebAssembly (useful for existing applications), is
in the [polyfill repo][].  We also leave open the possibility of multiple
polyfills existing to meet different developers' needs.

An **effective** polyfill should reuse much of the Web platform's existing
capabilities, as detailed in the browser embedding
[implementation details](Web.md#implementation-details).

  [polyfill]: https://remysharp.com/2010/10/08/what-is-a-polyfill
  [Emscripten]: http://emscripten.org
  [asm.js]: http://asmjs.org
  [PNaCl]: http://gonacl.com
  [polyfill repo]: https://github.com/WebAssembly/polyfill-prototype-1

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

* **[Misaligned heap access](AstSemantics.md#alignment)**: Since misaligned
  loads/stores are guaranteed to produce correct results and heap accesses in
  asm.js force alignment (e.g., `HEAP32[i>>2]` masks off the low two bits), an
  asm.js polyfill would need to translate *all* loads/stores into byte accesses
  (regardless of specified alignment) to be correct. To achieve competitive
  performance, the polyfill prototype defaults to incorrect behavior by emitting
  full-size accesses as if the index was never misaligned. Providing correct
  alignment information is important for portable WebAssembly performance in
  general; that information also guarantees that the polyfill is both correct
  and fast.
* **[Out of bounds heap access](AstSemantics.md#out-of-bounds)**: Regardless of
  WebAssembly behavior, an asm.js polyfill will follow standard asm.js behavior:
  - Out of bound stores are ignored (treated as no-op);
  - Out of bound loads return zero for integer loads or NaN for floating point.
* **[32-bit integer operators](AstSemantics.md#32-bit-integer-operators)**:
  Regardless of WebAssembly behavior, an asm.js polyfill will follow its
  standard behavior:
  - Division by zero returns zero;
  - `INT32_MIN / -1` returns `INT32_MIN`;
  - Shift counts are implicitly masked.
* **[Datatype conversions](AstSemantics.md#datatype-conversions-truncations-reinterpretations-promotions-and-demotions)**:
  Regardless of WebAssembly behavior, an asm.js polyfill will follow its
  standard behavior:
  - Return zero when conversion from floating point to integer fails;
  - Optionally canonicalize NaN values.

## Polyfill Evolution

The MVP feature set is expected to be entirely polyfillable effectively and
efficiently. As WebAssembly evolves past MVP the working group will strive to:

* Standardize features which can be polyfilled;
* Co-evolve with other web standards bodies, ensuring that upcoming WebAssembly
  features remain polyfillable.

There is, however, no hard requirement that WebAssembly features be polyfillable
past the MVP. If a feature is highly desirable but cannot be effectively or
efficiently polyfilled then WebAssembly may adopt the feature. This is not a
decision that will be taken lightly, and the WebAssembly working group will
attempt to make it possible for developers to avoid the feature or provide a
fallback through [feature detection](FeatureTest.md).
