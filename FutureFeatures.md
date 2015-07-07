# Feature to add after the MVP

These are features that make sense in the context of the
[high-level goals](HighLevelGoals.md) of WebAssembly but are not considered part
of the [Minimum Viable Product](MVP.md) or the essential [post-MVP](PostMVP.md)
feature set which are expected to be standardized immediately after the
MVP. These will be prioritized based on developer feedback, and will be
available under [feature tests](FeatureTest.md).

## Great tooling support

This is covered in the [tooling](Tooling.md) section.

## Dynamic linking

[Dynamic loading](MVP.md#code-loading-and-imports) is in [the MVP](MVP.md), but
all loaded modules have their own [separate linear memory](MVP.md#linear-memory) and cannot share
[function pointers](MVP.md#function-pointers). Dynamic linking will allow
developers to share memory and function pointers between WebAssembly modules.

WebAssembly will support both load-time and run-time (`dlopen`) dynamic linking
of both WebAssembly modules and non-WebAssembly modules (e.g., on the Web, ES6
ones containing JavaScript).

Dynamic linking is especially useful when combined with a Content Distribution
Network (CDN) such as [hosted libraries][] because the library is only ever
downloaded and compiled once per user device. It can also allow for smaller
differential updates, which could be implemented in collaboration with
[service workers][].

Standardize a single [ABI][] per source language, allowing for WebAssembly
modules to interface with each other regardless of compiler. While it is highly
recommended for compilers targeting WebAssembly to adhere to the specified ABI
for interoperability, WebAssembly runtimes will be ABI agnostic, so it will be
possible to use a non-standard ABI for specialized purposes.

  [hosted libraries]: https://developers.google.com/speed/libraries/
  [service workers]: https://www.w3.org/TR/service-workers/
  [ABI]: https://en.wikipedia.org/wiki/Application_binary_interface

## Finer-grained control over memory

* `mmap` of files.
* `madvise(MADV_DONTNEED)`.
* Shared memory, where a physical address range is mapped to multiple physical
  pages in a single WebAssembly module as well as across modules.

## More expressive control flow

Some types of control flow (especially irreducible and indirect) cannot be
expressed with maximum efficiency in WebAssembly without patterned output by the
relooper and [jump-threading](https://en.wikipedia.org/wiki/Jump_threading)
optimizations in the engine. Target uses for more expressive control flow are:
* Language interpreters, which often use computed-`goto`.
* Functional language support, where guaranteed tail call optimization is
  expected for correctness and performance.

Options under consideration:
* No action, `while` and `switch` combined with jump-threading are enough.
* Just add `goto` (direct and indirect).
* Add new control-flow primitives that address common patterns.
* Add signature-restricted Proper Tail Calls.
* Add proper tail call, expanding upon signature-restricted proper tail calls, and
  making it easier to support other languages, especially functional programming
  languages.

## GC/DOM Integration

* Access to certain kinds of Garbage-Collected (GC) objects from variables,
  arguments, expressions.
* Ability to GC-allocate certain kinds of GC objects.
* Initially, things with fixed structure:
  * JavaScript strings;
  * JavaScript functions (as callable closures);
  * Typed Arrays;
  * [Typed objects](https://github.com/nikomatsakis/typed-objects-explainer/);
  * DOM objects via WebIDL.
* Perhaps a rooting API for safe reference from the linear address space.

## Linear memory bigger than 4GiB

WebAssembly will eventually allow a module to have a linear memory size greater than 4GiB by providing
load/store operations that take 64-bit index operands. Modules which opt-in to
this feature have `int64` as the canonical pointer type.

On a 32-bit system, memory must still be smaller than 4GiB. A WebAssembly
implementation running on such a platform may restrict allocations to the lower
4GiB, and leave the two 32-bits untouched.

## Source maps integration

* Add a new source maps [module section type](MVP.md#module-structure).
* Either embed the source maps directly or just a URL from which source maps can
  be downloaded.
* Text source maps become intractably large for even moderate-sized compiled
  codes, so probably need to define new binary format for source maps.
* Gestate ideas and start discussions at the
  [Source Map RFC repository](https://github.com/source-map/source-map-rfc/issues)

## Coroutines

Coroutines will [eventually be part of C++][] and is already popular in other
programming languages that WebAssembly will support.

  [eventually be part of C++]: http://wg21.link/n4499

## Signature-restricted Proper Tail Calls

See the [asm.js RFC][] for a full description of signature-restricted Proper
Tail Calls (PTC).

Useful properties of signature-restricted PTCs:

* In most cases, can be compiled to a single jump.
* Can express indirect `goto` via function-pointer calls.
* Can be used as a compile target for languages with unrestricted PTCs; the code
  generator can use a stack in the linear memory to effectively implement a custom call
  ABI on top of signature-restricted PTCs.
* An engine that wishes to perform aggressive optimization can fuse a graph of
  PTCs into a single function.
* To reduce compile time, a code generator can use PTCs to break up ultra-large
  functions into smaller functions at low overhead using PTCs.
* A compiler can exert some amount of control over register allocation via the
  ordering of arguments in the PTC signature.

  [asm.js RFC]: http://discourse.specifiction.org/t/request-for-comments-add-a-restricted-subset-of-proper-tail-calls-to-asm-js
 
## Asynchronous Signals

TODO

## "Long SIMD"

The initial SIMD API will be a "short SIMD" API, centered around fixed-width
128-bit types and explicit SIMD operations. This is quite portable and useful,
but it won't be able to deliver the full performance capabilities of some of
today's popular hardware. There is [a proposal in the SIMD.js repository][] for
a "long SIMD" model which generalizes to wider hardware vector lengths, making
more natural use of advanced features like vector lane predication,
gather/scatter, and so on. Interesting questions to ask of such an model will
include:

* How will this model map onto popular modern SIMD hardware architectures?
* What is this model's relationship to other hardware parallelism features, such
  as GPUs and threads with shared memory?
* How will this model be used from higher-level programming languages? For
  example, the C++ committee is considering a wide variety of possible
  approaches; which of them might be supported by the model?
* What is the relationship to the "short SIMD" API? "None" may be an acceptable
  answer, but it's something to think about.
* What nondeterminism does this model introduce into the overall platform?
* What happens when code uses long SIMD on a hardware platform which doesn't
  support it? Reasonable options may include emulating it without the benefit of
  hardware acceleration, or indicating a lack of support through feature tests.

  [a proposal in the SIMD.js repository]: https://github.com/tc39/ecmascript_simd/issues/180

## Platform-independent Just-in-Time compilation

WebAssembly is a new virtual ISA, and as such applications won't be able to
simply reuse their existing JIT-compiler backends. Applications will instead
have to interface with WebAssembly's instructions as if they were a new ISA.

Applications expect a wide variety of JIT-compilation capabilities. WebAssembly
should support:

* Producing a dynamic library and loading it into the current WebAssembly
  module.
* Define lighter-weight mechanisms, such as the ability to add a function to an
  existing module.
* Support explicitly patchable constructs within functions to allow for very
  fine-grained JIT-compilation. This includes:
    * Code patching for polymorphic inline caching;
	* Call patching to chain JIT-compiled functions together;
	* Temporary halt-insertion within functions, to trap if a function start
      executing while a JIT-compiler's runtime is performing operations
      dangerous to that function.
* Provide JITs access to profile feedback for their JIT-compiled code.
* Code unloading capabilities, especially in the context of code garbage
  collection and defragmentation.

## Multiprocess support

* `vfork`.
* Inter-process communication.
* Inter-process `mmap`.

## Trapping or non-trapping strategies.

Presently, when an instruction traps, the program is immediately terminated.
This suits C/C++ code, where trapping conditions indicate Undefined Behavior at
the source level, and it's also nice for handwritten code, where trapping
conditions typically indicate an instruction being asked to perform outside its
supported range. However, the current facilities do not cover some interesting
use cases:

* Not all likely-bug conditions are covered. For example, it would be very nice
  to have a signed-integer add which traps on overflow. Such a construct would
  add too much overhead on today's popular hardware architectures to be used in
  general, however it may still be useful in some contexts.
* Some higher-level languages define their own semantics for conditions like
  division by zero and so on. It's possible for compilers to add explicit checks
  and handle such cases manually, though more direct support from the platform
  could have advantages:
  * Non-trapping versions of some opcodes, such as an integer division
    instruction that returns zero instead of trapping on division by zero, could
    potentially run faster on some platforms.
  * The ability to recover gracefully from traps in some way could make many
    things possible. Possibly this could involve throwing or possibly by
    resuming execution at the trapping instruction with the execution state
    altered, if there can be a reasonable way to specify how that should work.

## Additional integer operations

* The following operations can be built from other operators already present,
  however in doing so they read at least one non-constant input multiple times,
  breaking single-use expression tree formation.
  * `int32.rotr`: bitwise rotate right
  * `int32.rotl`: bitwise rotate left
  * `int32.smin`: signed minimum
  * `int32.smax`: signed maximum
  * `int32.umin`: unsigned minimum
  * `int32.umax`: unsigned maximum
  * `int32.sext`: `sext(x, y)` is `x<<y>>y`
  * `int32.abs`: absolute value (is `abs(INT32_MIN)` `INT32_MIN` or should it trap?)
  * `int32.bswap`: reverse bytes (endian conversion)
  * `int32.bswap16`: `bswap16(x)` is `((x>>8)&255)|((x&255)<<8)`

* The following operations are just potentially interesting.
  * `int32.clrs`: count leading redundant sign bits (defined for all values, including 0)

## Additional floating point operations

  * `float32.minnum`: minimum; if exactly one operand is NaN, returns the other operand
  * `float32.maxnum`: maximum; if exactly one operand is NaN, returns the other operand
  * `float32.fma`: fused multiply-add (results always conforming to IEEE-754)
  * `float64.minnum`: minimum; if exactly one operand is NaN, returns the other operand
  * `float64.maxnum`: maximum; if exactly one operand is NaN, returns the other operand
  * `float64.fma`: fused multiply-add (results always conforming to IEEE-754)

`minnum` and `maxnum` operations would treat `-0.0` as being effectively less
than `0.0`.

Note that some operations, like `fma`, may not be available or may not perform
well on all platforms. These should be guarded by
[feature tests](FeatureTest.md) so that if available, they behave consistently.

## Floating point approximation operations

  * `float32.reciprocal_approximation`: reciprocal approximation
  * `float64.reciprocal_approximation`: reciprocal approximation
  * `float32.reciprocal_sqrt_approximation`: reciprocal sqrt approximation
  * `float64.reciprocal_sqrt_approximation`: reciprocal sqrt approximation

These operations would not required to be fully precise, but the specifics
would need clarification.

## 16-bit and 128-bit floating point support

For 16-bit floating point support, it may make sense to split the feature
into two parts: support for just converting between 16-bit and 32-bit or
64-bit formats possibly folded into load and store operations, and full
support for actual 16-bit arithmetic.

128-bit is an interesting question because hardware support for it is very
rare, so it's usually going to be implemented with software emulation anyway,
so there's nothing preventing WebAssembly applications from linking to an
appropriate emulation library and getting similarly performant results.
Emulation libraries would have more flexibility to offer approximation
techniques such as double-double arithmetic. If we standardize 128-bit
floating point in WebAssembly, it will probably be standard IEEE-754
quadruple precision.

## Floating point library intrinsics

These operations aren't needed because they can be implemented in WebAssembly
code and linked into WebAssembly modules as at small size cost, and this avoids
a non-trivial specification burden of their semantics/precision. Adding these
intrinsics would allow for better high-level backend optimization of these
intrinsics that require builtin knowledge of their semantics. On the other
hand, a code generator may continue to statically link in its own
implementation since this provides greater control over precision/performance
tradeoffs.

  * `float64.sin`: trigonometric sine
  * `float64.cos`: trigonometric cosine
  * `float64.tan`: trigonometric tangent
  * `float64.asin`: trigonometric arcsine
  * `float64.acos`: trigonometric arccosine
  * `float64.atan`: trigonometric  arctangent
  * `float64.atan2`: trigonometric arctangent with two arguments
  * `float64.exp`: exponentiate e
  * `float64.ln`: natural logarithm
  * `float64.pow`: exponentiate
  * `float32.sin`: trigonometric sine
  * `float32.cos`: trigonometric cosine
  * `float32.tan`: trigonometric tangent
  * `float32.asin`: trigonometric arcsine
  * `float32.acos`: trigonometric arccosine
  * `float32.atan`: trigonometric  arctangent
  * `float32.atan2`: trigonometric arctangent with two arguments
  * `float32.exp`: exponentiate e
  * `float32.ln`: natural logarithm
  * `float32.pow`: exponentiate

The rounding behavior of these operations would need clarification.

## Full IEEE-754 conformance

WebAssembly floating point conforms IEEE-754 in most respects, but there are a
few areas that are [not yet covered](AstSemantics.md#floating-point-operations).

IEEE-754 NaN bit pattern propagation is presently permitted but not required.
It would be possible for WebAssembly to require it in the future.

To support exceptions and alternate rounding modes, one option is to define an
alternate form for each of `add`, `sub`, `mul`, `div`, `sqrt`, and `fma`. These
alternate forms would have extra operands for rounding mode, masked traps, and
old flags, and an extra result for a new flags value. These operations would be
fairly verbose, but it's expected that their use cases will specialized. This
approach has the advantage of exposing no global (even if only per-thread)
control and status registers to applications, and to avoid giving the common
operations the possibility of having side effects.

Debugging techniques are also important, but they don't necessarily need to be
in the spec itself. Implementations are welcome (and encouraged) to support
non-standard execution modes, enabled only from developer tools, such as modes
with alternate rounding, or evaluation of floating point expressions at greater
precision, to support [techniques for detecting numerical instability]
(https://www.cs.berkeley.edu/~wkahan/Mindless.pdf).

To help developers find the sources of floating point exceptions,
implementations may wish to provide a mode where NaN values are produced with
payloads containing identifiers helping programmers locate where the NaNs first
appeared. Another option would be to offer another non-standard execution mode,
enabled only from developer tools, that would enable traps on selected floating
point exceptions, however care should be taken, since not all floating point
exceptions indicate bugs.
