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
 * NaN bit patterns in floating point
   [operators](AstSemantics.md#floating-point-operators) and
   [conversions](AstSemantics.md#datatype-conversions-truncations-reinterpretations-promotions-and-demotions)
 * [Fixed-width SIMD may want some flexibility](PostMVP.md#fixed-width-simd)
   - In SIMD.js, floating point values may or may not have subnormals flushed to
     zero.
   - In SIMD.js, operators ending in "Approximation" return approximations that
     may vary between platforms.
 * Environment-dependent resource limits may be exhausted. A few examples:
   - Memory allocation may fail.
   - Program stack may get exhausted (e.g., because function call depth is too big,
     or functions have too many locals, or infinite recursion). Note that this stack
     isn't located in the program-accessible linear memory.
   - Resources such as handles may get exhausted.

Users of C, C++, and similar languages should be aware that operators which
have defined or constrained behavior in WebAssembly itself may nonetheless still
have undefined behavior
[at the source code level](CAndC++.md#undefined-behavior).
