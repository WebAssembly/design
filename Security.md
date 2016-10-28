# Security

The security model of WebAssembly has two important goals: (1) protect *users*
from buggy or malicious modules, and (2) provide *developers* with useful
primitives and mitigations for developing safe applications, within the
constraints of (1).

## Users

Each WebAssembly module executes within a sandboxed environment separated from
the host runtime using fault isolation techniques. This implies:

  * Applications execute independently, and can't escape the sandbox without
  going through appropriate APIs.
  * Applications generally execute deterministically
  [with limited exceptions](Nondeterminism.md).

Additionally, each module is subject to the security policies of its embedding.
Within a [web browser](Web.md), this includes restrictions on information flow
through [same-origin policy][]. On a [non-web](NonWeb.md) platform, this could
include the POSIX security model.

## Developers

The design of WebAssembly promotes safe programs by eliminating dangerous
features from its execution semantics, while maintaining compatibility with
programs written for [C/C++](CandC++.md).

Modules must declare all accessible functions and their associated types
at load time, even when [dynamic linking](DynamicLinking.md) is used. This
allows implicit enforcement of [control-flow integrity][] (CFI) through
structured control-flow. Since compiled code is immutable and not observable at
runtime, WebAssembly programs are protected from control flow hijacking attacks.

  * [Function calls](Semantics.md#calls) must specify the index of a target
  that corresponds to a valid entry in the
  [function index space](Modules.md#function-index-space) or
  [table index space](Modules.md#table-index-space).
  * [Indirect function calls](Rationale.md#indirect-calls) are subject to a type
  signature check at runtime; the type signature of the selected indirect
  function must match the type signature specified at the call site.
  * A shadow stack is used to maintain a trusted call stack that is invulnerable
  to buffer overflows in the module heap, ensuring safe function returns.
  * [Branches](Semantics.md#branches-and-nesting) must point to valid
  destinations within the enclosing function.

Variables in C/C++ can be lowered to two different primitives in WebAssembly,
depending on their scope. [Local variables](Semantics.md#local-variables)
with fixed scope and [global variables](Semantics.md#global-variables) are
represented as fixed-type values stored by index. The former are initialized
to zero by default and are stored in the protected shadow stack, whereas
the latter are located in the [global index space](Modules.md#global-index-space)
and can be imported from external modules. Local variables with
[unclear static scope](Rationale.md#locals) (e.g. are used by the address-of
operator, or are of type `struct` and returned by value) are stored in a separate
user-addressable stack in [linear memory](Semantics.md#linear-memory) at
compile time. This is an isolated memory region with fixed maximum size that is
zero initialized by default. References to this memory are computed with
infinite precision to avoid wrapping and simplify bounds checking. In the future,
support for [multiple linear memory sections](Modules.md#linear-memory-section) and
[finer-grained memory operations](FutureFeatures.md#finer-grained-control-over-memory)
(e.g. shared memory, page protection, large pages, etc.) will be implemented.

[Traps](Semantics.md#traps) are used to immediately terminate execution and
signal abnormal behavior to the execution environment. In a browser, this is
represented as a JavaScript exception. Support for
[module-defined trap handlers](FutureFeatures.md#trappingor-non-trapping-strategies)
will be implemented in the future. Operations that can trap include:

  * specifying an invalid index in any index space,
  * performing an indirect function call with a mismatched signature,
  * exceeding the maximum size of the protected call stack,
  * accessing [out-of-bounds](#Rationale.md#out-of-bounds) addresses in linear
  memory,
  * executing an illegal arithmetic operations (e.g. division or remainder by
  zero, signed division overflow, etc).

### Memory Safety
Compared to traditional C/C++ programs, these semantics obviate certain classes
of memory safety bugs in WebAssembly. Buffer overflows, which occur when data
exceeds the boundaries of an object and accesses adjacent memory regions, cannot
affect local or global variables stored in index space, they are fixed-size and
addressed by index. Data stored in linear memory can overwrite adjacent objects,
since bounds checking is performed at linear memory region granularity and is
not context-sensitive. However, the presence of control-flow integrity and
protected shadow call stacks prevents direct code injection attacks. Thus,
common mitigations such as [data execution prevention][] (DEP) and
[stack smashing protection][] (SSP) are not needed by WebAssembly programs.

Another common class of memory safety errors involves unsafe pointer usage and
[undefined behavior](CandC++.md#undefined-behavior). This includes dereferencing
pointers to unallocated memory (e.g. `NULL`), or freed memory allocations. In
WebAssembly, the semantics of pointers have been eliminated for function calls
and variables with fixed static scope, allowing references to invalid indexes in
any index space to trigger a validation error at load time, or at worst a trap
at runtime. Accesses to linear memory are bounds-checked at the region level,
potentially resulting in a trap at runtime. These memory region(s) are isolated
from the internal memory of the runtime, and are set to zero by default unless
otherwise initialized.

Nevertheless, other classes of bugs are not obviated by the semantics of
WebAssembly. Although attackers cannot perform direct code injection attacks,
it is possible to hijack the control flow of a module using code reuse attacks
against indirect calls. However, conventional [return-oriented programming][]
(ROP) attacks using short sequences of instructions ("gadgets") are not possible
in WebAssembly, because control-flow integrity ensures that call targets are
valid functions declared at load time. Likewise, race conditions, such as
[time of check to time of use][] (TOCTOU) vulnerabilities, are possible in
WebAssembly, since no execution or scheduling guarantees are provided beyond
in-order execution and [post-MVP atomic memory primitives
:unicorn:][future threads].
Similarly, [side channel attacks][] can occur, such as timing attacks against
modules. In the future, additional protections may be provided by runtimes or
the toolchain, such as code diversification or memory randomization (similar to
[address space layout randomization][] (ASLR)), [bounded pointers][] ("fat"
pointers), or finer-grained control-flow integrity.

  [address space layout randomization]: https://en.wikipedia.org/wiki/Address_space_layout_randomization
  [bounded pointers]: https://en.wikipedia.org/wiki/Bounded_pointer
  [control-flow integrity]: https://research.microsoft.com/apps/pubs/default.aspx?id=64250
  [data execution prevention]: https://en.wikipedia.org/wiki/Executable_space_protection
  [return-oriented programming]: https://en.wikipedia.org/wiki/Return-oriented_programming
  [same-origin policy]: https://www.w3.org/Security/wiki/Same_Origin_Policy
  [side channel attacks]: https://en.wikipedia.org/wiki/Side-channel_attack
  [stack smashing protection]: https://en.wikipedia.org/wiki/Buffer_overflow_protection#Random_canaries
  [time of check to time of use]: https://en.wikipedia.org/wiki/Time_of_check_to_time_of_use

[future threads]: FutureFeatures.md#threads
