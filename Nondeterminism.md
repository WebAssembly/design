# Nondeterminism in WebAssembly

WebAssembly is a [portable](Portability.md) sandboxed platform with limited,
local, nondeterminism. 
  * *Limited*: non-deterministic execution can only occur in a small number of
    well-defined cases (described below) and, in those cases, the implementation
    may select from a limited set of possible behaviors.
  * *Local*: when non-deterministic execution occurs, the effect is local,
    there is no "spooky action at a distance".

The limited, local, non-deterministic model implies:
  * Applications can't access data outside the sandbox without going through
    appropriate APIs, or otherwise escape the sandbox.
  * WebAssembly always maintains valid, trusted callstacks; stray pointer writes
    cannot corrupt return addresses or spilled variables on the stack.
  * Calls and branches always have valid destinations ensuring 
    [Control Flow Integrity](http://research.microsoft.com/apps/pubs/default.aspx?id=64250).
  * WebAssembly has no [nasal demons](https://en.wikipedia.org/w/index.php?title=Nasal_demons).

Ideally, WebAssembly would be fully deterministic (except where nondeterminism
was essential to the API, like random number generators, date/time functions or
input events). Nondeterminism is only specified as a compromise when there is no
other practical way to achieve [portable](Portability.md) native performance.

The following is a list of the places where the WebAssembly specification
currently admits nondeterminism:

 - [No sequential consistency guarantee for programs which contain races](EssentialPostMVPFeatures.md#threads)

 - [Out of bounds heap accesses may want some flexibility](AstSemantics.md#out-of-bounds)

 - [NaN bit patterns](AstSemantics.md#floating-point-operations)

 - [Fixed-width SIMD may want some flexibility](EssentialPostMVPFeatures.md#fixed-width-simd)
   - In SIMD.js, floating point values may or may not have subnormals flushed to zero.
   - In SIMD.js, operations ending in "Approximation" return approximations that may vary between platforms.

 - Environment-dependent resource limits may be exhausted.

## Note for users of C, C++, and similar languages

Some operations which have fully defined behavior in WebAssembly itself may nonetheless have undefined behavior at the source code level. For example, while unaligned memory access is fully defined in WebAssembly, C and C++ compilers make no guarantee that a (non-packed) unaligned memory access at the source level is harmlessly translated into an unaligned memory access in WebAssembly. And in practice, popular C and C++ compilers do optimize on the assumption that alignment rules are followed, meaning that they don't always preserve program behavior otherwise.

On WebAssembly, the primary invariants are always maintained. Demons can't actually fly out your nose, as that would constitute an escape from the sandbox. And, callstacks can't become corrupted.

Other than that, programs which invoke undefined behavior at the source language level may be compiled into WebAssembly programs which do anything else, including corrupting the contents of the application heap, calling APIs with arbitrary parameters, hanging, trapping, or consuming arbitrary amounts of resources (within the limits).

[Tools are being developed and ported](Tooling.md) to help developers find and fix bugs in their code.
