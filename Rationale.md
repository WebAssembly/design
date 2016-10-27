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
and update the [design](Semantics.md) before the MVP is finalized.


## Why a stack machine?

Why not an AST, or a register- or SSA-based bytecode?

* We started with an AST and generalized to a [structured stack machine](Semantics.md). ASTs allow a 
  dense encoding and efficient decoding, compilation, and interpretation.
  The structured stack machine of WebAssembly is a generalization of ASTs allowed in previous versions while allowing
  efficiency gains in interpretation and baseline compilation, as well as a straightforward 
  design for multi-return functions.
* The stack machine allows smaller binary encoding than registers or SSA [JSZap][], [Slim Binaries][],
  and structured control flow allows simpler and more efficient verification, including decoding directly
  to a compiler's internal SSA form.
* [Polyfill prototype][] shows simple and efficient translation to asm.js.

  [JSZap]: https://research.microsoft.com/en-us/projects/jszap/
  [Slim Binaries]: https://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.108.1711
  [Polyfill prototype]: https://github.com/WebAssembly/polyfill-prototype-1


## Why not a fully-general stack machine?

The WebAssembly stack machine is restricted to structured control flow and structured
use of the stack. This greatly simplifies one-pass verification, avoiding a fixpoint computation
like that of other stack machines such as the Java Virtual Machine (prior to [stack maps](https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html)).
This also simplifies compilation and manipulation of WebAssembly code by other tools.
Further generalization of the WebAssembly stack machine is planned post-MVP, such as the
addition of multiple return values from control flow constructs and function calls.

## Basic Types Only

WebAssembly only represents [a few types](Semantics.md#Types).

* More complex types can be formed from these basic types. It's up to the source
  language compiler to express its own types in terms of the basic machine
  types. This allows WebAssembly to present itself as a virtual ISA, and lets
  compilers target it as they would any other ISA.
* These types are directly representable on all modern CPU architectures.
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
[addressing](Semantics.md#Addressing). This is intended to simplify folding
of offsets into complex address modes in hardware, and to simplify bounds
checking optimizations. It offloads some of the optimization work to the
compiler that targets WebAssembly, executing on the developer's machine, instead
of performing that work in the WebAssembly compiler on the user's machine.


## Alignment Hints

Load/store instructions contain
[alignment hints](Semantics.md#Alignment). This makes it easier to generate
efficient code on certain hardware architectures.

Either tooling or an explicit opt-in "debug mode" in the spec could allow
execution of a module in a mode that threw exceptions on misaligned access.
This mode would incur some runtime cost for branching on most platforms which is
why it isn't the specified default.


## Out of Bounds

The ideal semantics is for
[out-of-bounds accesses](Semantics.md#Out-of-Bounds) to trap, but the
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


## Linear Memory Resizing

To allow efficient engines to employ virtual-memory based techniques for bounds
checking, memory sizes are required to be page-aligned.
For portability across a range of CPU architectures and operating systems,
WebAssembly defines a fixed page size.
Programs can depend on this fixed page size and still remain portable across all
WebAssembly engines.
64KiB represents the least common multiple of many platforms and CPUs.
In the future, WebAssembly may offer the ability to use larger page sizes on
some platforms for increased TLB efficiency.

The `grow_memory` operator returns the old memory size. This is desirable for
using `grow_memory` independently on multiple threads, so that each thread can
know where the region it allocated starts. The obvious alternative would be for
such threads to communicate manually, however wasm implementations will likely
already be communicating between threads in order to properly allocate the sum
of the allocation requests, so it's expected that they can provide the needed
information without significant extra effort.

The [optional maximum size](Modules.md#linear-memory-section) is designed to
address a number of competing constraints:

1. Allow WebAssembly modules to grab large regions of contiguous memory in a
   32-bit address space early in an application's startup before the virtual
   address space becomes fragmented by execution of the application.
2. Allow many small WebAssembly instances to execute in a single 32-bit process.
   (For example, it is common for a single web application to use dozens of
   libraries, each of which may, over time, include WebAssembly modules as
   implementation details.)
3. Avoid *forcing* every developer using WebAssembly to understand their precise
   maximum heap usage.
4. When threading and shared memory are added to WebAssembly
   [post-MVP](PostMVP.md#threads), the design should not require memory growth
   to `realloc` since this implies significant implementation complexity,
   security hazards, and optimization challenges.

The optional maximum addresses these constraints:

* (1) is addressed by specifying a large maximum memory size. Simply setting a
  large *initial* memory size has problems due to (3) and the fact that a
  failure to allocate initial is a fatal error which makes the choice of "how
  big?" difficult.
* (2) and (3) are addressed by making the maximum optional combined with the
  implied implementation that, on 32-bit, engines will not allocate
  significantly more than the current memory size, *and* the compiler sets the
  initial size to just enough to hold static data.
* (4) is addressed assuming that, when threading is added, a new, optional
  "shared" flag is added to the memory section that must be set to enable shared
  memory and the shared flag forces the maximum to be specified. In this case,
  shared memory never moves; the only thing that changes is that the bounds
  grows which does not have all the abovementioned hazards. In particular, any
  extant `SharedArrayBuffer`s that alias linear memory stay valid without
  any updates.

## Linear memory disabled if no linear memory section

See [#107](https://github.com/WebAssembly/spec/pull/107).


## Control Flow

Structured control flow provides simple and size-efficient binary encoding and
compilation. Any control flow--even irreducible--can be transformed into structured
control flow with the
[Relooper](https://github.com/kripken/emscripten/raw/master/docs/paper.pdf)
[algorithm](http://dl.acm.org/citation.cfm?id=2048224&CFID=670868333&CFTOKEN=46181900),
with guaranteed low code size overhead, and typically minimal throughput
overhead (except for pathological cases of irreducible control
flow). Alternative approaches can generate reducible control flow via node
splitting, which can reduce throughput overhead, at the cost of increasing
code size (potentially very significantly in pathological cases).
Also,
[more expressive control flow constructs :unicorn:][future flow control]
may be added in the future.

## Nop

The nop operator does not produce a value or cause side effects.
It is nevertheless useful for compilers and tools, which sometimes need to replace instructions with a `nop`. Without a `nop` instruction, code generators would use alternative *does-nothing* opcode patterns that consume space in a module and may have a runtime cost. Finding an appropriate opcode that does nothing but has the appropriate type for the node's location is nontrivial. The existence of many different ways to encode `nop` - often mixed in the same module - would reduce the efficiency of compression algorithms.


## Locals

C/C++ makes it possible to take the address of a function's local values and
pass this pointer to callees or to other threads. Since WebAssembly's local
variables are outside the address space, C/C++ compilers implement address-taken
variables by creating a separate stack data structure within linear memory. This
stack is sometimes called the "aliased" stack, since it is used for variables
which may be pointed to by pointers.

Since the aliased stack appears to the WebAssembly engine as normal memory,
WebAssembly optimizations that would target the aliased stack need to be more
general, and thus more complicated. We observe that common compiler
optimizations done before the WebAssembly code is produced, such as LLVM's
global value numbering, effectively split address-taken variables into many
small ranges that can often be allocated as local variables. Thus our
expectation that any loss of optimization potential here is minimal.

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


## Variable-Length Argument Lists ("varargs")

C and C++ compilers are expected to implement variable-length argument lists by
storing arguments in a buffer in linear memory and passing a pointer to the
buffer. This greatly simplifies WebAssembly VM implementations by punting this
ABI consideration to the front-end compiler. It does negatively impact
performance, but variable-length calls are already somewhat slow.


## Multiple Return Values

WebAssembly's MVP does not support multiple return values from functions because
they aren't strictly necessary for the earliest anticipated use cases (and it's
a *minimum* viable product), and they would introduce some complexity for some
implementations. However, multiple return values are a very useful feature, and
are relevant to ABIs, so it's likely to be added soon after the MVP.


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

## Control Flow Instructions with Values

Control flow instructions such as `br`, `br_if`, `br_table`, `if` and `if-else` can 
transfer stack values in WebAssembly. These primitives are useful building blocks for 
WebAssembly producers, e.g. in compiling expression languages. It offers significant 
size reduction by avoiding the need for `set_local`/`get_local` pairs in the common case 
of an expression with only one immediate use. Control flow instructions can then model
expressions with result values, thus allowing even more opportunities to further reduce
`set_local`/`get_local` usage (which constitute 30-40% of total bytes in the
[polyfill prototype](https://github.com/WebAssembly/polyfill-prototype-1)).
`br`-with-value and `if` constructs that return values can model also model `phis` which
appear in SSA representations of programs.


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
* Allows implementations to [security-harden](Security.md) certain usecases.

When nondeterminism is allowed into WebAssembly it is always done in a limited
and local manner. This prevents the entire program from being invalid, as would
be the case with C++ undefined behavior.

As WebAssembly gets implemented and tested with multiple languages on multiple
architectures we may revisit some of the design decisions:

* When all relevant hardware implements an operation the same way, there's no
  need for nondeterminism in WebAssembly semantics. One such
  example is floating-point: at a high-level most operators follow
  IEEE-754 semantics, it is therefore not necessary to specify WebAssembly's
  floating-point operators differently from IEEE-754.
* When different languages have different expectations then it's unfortunate if
  WebAssembly measurably penalizes one's performance by enforcing determinism
  which that language doesn't care about, but which another language may want.


## NaN bit pattern propagation

In general, WebAssembly's floating point operations provide the guarantee that
if all NaNs passed to an operation are "canonical", the result is "canonical",
where canonical means the most significant bit of the fraction field is 1, and
the trailing bits are all 0.

This is intended to support interpreters running on WebAssembly that use
NaN-boxing, because they don't have to canonicalize the output of an arithmetic
instruction if they know the inputs are canonical.

When the inputs are non-canonical, the resulting NaN is nondeterministic, to
accomodate a variety of known hardware behaviors: returning one of the input
NaNs, returning a canonical NaN, or bitwise-or'ing the input NaNs together.

The sign bit of generated NaNs is always nondeterministic since x86 generates
NaNs with it set to 1 while other architectures generate NaNs with it set to 0.


## Integer operations

WebAssembly's signed integer divide rounds its result toward zero. This is not
because of a lack of sympathy for
[better alternatives](https://python-history.blogspot.com/2010/08/why-pythons-integer-division-floors.html),
but out of practicality. Because all popular hardware today implements
rounding toward zero, and because C and many other languages now specify
rounding to zero, having WebAssembly in the middle doing something different
would mean divisions would have to be doubly complicated.

Similarly, WebAssembly's shift operators mask their shift counts to the number
of bits in the shifted value. Confusingly, this means that shifting a 32-bit
value by 32 bits is an identity operation, and that a left shift is not
equivalent to a multiplication by a power of 2 because the overflow behavior
is different. Nevertheless, because several popular hardware architectures
today implement this masking behavior, and those that don't can typically
emulate it with a single extra mask instruction, and because several popular
source languages, including JavaScript and C#, have come to specify this
behavior too, we reluctantly adopt this behavior as well.

WebAssembly has three classes of integer operations: signed, unsigned, and
sign-agnostic. The signed and unsigned instructions have the property that
whenever they can't return their mathematically expected value (such as when
an overflow occurs, or when their operand is outside their domain), they
trap, in order to avoid silently returning an incorrect value.

Note that the `add`, `sub`, and `mul` operators are categorized as
sign-agnostic. Because of the magic of two's complement representation, they
may be used for both signed and unsigned purposes. Note that this (very
conveniently!) means that engines don't need to add extra overflow-checking
code for these most common of arithmetic operators on the most popular
hardware platforms.


## Motivating Scenarios for Feature Testing

1. [Post-MVP](PostMVP.md),
[`i32.min_s` :unicorn:][future integers] is introduced. A
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


## Why a binary encoding?

Given that text is so compressible and it is well known that it is hard to beat
gzipped source, is there any win from having a binary format over a text format?
Yes:

* Large reductions in payload size can still significantly decrease the
  compressed file size.
  * Experimental results from a
    [polyfill prototype](https://github.com/WebAssembly/polyfill-prototype-1) show the
    gzipped binary format to be about 20-30% smaller than the corresponding
    gzipped asm.js.
* A binary format that represents the names of variables and functions with raw
  indices instead of strings is much faster to decode: array indexing
  vs. dictionary lookup.
  * Experimental results from a
    [polyfill prototype](https://github.com/WebAssembly/polyfill-prototype-1) show that
    decoding the binary format is about 23× faster than parsing the
    corresponding asm.js source (using
    [this demo](https://github.com/lukewagner/AngryBotsPacked), comparing
    *just* parsing in SpiderMonkey (no validation, IR generation) to *just*
    decoding in the polyfill (no asm.js code generation).
* A binary format allows many optimizations for code size and decoding speed that would
  not be possible on a source form.


## Why a layered binary encoding?
* We can do better than generic compression because we are aware of the code
  structure and other details:
  * For example, macro compression that
    [deduplicates AST trees](https://github.com/WebAssembly/design/issues/58#issuecomment-101863032)
    can focus on ASTs + their children, thus having `O(nodes)` entities
    to worry about, compared to generic compression which in principle would
    need to look at `O(bytes*bytes)` entities.  Such macros would allow the
    logical equivalent of `#define ADD1(x) (x+1)`, i.e., to be
    parameterized. Simpler macros (`#define ADDX1 (x+1)`) can implement useful
    features like constant pools.
  * Another example is reordering of functions and some internal nodes, which
    we know does not change semantics, but
    [can improve general compression](https://www.rfk.id.au/blog/entry/cromulate-improve-compressibility/).
* JITs and simple developer tooling do not benefit from compression, so layering allows
  the related development and maintenance burden to be offloaded to reusable tools/libraries.
* Each of the layers works to find compression opportunities to the best of
  its abilities, without encroaching upon the subsequent layer's compression
  opportunities.
* [Existing web standards](https://www.w3.org/TR/PNG/) demonstrate many of
  the advantages of a layered encoding strategy.

[future flow control]: FutureFeatures.md#more-expressive-control-flow
[future integers]: FutureFeatures.md#additional-integer-operations
