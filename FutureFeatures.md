# Feature to add after v.1

These are features that make sense in the context of the
[high-level goals](HighLevelGoals.md) of WebAssembly but are not considered part
of the [Minimum Viable Product](V1.md) or the
[essential post-v.1 feature set](EssentialPostV1Features.md) which are expected
to be standardized immediately after v.1. These will be prioritized based on
developer feedback.

## Dynamic loading
 * Both load-time and run-time.
 * TODO

## Finer-grained control over memory
 * `mmap` of File, `madvise(MADV_DONTNEED)`, ...
 * TODO
 
## GC/DOM Integration
 * Access to certain kinds of GC things from variables/arguments/expressions
 * Ability to GC-allocate certain kinds of GC things
 * Initially, things with fixed structure:
   * JS strings
   * JS functions (as callable closures)
   * [ES6 typed objects](https://github.com/nikomatsakis/typed-objects-explainer/)
   * DOM objects via WebIDL
 * Perhaps a rooting API for safe reference from the linear address space
 * TODO

## Support for Just-in-Time Compilation
 * Finer-grained dynamic linking
 * Safe patching (of immediates, branch targets, ...)
 * TODO
 
## Proper Tail Calls
 * Expands upon Signature-restricted Proper Tail Calls.
 * TODO
 
## Asynchronous Signals
 * TODO

## Non-fixed-width SIMD
 * TODO
 
## Operations which may not be available or may not perform well on all platforms
 * Fused multiply-add.
 * Reciprocal square root approximate.
 * 16-bit floating point.
 * and more!

## Platform-independent Just-in-Time compilation
* TODO
* Potentially through dynamic loading.
