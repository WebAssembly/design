# Nondeterminism in WebAssembly

WebAssembly is a [portable](Portability.md) [sandboxed](Security.md) platform
with limited, local, nondeterminism.

  * *Limited*: nondeterministic execution can only occur in a small number of
    well-defined cases (described below) and, in those cases, the implementation
    may select from a limited set of possible behaviors.
  * *Local*: when nondeterministic execution occurs, the effect is local,
    there is no "spooky action at a distance".

The [rationale](Rationale.md) document details why WebAssembly is designed as
detailed in this document.

The following is a list of the places where the WebAssembly specification
currently admits nondeterminism:

 * New features will be added to WebAssembly, which means different implementations
   will have different support for each feature.
 * The sequence of calls of exported functions, and the values of the incoming
   arguments and return values from the outside environment, are not
   determined by the Wasm spec.
 * With `shared` memory that can be accessed by multiple threads, the results of
   load, read-modify-write, wait, and awake operators are nondeterministic.
 * Except when otherwise specified, when an arithmetic operator returns NaN,
   there is nondeterminism in determining the specific bits of the NaN. However,
   wasm does still provide the guarantee that NaN values returned from an operation
   will not have 1 bits in their fraction field that aren't set in any NaN values
   in the input operands, except for the most significant bit of the fraction field
   (which most operators set to 1).
 * Except when otherwise specified, when an arithmetic operator with a floating
   point result type receives no NaN input values and produces a NaN result
   value, the sign bit of the NaN result value is nondeterministic.
 * The [relaxed SIMD] instructions have nondeterministic results.
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

The following proposals would add additional nondeterminism:

 * [Flexible-vectors], adding nondeterminism in the length of the vectors.

Users of C, C++, and similar languages should be aware that operators which
have defined or constrained behavior in WebAssembly itself may nonetheless still
have undefined behavior
[at the source code level](CAndC++.md#undefined-behavior).

[relaxed SIMD]: https://github.com/WebAssembly/relaxed-simd
[Flexible-vectors]: https://github.com/WebAssembly/flexible-vectors
