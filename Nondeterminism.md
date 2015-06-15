# Nondeterminism in WebAssembly

WebAssembly is a [portable](Portability.md) sandboxed platform with limited,
local, nondeterminism. 
  * *Limited*: nondeterministic execution can only occur in a small number of
    well-defined cases (described below) and, in those cases, the implementation
    may select from a limited set of possible behaviors.
  * *Local*: when nondeterministic execution occurs, the effect is local,
    there is no "spooky action at a distance".

The limited, local, nondeterministic model implies:
  * Applications can't access data outside the sandbox without going through
    appropriate APIs, or otherwise escape the sandbox.
  * WebAssembly always maintains valid, trusted callstacks; stray pointer writes
    cannot corrupt return addresses or spilled variables on the stack.
  * Calls and branches always have valid destinations ensuring 
    [Control Flow Integrity](https://research.microsoft.com/apps/pubs/default.aspx?id=64250).
  * WebAssembly has no [nasal demons](https://en.wikipedia.org/w/index.php?title=Nasal_demons).

Ideally, WebAssembly would be fully deterministic (except where nondeterminism
was essential to the API, like random number generators, date/time functions or
input events). Nondeterminism is only specified as a compromise when there is no
other practical way to achieve [portable](Portability.md) native performance.

The following is a list of the places where the WebAssembly specification
currently admits nondeterminism:

 * [When threads are added as a feature](PostMVP.md#threads), even without
   shared memory, nondeterminism will be visible through the global sequence of
   API calls. With shared memory, the result of load operations is
   nondeterministic.
 * Out of bounds heap accesses *may* want
   [some flexibility](AstSemantics.md#out-of-bounds)
 * [NaN bit patterns](AstSemantics.md#floating-point-operations)
 * [Fixed-width SIMD may want some flexibility](PostMVP.md#fixed-width-simd)
   - In SIMD.js, floating point values may or may not have subnormals flushed to
     zero.
   - In SIMD.js, operations ending in "Approximation" return approximations that
     may vary between platforms.
 * Environment-dependent resource limits may be exhausted. A few examples:
   - Memory allocation may fail.
   - Program stack may get exhausted.
   - Resources such as handles may get exahusted.

Users of C, C++, and similar languages should be aware that operations which
have defined or constrained behavior in WebAssembly itself may nonetheless still
have undefined behavior
[at the source code level](CAndC++.md#undefined-behavior).
