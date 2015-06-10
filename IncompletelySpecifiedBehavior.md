# Incompletely Specified Behavior

WebAssembly is a [portable](Portability.md) sandboxed platform. Applications
can't access data outside the sandbox without going through appropriate APIs, or
otherwise escape the sandbox, even if the behavior inside the sandbox should
ever be unspecified in any way.

WebAssembly always maintains valid callstacks. Return addresses are stored on the trusted stack and can't be clobbered by the application. And, WebAssembly ensures that calls and branches always have valid destinations.

Beyond that, WebAssembly minimizes observable differences between implementations, to reduce the risk of applications becoming dependent on any particular implementation's behavior. However, occasionally compromises are made due to performance concerns, listed below.

In particular, WebAssembly has no [nasal demons](https://en.wikipedia.org/w/index.php?title=Nasal_demons), since they are an extreme on the spectrum of observable differences, and since they make it difficult to reason about what state an application might be in. WebAssembly prefers to [trap](AstSemantics.md) when feasible, and otherwise it permits a specific set of possible conforming behaviors.

The following is a list of the places where the WebAssembly specification currently admits or is expected to admit multiple possible behaviors.

 - [Out of bounds heap accesses](AstSemantics.md#accessing-the-heap)

 - [Environment-dependent resource limits may be exhausted](AstSemantics.md)

 - [NaN bit patterns](AstSemantics.md#floating-point-operations)

 - [Races between threads](EssentialPostMVPFeatures.md#threads)

 - [Fixed-width SIMD may want some flexibility](EssentialPostMVPFeatures.md#fixed-width-simd)
   - In SIMD.js, floating point values may or may not have subnormals flushed to zero.
   - In SIMD.js, operations ending in "Approximation" return approximations that may vary between platforms.

## Note for users of C, C++, and similar languages

Some operations which have fully defined behavior in WebAssembly itself may nonetheless have undefined behavior at the source code level. For example, while unaligned memory access is fully defined in WebAssembly, C and C++ compilers make no guarantee that a (non-packed) unaligned memory access at the source level is harmlessly translated into an unaligned memory access in WebAssembly. And in practice, popular C and C++ compilers do optimize on the assumption that alignment rules are followed, meaning that they don't always preserve program behavior otherwise.

On WebAssembly, the primary invariants are always maintained. Demons can't actually fly out your nose, as that would constitute an escape from the sandbox. And, callstacks can't become corrupted.

Other than that, programs which invoke undefined behavior at the source language level may be compiled into WebAssembly programs which do anything else, including corrupting the contents of the application heap, calling APIs with arbitrary parameters, hanging, trapping, or consuming arbitrary amounts of resources (within the limits).

[Tools are being developed and ported](Tooling.md) to help developers find and fix bugs in their code.
