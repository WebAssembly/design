# Feature to add after the MVP

These are features that make sense in the context of the
[high-level goals](HighLevelGoals.md) of WebAssembly but are not considered part
of the [Minimum Viable Product](MVP.md) or the essential [post-MVP](PostMVP.md)
feature set which are expected to be standardized immediately after the
MVP. These will be prioritized based on developer feedback, and will be
available under [feature tests](FeatureTest.md).

## Great tooling support

This is covered in the [tooling](Tooling.md) section.

## Finer-grained control over memory

Provide access to safe OS-provided functionality including:
* `map_file(addr, length, Blob, file-offset)`: semantically, this operator
   copies the specified range from `Blob` into the range `[addr, addr+length)`
   (where `addr+length <= memory_size`) but implementations are encouraged
   to `mmap(addr, length, MAP_FIXED | MAP_PRIVATE, fd)`
* `discard(addr, length)`: semantically, this operator zeroes the given range
   but the implementation is encouraged to drop the zeroed physical pages from
   the process's working set (e.g., by calling `madvise(MADV_DONTNEED)` on
   POSIX)
* `shmem_create(length)`: create a memory object that can be simultaneously
  shared between multiple linear memories
* `map_shmem(addr, length, shmem, shmem-offset)`: like `map_file` except
  `MAP_SHARED`, which isn't otherwise valid on read-only Blobs
* `mprotect(addr, length, prot-flags)`: change protection on the range
  `[addr, addr+length)` (where `addr+length <= memory_size`)
* `decommit(addr, length)`: equivalent to `mprotect(addr, length, PROT_NONE)`
  followed by `discard(addr, length)` and potentially more efficient than
  performing these operators in sequence.

The `addr` and `length` parameters above would be required to be multiples of
[`page_size`](AstSemantics.md#resizing).

The `mprotect` operator would require hardware memory protection to execute
efficiently and thus may be added as an "optional" feature (requiring a
[feature test](FeatureTest.md) to use). To support efficient execution even when
no hardware memory protection is available, a restricted form of `mprotect`
could be added which is declared statically and only protects low memory
(providing the expected fault-on-low-memory behavior of native C/C++ apps).

The above list of functionality mostly covers the set of functionality
provided by the `mmap` OS primitive. One significant exception is that `mmap`
can allocate noncontiguous virtual address ranges. See the
[FAQ](FAQ.md#what-about-mmap) for rationale.

## Large page support

Some platforms offer support for memory pages as large as 16GiB, which 
can improve  the efficiency of memory management in some situations. WebAssembly
may offer programs the option to specify a larger page size than the [default] (AstSemantics.md#resizing).

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

See [GC.md](GC.md).

## Linear memory bigger than 4 GiB

The WebAssembly MVP will support the wasm32 mode of WebAssembly, with linear
memory sizes up to 4 GiB using 32-bit linear memory indices. To support larger
sizes, the wasm64 mode of WebAssembly will be added in the future, supporting
much greater linear memory sizes using 64-bit linear memory indices. wasm32
and wasm64 are both just modes of WebAssembly, to be selected by a flag in
a module header, and don't imply any semantics differences outside of how
linear memory is handled. Platforms will also have APIs for querying which of
wasm32 and wasm64 are supported.

Of course, the ability to actually allocate this much memory will always be
subject to dynamic resource availability.

It is likely that wasm64 will initially support only 64-bit linear memory
indices, and wasm32 will leave 64-bit linear memory indices unsupported, so
that implementations don't have to support multiple index sizes in the same
instance. However, operators with 32-bit indices and operators with 64-bit
indices will be given separate names to leave open the possibility of
supporting both in the same instance in the future.

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
 
## General-purpose Proper Tail Calls

General-purpose Proper Tail Calls would have no signature restrictions, and
therefore be more broadly usable than
[Signature-restricted Proper Tail Calls](AstSemantics.md#signature-restricted-proper-tail-calls),
though there would be some different performance characteristics.

## Asynchronous Signals

TODO

## "Long SIMD"

The initial SIMD API will be a "short SIMD" API, centered around fixed-width
128-bit types and explicit SIMD operators. This is quite portable and useful,
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

## Platform-independent Just-in-Time (JIT) compilation

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
      executing while a JIT-compiler's runtime is performing operators
      dangerous to that function.
* Provide JITs access to profile feedback for their JIT-compiled code.
* Code unloading capabilities, especially in the context of code garbage
  collection and defragmentation.

WebAssembly's JIT interface would likely be fairly low-level. However, there
are use cases for higher-level functionality and optimization too. One avenue
for addressing these use cases is a
[JIT and Optimization library](JITLibrary.md).

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
  * Non-trapping versions of some operators, such as an integer division
    instruction that returns zero instead of trapping on division by zero, could
    potentially run faster on some platforms.
  * The ability to recover gracefully from traps in some way could make many
    things possible. Possibly this could involve throwing or possibly by
    resuming execution at the trapping instruction with the execution state
    altered, if there can be a reasonable way to specify how that should work.

## Additional integer operators

* The following operators can be built from other operators already present,
  however in doing so they read at least one non-constant input multiple times,
  breaking single-use expression tree formation.
  * `i32.min_s`: signed minimum
  * `i32.max_s`: signed maximum
  * `i32.min_u`: unsigned minimum
  * `i32.max_u`: unsigned maximum
  * `i32.sext`: sign-agnostic `sext(x, y)` is `shr_s(shl(x,y),y)`
  * `i32.abs_s`: signed absolute value (traps on `INT32_MIN`)
  * `i32.bswap`: sign-agnostic reverse bytes (endian conversion)
  * `i32.bswap16`: sign-agnostic, `bswap16(x)` is `((x>>8)&255)|((x&255)<<8)`

* The following operators are just potentially interesting.
  * `i32.clrs`: sign-agnostic count leading redundant sign bits (defined for
    all values, including 0)
  * `i32.floor_div_s`: signed division (result is [floored](https://en.wikipedia.org/wiki/Floor_and_ceiling_functions))

* The following 64-bit-only operators are potentially interesting as well.
  * `i64.mor`: sign-agnostic [8x8 bit-matrix multiply with or](http://mmix.cs.hm.edu/doc/instructions-en.html#MOR)
  * `i64.mxor`: sign-agnostic [8x8 bit-matrix multiply with xor](http://mmix.cs.hm.edu/doc/instructions-en.html#MXOR)

## Additional floating point operators

  * `f32.minnum`: minimum; if exactly one operand is NaN, returns the other operand
  * `f32.maxnum`: maximum; if exactly one operand is NaN, returns the other operand
  * `f32.fma`: fused multiply-add (results always conforming to IEEE 754-2008)
  * `f64.minnum`: minimum; if exactly one operand is NaN, returns the other operand
  * `f64.maxnum`: maximum; if exactly one operand is NaN, returns the other operand
  * `f64.fma`: fused multiply-add (results always conforming to IEEE 754-2008)

`minnum` and `maxnum` operators would treat `-0.0` as being effectively less
than `0.0`.

Note that some operators, like `fma`, may not be available or may not perform
well on all platforms. These should be guarded by
[feature tests](FeatureTest.md) so that if available, they behave consistently.

## Floating point approximation operators

  * `f32.reciprocal_approximation`: reciprocal approximation
  * `f64.reciprocal_approximation`: reciprocal approximation
  * `f32.reciprocal_sqrt_approximation`: reciprocal sqrt approximation
  * `f64.reciprocal_sqrt_approximation`: reciprocal sqrt approximation

These operators would not required to be fully precise, but the specifics
would need clarification.

## 16-bit and 128-bit floating point support

For 16-bit floating point support, it may make sense to split the feature
into two parts: support for just converting between 16-bit and 32-bit or
64-bit formats possibly folded into load and store operators, and full
support for actual 16-bit arithmetic.

128-bit is an interesting question because hardware support for it is very
rare, so it's usually going to be implemented with software emulation anyway,
so there's nothing preventing WebAssembly applications from linking to an
appropriate emulation library and getting similarly performant results.
Emulation libraries would have more flexibility to offer approximation
techniques such as double-double arithmetic. If we standardize 128-bit
floating point in WebAssembly, it will probably be standard IEEE 754-2008
quadruple precision.

## Full IEEE 754-2008 conformance

WebAssembly floating point conforms IEEE 754-2008 in most respects, but there
are a few areas that are
[not yet covered](AstSemantics.md#floating-point-operators).

To support exceptions and alternate rounding modes, one option is to define an
alternate form for each of `add`, `sub`, `mul`, `div`, `sqrt`, and `fma`. These
alternate forms would have extra operands for rounding mode, masked traps, and
old flags, and an extra result for a new flags value. These operators would be
fairly verbose, but it's expected that their use cases will be specialized. This
approach has the advantage of exposing no global (even if only per-thread)
control and status registers to applications, and to avoid giving the common
operators the possibility of having side effects.

Debugging techniques are also important, but they don't necessarily need to be
in the spec itself. Implementations are welcome (and encouraged) to support
non-standard execution modes, enabled only from developer tools, such as modes
with alternate rounding, or evaluation of floating point expressions at greater
precision, to support [techniques for detecting numerical instability]
(https://www.cs.berkeley.edu/~wkahan/Mindless.pdf), or modes using alternate
NaN bitpattern rules, to carry diagnostic information and help developers track
down the sources of NaNs.

To help developers find the sources of floating point exceptions,
implementations may wish to provide a mode where NaN values are produced with
payloads containing identifiers helping programmers locate where the NaNs first
appeared. Another option would be to offer another non-standard execution mode,
enabled only from developer tools, that would enable traps on selected floating
point exceptions, however care should be taken, since not all floating point
exceptions indicate bugs.

## Flushing Subnormal Values to Zero

Many popular CPUs have significant stalls when processing subnormal values,
and support modes where subnormal values are flushed to zero which avoid
these stalls. And, ARMv7 NEON has no support for subnormal values and always
flushes them. A mode where floating point computations have subnormals flushed
to zero in WebAssembly would address these two issues.

## Integer Overflow Detection

There are two different use cases here, one where the application wishes to
handle overflow locally, and one where it doesn't.

When the application is prepared to handle overflow locally, it would be useful
to have arithmetic operators which can indicate when overflow occurred. An
example of this is the checked arithmetic builtins available in compilers such
as
[clang](http://clang.llvm.org/docs/LanguageExtensions.html#checked-arithmetic-builtins)
and
[GCC](https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html).
If WebAssembly is made to support nodes with multiple return values, that could
be used instead of passing a pointer.

There are also several use cases where an application does not wish to handle
overflow locally. One family of examples includes implementing optimized bignum
arithmetic, or optimizing JavaScript Numbers to use int32 operators. Another family
includes compiling code that doesn't expect overflow to occur, but which wishes
to have overflow detected and reported if it does happen. These use cases would
ideally like to have overflow trap, and to allow them to
[handle trap specially][]. Following the rule that explicitly signed and
unsigned operators trap whenever the result value can not be represented in the
result type, it would be possible to add explicitly signed and unsigned versions
of integer `add`, `sub`, and `mul`, which would trap on overflow. The main
reason we haven't added these already is that they're not efficient for
general-purpose use on several of today's popular hardware architectures.

  [handle trap specially]: FutureFeatures.md#trapping-or-non-trapping-strategies

## Better feature testing support

The [MVP feature testing situation](FeatureTest.md) could be improved by
allowing unknown/unsupported AST operators to decode and validate. The runtime
semantics of these unknown operators could either be to trap or call a
same-signature module-defined polyfill function. This feature could provide a
lighter-weight alternative to load-time polyfilling (approach 2 in
[FeatureTest.md](FeatureTest.md)), especially if the [specific layer](BinaryEncoding.md)
were to be standardized and performed natively such that no user-space translation 
pass was otherwise necessary.

## Mutable global variables

In the MVP, there are no global variables; C/C++ global variables are stored in
linear memory and thus accessed through normal
[linear memory operators](AstSemantics.md#linear-memory-operators).
[Dynamic linking](DynamicLinking.md) will add some form of immutable global
variable analogous to "symbols" in native binaries. In some cases, though,
it may be useful to have a fully mutable global variable which lives outside
linear memory. This would allow more aggressive compiler optimizations (due to
better alias information). If globals are additionally allowed array types,
significant portions of memory could be moved out of linear memory which could
reduce fragmentation issues. Languages like Fortran which limit aliasing would be
one use case. C/C++ compilers could also determine that some global variables never
have their address taken.

## Streaming Compilation

The WebAssembly binary format is designed to allow streaming decoding,
validation and compilation. In the MVP, however, the only way to compile
WebAssembly in a browser is through [JS API](JS.md) functions which
require all code to be available in an `ArrayBuffer` before compilation
can begin.

There are two future features that would allow streaming compilation
of WebAssembly in browsers:
* [ES6 Module integration](Modules.md#integration-with-es6-modules) would allow
  the browser's network layer to feed a stream directly into the engine.
* The asynchronous [`WebAssembly.compile`](JS.md#wasmcompile) function could be 
  extended to accept a readable stream (as defined by the 
  [Streams API](https://streams.spec.whatwg.org))
  which would allow the engine to compile the stream as chunks became available,
  fulfilling the promise when the stream was complete. Readable streams
  can come from not just the network (via [`fetch`](http://fetch.spec.whatwg.org/)),
  but also JS stream writers and likely other future Web APIs. Thus, this feature
  would enable Web apps to perform their own (["layer 1"](BinaryEncoding.md))
  custom compression (on top of the spec-defined binary format, under generic
  HTTP `Content-Encoding` compression).

## More Table Operators and Types

In the MVP, [tables](AstSemantics.md#tables) can only store functions
and can only be called. The host-environment can do much more (see, e.g.,
the [JavaScript `WebAssembly.Table` API](JS.md#webassemblytable-objects)),
but it would be useful to be able to do everything from within WebAssembly 
(so, e.g., it was possible to have a dynamic loader written in WebAssembly). As
a prerequisite, WebAssembly would need first-class support for 
[GC references](GC.md) in expressions and locals. Given that, the following could be
added:
* `get_table`/`set_table`: get or set the table element at a given dynamic
  index; the got/set value would have a GC reference type
* `grow_table`: grow the current table (up to the optional maximum), similar to
  `grow_memory`
* `current_table_length`: like `current_memory`.
