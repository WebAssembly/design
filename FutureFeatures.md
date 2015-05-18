# Feature to add after v.1

These are features that make sense in the context of the
[high-level goals](HighLevelGoals.md) of WebAssembly but are not considered part
of the [Minimum Viable Product](V1.md) or the
[essential post-v.1 feature set](EssentialPostV1Features.md) which are expected
to be standardized immediately after v.1. These will be prioritized based on
developer feedback.

## Great tooling support
This is covered in the [tooling](Tooling.md) section.

## Dynamic linking
 * [Dynamic loading](V1.md#code-loading-and-imports) is in [v.1](V1.md), but all loaded modules have
   their own [separate heaps](V1.md#heap) and cannot share [function pointers](V1.md#function-pointers).
 * Support both load-time and run-time (`dlopen`) dynamic linking of both
   WebAssembly modules and non-WebAssembly modules (e.g., on the web, ES6
   ones containing JS), sharing the heap as well as function pointers.
 * TODO

## Finer-grained control over memory
 * `mmap` of File, `madvise(MADV_DONTNEED)`, ...
 * TODO

## More expressive control flow
 * Some types of control flow (esp. irreducible and indirect) cannot be
   expressed with maximum efficiency in WebAssembly without patterned output by
   the relooper and [jump-threading](http://en.wikipedia.org/wiki/Jump_threading)
   optimizations in the engine.
 * Options under consideration:
   * No action, while+switch and jump-threading are enough.
   * Just add goto (direct and indirect).
   * Add [signature-restricted Proper Tail Calls](FutureFeatures.md#signature-restricted-proper-tail-calls).
   * Add new control-flow primitives that address common patterns.

## GC/DOM Integration
 * Access to certain kinds of GC things from variables/arguments/expressions
 * Ability to GC-allocate certain kinds of GC things
 * Initially, things with fixed structure:
   * JS strings
   * JS functions (as callable closures)
   * Typed Arrays
   * [Typed objects](https://github.com/nikomatsakis/typed-objects-explainer/)
   * DOM objects via WebIDL
 * Perhaps a rooting API for safe reference from the linear address space
 * TODO

## Heaps bigger than 4GiB
* Allow heaps greater than 4GiB.
* Provide load/store operations that take 64-bit address operands; `int64` becomes the
  canonical pointer type.
* On a 32-bit system, heaps must still be <4GiB so all the int64 arithmetic (which will be much
  slower than 32-bit arithmetic) will be unnecessary.
  * Show we provide a uintptr_t (only 64-bit on 64-bit systems)?
    * This feature alone would not allow a C++ compiler to write size-polymorphic code since the word
      size is also baked into the code in a hundred other ways (consider `offsetof`).
    * The compiler *could* inflate all pointer types that are used in heap storage to 64-bit (so the
      uintptr_t type was only used for local variable/expression types).
      * This would imply an implicit truncation of any load of a pointer from the heap which could cause
        subtle bugs if the pointer was storing a real int64-width value.
      * This would still unnecessarily increase heap size on 32-bit; applications sensitive to OOM would
        still want a separate 32-bit build.
      * Now there are three compile targets: all-32, all-64, and this uintptr_t hybrid.
    * More discussion and experimentation needed.
      * Would the hybrid mostly Just Work?
      * Are there users who would want to ship a hybrid build instead of two 32- and 64-bit builds
        (conditionally loaded after a feature test)?

## Source maps integration
 * Add a new source maps [module section type](V1.md#module-structure).
 * Either embed the source maps directly or just a URL from which source maps can be downloaded.
 * Text source maps become intractably large for even moderate-sized compiled codes, so probably
   need to define new binary format for source maps.

## Signature-restricted Proper Tail Calls
* See the [asm.js RFC](http://discourse.specifiction.org/t/request-for-comments-add-a-restricted-subset-of-proper-tail-calls-to-asm-js).
* Useful properties of signature-restricted PTCs:
  * In most cases, can be compiled to a single jump.
  * Can express indirect `goto` via function-pointer calls.
  * Can be used as a compile target for languages with unrestricted PTCs;
    the code generator can use a stack in the heap to effectively implement a
    custom call ABI on top of signature-restricted PTCs.
  * An engine that wishes to perform aggressive optimization can fuse a graph of PTCs into a
    single function.
  * To reduce compile time, a code generator can use PTCs to break up
    ultra-large functions into smaller functions at low overhead using PTCs.
  * A compiler can exert some amount of control over register allocation via the ordering of
    arguments in the PTC signature.
 
## Proper Tail Calls
 * Expands upon Signature-restricted Proper Tail Calls.
 * TODO
 
## Asynchronous Signals
 * TODO

## "Long SIMD"
* The initial SIMD API will be a "short SIMD" API, centered around fixed-width
  128-bit types and explicit SIMD operations. This is quite portable and useful,
  but it won't be able to deliver the full performance capabilities of some of
  today's popular hardware. There is an opportunity for someone to propose a
  "long SIMD" model which will generalize to wider hardware vector lengths make
  more natural use of advanced features like vector lane predication,
  gather/scatter, and so on. Interesting questions to ask of such an model will
  include:
    * How will this model map onto popular modern SIMD hardware architectures?
    * What is this model's relationship to other hardware parallelism features,
      such as GPUs and threads with shared memory?
    * How will this model be used from higher-level programming languages?
      For example, the C++ committee is considering a wide variety of possible
      approaches; which of them might be supported by the model?
    * What is the relationship to the "short SIMD" API? "None" may be an
      acceptable answer, but it's something to think about.
    * What non-determinism does this model introduce into the overall platform?
    * What happens when code uses long SIMD on a hardware platform which doesn't
      support it? Reasonable options may include emulating it without the
      benefit of hardware acceleration, or indicating a lack of support through
      feature tests.
 
## Operations which may not be available or may not perform well on all platforms
 * Fused multiply-add.
 * Reciprocal square root approximate.
 * 16-bit floating point.
 * and more!

## Platform-independent Just-in-Time compilation
* Minimally, we need mechanisms to make this possible.
  * Producing a dynamic library and loading it is very likely the first step, as
    it will be easy to get working.

  * After that, it may become desirable to define lighter-weight mechanisms, such
    as the ability to add a function to an existing module, or even the ability to
    define explicitly patchable constructs within functions to allow for very
    fine-grained JITing.

* Potential enhancements include:
  * Provide JITs access to profile feedback for their JITed code.

## Multiprocess support
 * `vfork`.
 * Inter-process communication.
 * Inter-process `mmap`.

## Trapping or non-trapping strategies.
* Presently, when an instruction traps, the program is immediately terminated.
  This suits C/C++ code, where trapping conditions indicate Undefined Behavior
  at the source level, and it's also nice for handwritten code, where trapping
  conditions typically indicate an instruction being asked to perform outside
  its supported range. However, the current facilities do not cover some
  interesting use cases:

  * Not all likely-bug conditions are covered. For example, it would be very
    nice to have a signed-integer add which traps on overflow. Such a construct
    would add too much overhead on today's popular hardware architectures to be
    used in general, however it may still be useful in some contexts.

  * Some higher-level languages define their own semantics for conditions like
    division by zero and so on. It's possible for compilers to add explicit
    checks and handle such cases manually, though more direct support from the
    platform could have advantages:
    * Non-trapping versions of some opcodes, such as an integer division
      instruction that returns zero instead of trapping on division by zero,
      could potentially run faster on some platforms.
    * The ability to recover gracefully from traps in some way could make many
      things possible. Possibly this could involve throwing or possibly by
      resuming execution at the trapping instruction with the execution state
      altered, if there can be a reasonable way to specify how that should work.
