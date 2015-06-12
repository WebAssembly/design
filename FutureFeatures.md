# Feature to add after the MVP

These are features that make sense in the context of the
[high-level goals](HighLevelGoals.md) of WebAssembly but are not considered part
of the [Minimum Viable Product](MVP.md) or the essential [post-MVP](PostMVP.md)
feature set which are expected to be standardized immediately after the
MVP. These will be prioritized based on developer feedback, and will be
available under [feature tests](FeatureTest.md).

## Great tooling support

This is covered in the [tooling](Tooling.md) section.

## Dynamic linking

[Dynamic loading](MVP.md#code-loading-and-imports) is in [the MVP](MVP.md), but
all loaded modules have their own [separate heaps](MVP.md#heap) and cannot share
[function pointers](MVP.md#function-pointers). Dynamic linking will allow
developers to share heaps and function pointers between WebAssembly modules.

WebAssembly will support both load-time and run-time (`dlopen`) dynamic linking
of both WebAssembly modules and non-WebAssembly modules (e.g., on the Web, ES6
ones containing JavaScript).

Dynamic linking is especially useful when combined with a Content Distribution
Network (CDN) such as [hosted libraries][] because the library is only ever
downloaded and compiled once per user device. It can also allow for smaller
differential updates, which could be implemented in collaboration with
[service workers][].

Standardize a single [ABI][] per source language, allowing for WebAssembly
modules to interface with each other regardless of compiler. While it is highly
recommended for compilers targeting WebAssembly to adhere to the specified ABI
for interoperability, WebAssembly runtimes will be ABI agnostic, so it will be
possible to use a non-standard ABI for specialized purposes.

  [hosted libraries]: https://developers.google.com/speed/libraries/
  [service workers]: http://www.w3.org/TR/service-workers/
  [ABI]: http://en.wikipedia.org/wiki/Application_binary_interface

## Finer-grained control over memory

* `mmap` of files.
* `madvise(MADV_DONTNEED)`.
* Shared memory, where a physical address range is mapped to multiple physical
  pages in a single WebAssembly module as well as across modules.

## More expressive control flow

Some types of control flow (especially irreducible and indirect) cannot be
expressed with maximum efficiency in WebAssembly without patterned output by the
relooper and [jump-threading](http://en.wikipedia.org/wiki/Jump_threading)
optimizations in the engine.

Options under consideration:
* No action, `while` and `switch` combined with jump-threading are enough.
* Just add `goto` (direct and indirect).
* Add [signature-restricted Proper Tail Calls](FutureFeatures.md#signature-restricted-proper-tail-calls).
* Add new control-flow primitives that address common patterns.

## GC/DOM Integration

* Access to certain kinds of Garbage-Collected (GC) objects from variables,
  arguments, expressions.
* Ability to GC-allocate certain kinds of GC objects.
* Initially, things with fixed structure:
  * JavaScript strings;
  * JavaScript functions (as callable closures);
  * Typed Arrays;
  * [Typed objects](https://github.com/nikomatsakis/typed-objects-explainer/);
  * DOM objects via WebIDL.
* Perhaps a rooting API for safe reference from the linear address space.

## Heaps bigger than 4GiB

WebAssembly will eventually allow heaps greater than 4GiB by providing
load/store operations that take 64-bit address operands. Modules which opt-in to
this feature have `int64` as the canonical pointer type.

On a 32-bit system, heaps must still be smaller than 4GiB. A WebAssembly
implementation running on such a platform may restrict allocations to the lower
4GiB, and leave the two 32-bits untouched.

## Source maps integration

* Add a new source maps [module section type](MVP.md#module-structure).
* Either embed the source maps directly or just a URL from which source maps can
  be downloaded.
* Text source maps become intractably large for even moderate-sized compiled
  codes, so probably need to define new binary format for source maps.

## Coroutines

Coroutines will [eventually be part of C++][] and is already popular in other
programming languages that WebAssembly will support.

  [eventually be part of C++]: http://wg21.link/n4499

## Signature-restricted Proper Tail Calls

See the [asm.js RFC][] for a full description of signature-restricted Proper
Tail Calls (PTC).

Useful properties of signature-restricted PTCs:

* In most cases, can be compiled to a single jump.
* Can express indirect `goto` via function-pointer calls.
* Can be used as a compile target for languages with unrestricted PTCs; the code
  generator can use a stack in the heap to effectively implement a custom call
  ABI on top of signature-restricted PTCs.
* An engine that wishes to perform aggressive optimization can fuse a graph of
  PTCs into a single function.
* To reduce compile time, a code generator can use PTCs to break up ultra-large
  functions into smaller functions at low overhead using PTCs.
* A compiler can exert some amount of control over register allocation via the
  ordering of arguments in the PTC signature.

  [asm.js RFC]: http://discourse.specifiction.org/t/request-for-comments-add-a-restricted-subset-of-proper-tail-calls-to-asm-js

## Proper Tail Calls

Expands upon signature-restricted Proper Tail Calls, and makes it easier to
support other languages, especially functional programming languages.
 
## Asynchronous Signals

TODO

## "Long SIMD"

The initial SIMD API will be a "short SIMD" API, centered around fixed-width
128-bit types and explicit SIMD operations. This is quite portable and useful,
but it won't be able to deliver the full performance capabilities of some of
today's popular hardware. There is [a proposal in the SIMD.js repository][] for
a "long SIMD" model which generalizes to wider hardware vector lengths, making
more natural use of advanced features like vector lane predication,
gather/scatter, and so on. Interesting questions to ask of such an model will
include:

* How will this model map onto popular modern SIMD hardware architectures?
* What is this model's relationship to other hardware parallelism features, such
  as GPUs and threads with shared memory?
* How will this model be used from higher-level programming languages? For
  example, the C++ committee is considering a wide variety of possible
  approaches; which of them might be supported by the model?
* What is the relationship to the "short SIMD" API? "None" may be an acceptable
  answer, but it's something to think about.
* What non-determinism does this model introduce into the overall platform?
* What happens when code uses long SIMD on a hardware platform which doesn't
  support it? Reasonable options may include emulating it without the benefit of
  hardware acceleration, or indicating a lack of support through feature tests.

  [a proposal in the SIMD.js repository]: https://github.com/johnmccutchan/ecmascript_simd/issues/180

## Operations which may not be available or may not perform well on all platforms

* Fused multiply-add.
* Reciprocal square root approximate.
* 16-bit floating point.
* and more!

## Platform-independent Just-in-Time compilation

WebAssembly is a new virtual ISA, and as such applications won't be able to
simply reuse their existing JIT-compiler backends. Applications will instead
have to interface with WebAssembly's instructions as if they were a new ISA.

Applications expect a wide variety of JIT-compilation capabilities. WebAssembly
should support:

* Producing a dynamic library and loading it into the current WebAssembly
  module.
* Define lighter-weight mechanisms, such as the ability to add a function to an
  existing module.
* Support explicitly patchable constructs within functions to allow for very
  fine-grained JIT-compilation. This includes:
    * Code patching for polymorphic inline caching;
	* Call patching to chain JIT-compiled functions together;
	* Temporary halt-insertion within functions, to trap if a function start
      executing while a JIT-compiler's runtime is performing operations
      dangerous to that function.
* Provide JITs access to profile feedback for their JIT-compiled code.
* Code unloading capabilities, especially in the context of code garbage
  collection and defragmentation.

## Multiprocess support

* `vfork`.
* Inter-process communication.
* Inter-process `mmap`.

## Trapping or non-trapping strategies.

Presently, when an instruction traps, the program is immediately terminated.
This suits C/C++ code, where trapping conditions indicate Undefined Behavior at
the source level, and it's also nice for handwritten code, where trapping
conditions typically indicate an instruction being asked to perform outside its
supported range. However, the current facilities do not cover some interesting
use cases:

* Not all likely-bug conditions are covered. For example, it would be very nice
  to have a signed-integer add which traps on overflow. Such a construct would
  add too much overhead on today's popular hardware architectures to be used in
  general, however it may still be useful in some contexts.
* Some higher-level languages define their own semantics for conditions like
  division by zero and so on. It's possible for compilers to add explicit checks
  and handle such cases manually, though more direct support from the platform
  could have advantages:
  * Non-trapping versions of some opcodes, such as an integer division
    instruction that returns zero instead of trapping on division by zero, could
    potentially run faster on some platforms.
  * The ability to recover gracefully from traps in some way could make many
    things possible. Possibly this could involve throwing or possibly by
    resuming execution at the trapping instruction with the execution state
    altered, if there can be a reasonable way to specify how that should work.
