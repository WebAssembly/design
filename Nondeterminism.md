# Nondeterminism in WebAssembly

WebAssembly is a [portable](Portability.md) sandboxed platform with limited,
local, nondeterminism. 
  * *Limited*: nondeterministic execution can only occur in a small number of
    well-defined cases (described below) and, in those cases, the implementation
    may select from a limited set of possible behaviors.
  * *Local*: when nondeterministic execution occurs, the effect is local,
    there is no "spooky action at a distance".

The [rationale](Rationale.md) document details why WebAssembly is designed as
detailed in this document.

The limited, local, nondeterministic model implies:
  * Applications can't access data outside the sandbox without going through
    appropriate APIs, or otherwise escape the sandbox.
  * WebAssembly always maintains valid, trusted callstacks; stray pointer writes
    cannot corrupt return addresses or spilled variables on the stack.
  * Calls and branches always have valid destinations ensuring 
    [Control Flow Integrity](https://research.microsoft.com/apps/pubs/default.aspx?id=64250).
  * WebAssembly has no [nasal demons](https://en.wikipedia.org/w/index.php?title=Nasal_demons).

The following is a list of the places where the WebAssembly specification
currently admits nondeterminism:

 * New features will be added to WebAssembly, which means different implementations
   will have different support for each feature. This can be detected with
   `has_feature`, but is still a source of differences between executions.
 * [When threads are added as a feature](PostMVP.md#threads), even without
   shared memory, nondeterminism will be visible through the global sequence of
   API calls. With shared memory, the result of load operators is
   nondeterministic.
 * Except when otherwise specified, when an arithmetic operator returns NaN,
   there is nondeterminism in determining the specific bits of the NaN. However,
   wasm does still provide the guarantee that NaN values returned from an operation
   will not have 1 bits in their fraction field that aren't set in any NaN values
   in the input operands, except for the most significant bit of the fraction field
   (which most operators set to 1).
 * Except when otherwise specified, when an arithmetic operator with a floating
   point result type receives no NaN input values and produces a NaN result
   value, the sign bit of the NaN result value is nondeterministic.
 * [Fixed-width SIMD may want some flexibility](PostMVP.md#fixed-width-simd)
   - In SIMD.js, floating point values may or may not have subnormals flushed to
     zero.
   - In SIMD.js, operators ending in "Approximation" return approximations that
     may vary between platforms.
 * Environment-dependent resource limits may be exhausted. A few examples:
   - Memory allocation may fail.
   - The runtime can fail to allocate a physical page when a memory location is first
     accessed (e.g. through a load or store), even if that memory was virtually reserved
     by the maximum size property of the [memory section](Modules.md#linear-memory-section).
   - Program stack may get exhausted (e.g., because function call depth is too big,
     or functions have too many locals, or infinite recursion). Note that this stack
     isn't located in the program-accessible linear memory.
   - Resources such as handles may get exhausted.
   - Any other resource could get exhausted at any time. Caveat emptor.

Users of C, C++, and similar languages should be aware that operators which
have defined or constrained behavior in WebAssembly itself may nonetheless still
have undefined behavior
[at the source code level](CAndC++.md#undefined-behavior).
