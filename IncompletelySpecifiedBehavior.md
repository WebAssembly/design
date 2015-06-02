# Incompletely Specified Behavior

WebAssembly is a sandboxed platform. Applications can't access data outside the sandbox without going through appropriate APIs, or otherwise escape the sandbox, even if the behavior inside the sandbox should ever be unspecified in any way.

Beyond that, WebAssembly minimizes observable differences between implementations, to reduce the risk of applications becoming dependent on any particular implementation's behavior. However, occasionally compromises are made due to performance concerns, listed below.

In particular, WebAssembly has no [nasal demons](https://en.wikipedia.org/w/index.php?title=Nasal_demons), since they are an extreme on the spectrum of observable differences, and since they make it difficult to reason about what state an application might be in. WebAssembly prefers to [trap](AstSemantics.md) when feasible, and otherwise it permits a specific set of possible conforming behaviors.

The following is a list of the places where the WebAssembly specification currently admits or is expected to admit multiple possible behaviors.

 - [Out of bounds heap accesses](AstSemantics.md#accessing-the-heap)

 - [Environment-dependent resource limits may be exhausted](AstSemantics.md)

 - [NaN bit patterns](AstSemantics.md#floating-point-operations)

 - [Races between threads](EssentialPostMVPFeatures.md#threads)

 - [Fixed-width SIMD may want some flexibility](EssentialPostMVPFeatures.md#fixed-width-simd)
   - In SIMD.js, floating-point values may or may not have subnormals flushed to zero.
   - In SIMD.js, operations ending in "Approximation" return approximations that may vary between platforms.
