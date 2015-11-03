# Design Rationale

This document describes rationales for WebAssembly's design decisions, acting as
footnotes to the main design text, keeping the main specification easier to
read, and making it easier to revisit decisions later without having to plow
through all the issues and pull requests. This rationale document tries to list
how decisions were made, and where tradeoffs were made for the sake of language
ergonomics, portability, performance, security, and Getting Things Done.

WebAssembly was designed incrementally, with multiple implementations being
pursued concurrently. As the MVP stabilizes and we get experience from real-world
codebases, we'll revisit the alternatives listed below, reevaluate the tradeoffs
and update the [design](AstSemantics.md) before the MVP is finalized.


## Why AST?

Why not a stack-, register- or SSA-based bytecode?
* Trees allow a smaller binary encoding: [JSZap][], [Slim Binaries][].
* [Polyfill prototype][] shows simple and efficient translation to asm.js.

  [JSZap]: https://research.microsoft.com/en-us/projects/jszap/
  [Slim Binaries]: https://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.108.1711
  [Polyfill prototype]: https://github.com/WebAssembly/polyfill-prototype-1


## Basic Types Only

WebAssembly only represents [a few types](AstSemantics.md#Types).

* More complex types can be formed from these basic types. It's up to the source
  language compiler to express its own types in terms of the basic machine
  types. This allows WebAssembly to present itself as a virtual ISA, and lets
  compilers target it as they would any other ISA.
* These types are efficiently executed by all modern architectures.
* Smaller types (such as `i8` and `i16`) are usually no more efficient and in
  languages like C/C++ are only semantically meaningful for memory accesses
  since arithmetic get widened to `i32` or `i64`. Avoiding them at least for MVP
  makes it easier to implement a WebAssembly VM.
* Other types (such as `f16`, `i128`) aren't widely supported by existing
  hardware and can be supported by runtime libraries if developers wish to use
  them. Hardware support is sometimes uneven, e.g. some support load/store of
  `f16` only whereas other hardware also supports scalar arithmetic on `f16`,
  and yet other hardware only supports SIMD arithmetic on `f16`. They can be
  added to WebAssembly later without compromising MVP.
* More complex object types aren't semantically useful for MVP: WebAssembly
  seeks to provide the primitive building blocks upon which higher-level
  constructs can be built. They may become useful to support other languages,
  especially when considering [garbage collection](GC.md).


## Load/Store Addressing

Load/store instructions include an immediate offset used for
[addressing](AstSemantics.md#Addressing). This is intended to simplify folding
of offsets into complex address modes in hardware, and to simplify bounds
checking optimizations. It offloads some of the optimization work to the
compiler that targets WebAssembly, executing on the developer's machine, instead
of performing that work in the WebAssembly compiler on the user's machine.


## Alignment Hints

Load/store instructions contain
[alignment hints](AstSemantics.md#Alignment). This makes it easier to generate
efficient code on certain hardware architectures.

Either tooling or an explicit opt-in "debug mode" in the spec could allow
execution of a module in a mode that threw exceptions on misaligned access.
This mode would incur some runtime cost for branching on most platforms which is
why it isn't the specified default.


## Out of Bounds

The ideal semantics is for
[out-of-bounds accesses](AstSemantics.md#Out-of-Bounds) to trap, but the
implications are not yet fully clear.

There are several possible variations on this design being discussed and
experimented with. More measurement is required to understand the associated
tradeoffs.

  * After an out-of-bounds access, the instance can no longer execute code and
    any outstanding JavaScript [ArrayBuffer][] aliasing the linear memory are
    detached.
    * This would primarily allow hoisting bounds checks above effectful
      operators.
    * This can be viewed as a mild security measure under the assumption that
      while the sandbox is still ensuring safety, the instance's internal state
      is incoherent and further execution could lead to Bad Things (e.g., XSS
      attacks).
  * To allow for potentially more-efficient memory sandboxing, the semantics
    could allow for a nondeterministic choice between one of the following when
    an out-of-bounds access occurred.
    * The ideal trap semantics.
    * Loads return an unspecified value.
    * Stores are either ignored or store to an unspecified location in the
      linear memory.
    * Either tooling or an explicit opt-in "debug mode" in the spec should allow
      execution of a module in a mode that threw exceptions on out-of-bounds
      access.

  [ArrayBuffer]: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/ArrayBuffer


## Resizing

To allow efficient engines to employ virtual-memory based techniques for bounds
checking, memory sizes are required to be page-aligned.
For portability across a range of CPU architectures and operating systems,
WebAssembly defines a fixed page size.
Programs can depend on this fixed page size and still remain portable across all
WebAssembly engines.
64KiB represents the least common multiple of many platforms and CPUs.
In the future, WebAssembly may offer the ability to use larger page sizes on
some platforms for increased TLB efficiency.


## Linear memory disabled if no linear memory section

See [#107](https://github.com/WebAssembly/spec/pull/107).


## Control Flow

Structured control flow provides simple and size-efficient binary encoding and
compilation. Any control flow—even irreducible—can be transformed into structured
control flow with the
[Relooper](https://github.com/kripken/emscripten/raw/master/docs/paper.pdf)
[algorithm](http://dl.acm.org/citation.cfm?id=2048224&CFID=670868333&CFTOKEN=46181900),
with guaranteed low code size overhead, and typically minimal throughput
overhead (except for pathological cases of irreducible control
flow). Alternative approaches can generate reducible control flow via node
splitting, which can reduce throughput overhead, at the cost of increasing
code size (potentially very significantly in pathological cases).
Also,
[more expressive control flow constructs](FutureFeatures.md#more-expressive-control-flow)
may be added in the future.


## Locals

C/C++ makes it possible to take the address of a function's local values and
pass this pointer to callees or to other threads. Since WebAssembly's local
variables are outside the address space, C/C++ compilers implement address-taken
variables by creating a separate stack data structure within linear memory. This
stack is sometimes called the "aliased" stack, since it is used for variables
which may be pointed to by pointers.

This prevents WebAssembly from performing clever optimizations on the stack and
liveness of such variables, but this loss isn't expected to be
consequential. Common C compiler optimizations such as LLVM's global value
numbering effectively split address-taken variables into parts, shrinking the
range where they actually need to have their address taken, and creating new
ranges where they can be allocated as local variables.

Conversely, non-address taken values which are usually on the stack are instead
represented as locals inside functions. This effectively means that WebAssembly
has an infinite set of registers, and can choose to spill values as it sees fit
in a manner unobservable to the hosted code. This implies that there's a
separate stack, unaddressable from hosted code, which is also used to spill
return values. This allows strong security properties to be enforced, but does
mean that two stacks are maintained (one by the VM, the other by the compiler
which targets WebAssembly) which can lead to some inefficiencies.

Local variables are not in Static Single Assignment (SSA) form, meaning that
multiple incoming SSA values which have separate liveness can "share" the
storage represented by a local through the `set_local` operator. From an SSA
perspective, this means that multiple independent values can share a local
variable in WebAssembly, which is effectively a kind of pre-coloring that clever
producers can use to pre-color variables and give hints to a WebAssembly VM's
register allocation algorithms, offloading some of the optimization work from
the WebAssembly VM.


## Variable-Length Argument Lists

C and C++ compilers are expected to implement variable-length argument lists by
storing arguments in a buffer in linear memory and passing a pointer to the
buffer. This greatly simplifies WebAssembly VM implementations by punting this
ABI consideration to the front-end compiler. It does negatively impact
performance, but variable-length calls are already somewhat slow.


## Multiple Return Values

TODO


## Indirect Calls

The table-based scheme for indirect function calls was motivated by the need
to represent function pointers as integer values that can be stored into the
linear memory, as well as to enforce basic safety properties such as
calling a function with the wrong signature does not destroy the safety
guarantees of WebAssembly. In particular, an exact signature match implies
an internal machine-level ABI match, which some engines require to ensure safety.
An indirection also avoids a possible information leak through raw code addresses.

Languages like C and C++ that compile to WebAssembly also imposed
requirements, such as the uniqueness of function pointers and the ability
to compare function pointers to data pointers, or treat data as function
pointers.

Several alternatives to direct indices with a heterogeneous indirect function table
were considered, from alternatives with multiple tables to statically typed function
pointers that can be mapped back and forth to integers. With the added complication
of dynamic linking and dynamic code generation, none of these alternatives perfectly
fit the requirements.

The current design requires two dynamic checks when invoking a function pointer:
a bounds check against the size of the indirect function table and a signature check
for the function at that index against an expected signature. Some dynamic optimization
techniques (e.g. inline caches, or a one-element cache), can reduce the number of
checks in common cases. Other techniques such as trading a bounds check for a mask or
segregating the table per signature to require only a bounds check could be considered
in the future. Also, if tables are small enough, an engine can internally use per-signature
tables filled with failure handlers to avoid one check.

## Expressions with Control Flow

Expression trees offer significant size reduction by avoiding the need for
`set_local`/`get_local` pairs in the common case of an expression with only one,
immediate use. The `comma` and `conditional` primitives provide AST nodes that
express control flow and thus allow more opportunities to build bigger
expression trees and further reduce `set_local`/`get_local` usage (which
constitute 30-40% of total bytes in the
[polyfill prototype](https://github.com/WebAssembly/polyfill-prototype-1)).
Additionally, these primitives are useful building blocks for
WebAssembly-generators (including the JavaScript polyfill prototype).


## Limited Local Nondeterminism

There are a few obvious cases where nondeterminism is essential to the API, such
as random number generators, date/time functions or input events. The
WebAssembly specification is strict when it comes to other sources of
[limited local nondeterminism](Nondeterminism.md) of operators: it specifies
all possible corner cases, and specifies a single outcome when this can be done
reasonably.

Ideally, WebAssembly would be fully deterministic because a fully deterministic
platform is easier to:

* Reason about.
* Implement.
* Test portably.

Nondeterminism is only specified as a compromise when there is no other
practical way to:

* Achieve [portable](Portability.md) native performance.
* Lower resource usage.
* Reduce implementation complexity (both of WebAssembly VMs as well as compilers
  generating WebAssembly binaries).
* Allow usage of new hardware features.
* Allows implementations to security-harden certain usecases.

When nondeterminism is allowed into WebAssembly it is always done in a limited
and local manner. This prevents the entire program from being invalid, as would
be the case with C++ undefined behavior.

As WebAssembly gets implemented and tested with multiple languages on multiple
architectures there may be a need to revisit some of the decisions:

* When all relevant hardware implement features the same way then there's no
  need to add nondeterminism to WebAssembly when realistically there's only one
  mapping from WebAssembly expression to ISA-specific operators. One such
  example is floating-point: at a high-level most basic instructions follow
  IEEE-754 semantics, it is therefore not necessary to specify WebAssembly's
  floating-point operators differently from IEEE-754.
* When different languages have different expectations then it's unfortunate if
  WebAssembly measurably penalizes one's performance by enforcing determinism
  which that language doesn't care about, but which another language may want.


## Motivating Scenarios for Feature Testing

1. [Post-MVP](PostMVP.md),
[`i32.min_s`](FutureFeatures.md#additional-integer-operations) is introduced. A
WebAssembly developer updates their toolkit so that the compiler may leverage
`i32.min_s`. The developer's WebAssembly module works correctly both on
execution environments at MVP, as well as those supporting `i32.min_s`.

  * A variant of this, where a few more new opcodes are available, the compiler
is updated to be able to leverage all of them, but not all execution targets
support all of them. The developer wants to reach as many of their customers as
possible, while at the same time providing them with the best experience
possible. The developer has to balance the cost of the test matrix resulting
from the combinations of possible feature configurations.

2. Post-MVP, module authors may now use [Threading](PostMVP.md#threads) APIs in
the browser. A developer wants to leverage multithreading in their module.

  * In one variant of the scenario, our developer does not want to pay the
engineering cost of developing and supporting a threaded and non-threaded
version of their code. They opt not to support MVP targets, and only support
post-MVP targets. End-users (browser users) get some message indicating they
need MVP support.

  * In another variant, our developer explicitly authors both MVP-only and post-
MVP (with threads) code.

3. [SIMD](PostMVP.md#fixed-width-simd) support is not universally equivalent on
all targets. While polyfill variants of SIMD APIs are available, a developer
prefers writing dedicated SIMD and non-SIMD versions of their compression
algorithm, because the non-SIMD version performs better in environments without
SIMD support, when compared to the SIMD polyfill. They package their compression
code for reuse by third parties.

4. An application author is assembling together an application by reusing
modules such as those developed in the scenarios above. The application author's
development environment is able to quickly and correctly identify the platform
dependencies (e.g. threading, SIMD) and communicate back to the application
author the implications these dependencies have on the end-application. Some
APIs exposed from the threading-aware module are only pertinent to environments
supporting threading. As a consequence, the application author needs to write
specialized code when threads are/are not supported. (Note: we should understand
this scenario for both forms of WebAssembly reuse currently imagined: dynamic
linking and static imports.)

5. The compression algorithm described in scenario 3 is deployed on a
restrictive execution environment, as part of an application. In this
environment, a process may not change memory page access protection flags (e.g.
certain gaming consoles, to investigate server side deployment scenarios). The
compression module is compiled by the WebAssembly environment, enabling the
configuration most specific to the target (i.e. with/without Threads, SIMD,
etc).

  * A variant of this scenario where the environment is additionally separating
storage into system-visible and application-visible, the latter not being able
to contain machine-executable code (certain phones, to investigate if gaming
consoles or server side have a similar sandboxing mechanism).
