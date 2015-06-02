# Incompletely Specified Behavior

WebAssembly is a sandboxed platform. Applications can't access data outside the sandbox without going through appropriate APIs, or otherwise escape the sandbox, even if the behavior inside the sandbox should ever be unspecified in any way.

Beyond that, it is desirable to minimize observable differences between implementations when practical, so that applications don't inadvertently become dependent on any particular implementation's behavior. However, in some areas, performance concerns make this prohibitively expensive, so some compromises are made.

Also, there is a general preference against [nasal demons](https://en.wikipedia.org/w/index.php?title=Nasal_demons), since they are an extreme on the spectrum of observable differences, and since they make it difficult to reason about what state an application might be in. WebAssembly usually prefers to [trap](AstSemantics.md). Otherwise, it is generally preferred to have a specific set of possible behaviors rather than nasal demons. And presently, WebAssembly has no nasal demons.

The following is a list of the places where the WebAssembly specification currently admits or is expected to admit multiple possible behaviors.

 - [Out of bounds heap accesses](AstSemantics.md#accessing-the-heap)

 - [Environment-dependent resource limits may be exhausted](AstSemantics.md)

 - [NaN bit patterns](AstSemantics.md#floating-point-operations)

 - [Races between threads](EssentialPostV1Features.md#threads)

 - [Fixed-width SIMD may want some flexibility](EssentialPostV1Features.md#fixed-width-simd)
   - In SIMD.js, floating-point values may or may not have subnormals flushed to zero.
   - In SIMD.js, operations ending in "Approximation" return approximations that may vary between platforms.
