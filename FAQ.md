# FAQ

## Why create a new standard when there is already asm.js?

... especially since pthreads 
[[1](https://blog.mozilla.org/javascript/2015/02/26/the-path-to-parallel-javascript/),
 [2](https://groups.google.com/a/chromium.org/forum/#!topic/blink-dev/d-0ibJwCS24)] 
and SIMD 
[[1](https://hacks.mozilla.org/2014/10/introducing-simd-js/),
 [2](https://groups.google.com/a/chromium.org/forum/#!topic/blink-dev/2PIOEJG_aYY),
 [3](http://discourse.specifiction.org/t/request-for-comments-simd-js-in-asm-js/676)] 
are coming to asm.js.

There are two main benefits WebAssembly provides:

1. The kind of binary format being considered for WebAssembly can be natively
   decoded much faster than JS can be parsed 
   ([experiments show more than 20× faster](BinaryEncoding.md#why-a-binary-encoding-instead-of-a-text-only-representation)).
   On mobile, large compiled codes can easily take 20&ndash;40s *just to parse*, so
   native decoding (especially when combined with other techniques like
   [streaming](https://www.w3.org/TR/streams-api/) for better-than-gzip compression)
   is critical to providing a good cold-load user experience.

2. By avoiding the simultaneous asm.js constraints of
   (1) [AOT](http://asmjs.org/spec/latest/#ahead-of-time-compilation)-[compilability](https://blog.mozilla.org/luke/2014/01/14/asm-js-aot-compilation-and-startup-performance/) 
   and (2) good performance even on engines without 
   [specific asm.js optimizations](https://blog.mozilla.org/luke/2015/02/18/microsoft-announces-asm-js-optimizations/#asmjs-opts),
   a new standard makes it *much easier* to add the [features](FutureFeatures.md)
   required to reach native levels of performance.

Of course, every new standard introduces new costs (maintenance, attack surface,
code size) that must be offset by the benefits. WebAssembly minimizes costs by
having a design that allows (though not requires) a browser to implement
WebAssembly inside its *existing* JS engine (thereby reusing the JS engine's
existing compiler backend, ES6 module loading frontend, security sandboxing
mechanisms and other supporting VM components). Thus, in cost, WebAssembly
should be comparable to a big new JS feature, not a fundamental extension to
the browser model.

Comparing the two, even for engines which already optimize asm.js, the benefits
outweigh the costs.

## What are WebAssembly's use cases?

WebAssembly was designed with [a variety of use cases in mind](UseCases.md).

## Can the polyfill really be efficient?

Yes, this is a [high-level goal](HighLevelGoals.md) and there is a 
[prototype](https://github.com/WebAssembly/polyfill-prototype-1) with demos 
[[1](https://lukewagner.github.io/AngryBotsPacked), 
[2](https://lukewagner.github.io/PlatformerGamePacked)].  Although the 
[binary format](BinaryEncoding.md) is not yet specified in any detail, the format used 
by the prototype has promising initial experimental results. To allow direct comparison with asm.js, 
the prototype has a tool to [pack asm.js](https://github.com/WebAssembly/polyfill-prototype-1/blob/master/src/pack-asmjs.cpp#L3117)
into the prototype's binary format. Using this tool, we can see significant size savings before and 
after <code>gzip</code> compression:

| Demo | asm.js | binary | `gzip` asm.js | `gzip` binary |
|------|--------|--------|---------------|---------------|
| [AngryBots](https://lukewagner.github.io/AngryBotsPacked) | 19MiB | 6.3MiB | 4.1MiB | 3.0MiB |
| [PlatformerGame](https://lukewagner.github.io/PlatformerGamePacked) | 49MiB | 18MiB | 11MiB | 7.3MiB |

By writing the [decoder prototype in C++](https://github.com/WebAssembly/polyfill-prototype-1/blob/611ec5c8c41b08b112cf064ec49b13bf87e400cd/src/unpack.cpp#L2306) 
and Emscripten-compiling to asm.js, the polyfill is able to perform the translation to asm.js
faster than a native JS parser can parse the result (results measured in Firefox 41 
on an Intel® Xeon® E5-2665 @ 2.40GHz):

| Demo  | binary | time to decode into asm.js |
|-------|--------|----------------------------|
| [AngryBots](https://lukewagner.github.io/AngryBotsPacked) | 6.3MiB | 240ms |
| [PlatformerGame](https://lukewagner.github.io/PlatformerGamePacked)  | 18MiB  | 550ms |

Since the polyfill algorithm (at least in the prototype) is simple and single-pass, 
memory usage is basically the size of the input plus the size of the decoded text.

Additionally, there are two further improvements that can be made in the real polyfill:
  1. Decode while downloading using either chunked files, HTTP `Range` requests or (eventually) 
     the [Streams API](https://streams.spec.whatwg.org/).
  2. Include optional better-than-`gzip` compression in the polyfill.  For example, the 
     [lzham](https://github.com/richgel999/lzham_codec) library shows an *additional* 24% 
     improvement over the above "`gzip` binary" figures while maintaining high decode rates.

Extrapolating from the prototype, these extensions would provide a roughly 45% over-the-wire size 
reduction (compared to current gzipped asm.js) without hurting load time (assuming moderate network 
speeds and more than one core).  Developers may even want to switch to WebAssembly with the polyfill 
even before there is any native support.

## Is WebAssembly only for C/C++ programmers?

As explained in the [high-level goals](HighLevelGoals.md), to achieve a Minimum Viable Product, the
initial focus is on [C/C++](CAndC++.md).
However, by [integrating with JS at the ES6 Module interface](MVP.md#modules),
web developers don't need to write C++ to take advantage of libraries that others have written; 
reusing a modular C++ library can be as simple as [using a module from JS](http://jsmodules.io).

Beyond the MVP, another [high-level goal](HighLevelGoals.md) 
is to improve support for languages other than C/C++.  This includes [allowing WebAssembly code to 
allocate and access garbage-collected (JS, DOM, Web API) objects](FutureFeatures.md#gcdom-integration). 
Even before GC support is added to WebAssembly, it is possible to compile a language's VM 
to WebAssembly (assuming it's written in portable C/C++) and this has already been demonstrated 
([1](http://ruby.dj), [2](https://kripken.github.io/lua.vm.js/lua.vm.js.html),
[3](https://syntensity.blogspot.com/2010/12/python-demo.html)).  However, "compile the VM" strategies 
increase the size of distributed code, lose browser devtools integration, can have cross-language
cycle-collection problems and miss optimizations that require integration with the browser.

## What compilers can I use to build WebAssembly programs?

WebAssembly initially focuses on [C/C++](CAndC++.md), and a new, clean
WebAssembly backend is being proposed for upstream clang/LLVM, which can
then be used by LLVM-based projects like [Emscripten](http://emscripten.org)
and [PNaCl](http://gonacl.com).

As WebAssembly evolves it will support more languages than C/C++, and
we hope that other compilers will support it as well, even for the C/C++
language, for example [GCC](https://gcc.gnu.org). The WebAssembly
working group found it easier to start with LLVM support because they had more
experience with that toolchain from their [Emscripten](http://emscripten.org)
and [PNaCl](http://gonacl.com) work.

We hope that proprietary compilers also gain WebAssembly support, but we'll let
vendors speak about their own platform.

The [WebAssembly Community Group][] would be delighted to collaborate with more
compiler vendors, take their input into consideration in WebAssembly itself, and
work with them on ABI matters.

  [WebAssembly Community Group]: https://www.w3.org/community/webassembly/

## Will WebAssembly support View Source on the Web?

Yes! WebAssembly defines a [text format](TextFormat.md) to be rendered when developers view
the source of a WebAssembly module in any developer tool. Also, a specific goal of the text format 
is to allow developers to write WebAssembly modules by hand for testing, experimenting, optimizing, 
learning and teaching purposes. In fact, by dropping all the [coercions required by asm.js 
validation](http://asmjs.org/spec/latest/#introduction), the WebAssembly text format should be much 
more natural to read and write than asm.js. Outside the browser, command-line and online tools that 
convert between text and binary will also be made readily available.  Lastly, a scalable form of 
source maps is also being considered as part of the WebAssembly [tooling story](Tooling.md).

## What's the story for Emscripten users?

Existing Emscripten users will get the option to build their projects to
WebAssembly, by flipping a flag. Initially, Emscripten's asm.js output would
be converted to WebAssembly, but eventually Emscripten would use WebAssembly
throughout the pipeline. This painless transition is enabled by the 
[high-level goal](HighLevelGoals.md) that WebAssembly integrate well with the
Web platform (including allowing synchronous calls into and out of JS) which
makes WebAssembly compatible with Emscripten's current asm.js compilation model.

## Is WebAssembly trying to replace JavaScript?

No! WebAssembly is designed to be a complement to, not replacement of, JavaScript (JS). While WebAssembly
will, over time, allow many languages to be compiled to the Web, JS has an incredible amount of
momentum and will remain the single, privileged (as described [above](FAQ.md#is-webassembly-only-for-cc-programmers)) 
dynamic language of the Web. Furthermore, it is expected that JS and WebAssembly will be used 
together in a number of configurations:
  * Whole, compiled C++ apps that leverage JS to glue things together.
  * HTML/CSS/JS UI around a main WebAssembly-controlled center canvas, allowing developers to 
    leverage the power of web frameworks to build accessible, web-native-feeling experiences.
  * Mostly HTML/CSS/JS app with a few high-performance WebAssembly modules (e.g., graphing, 
    simulation, image/sound/video processing, visualization, animation, compression, etc.,
    examples which we can already see in asm.js today)
    allowing developers to reuse popular WebAssembly libraries just like JS libraries today.
  * When WebAssembly [gains the ability to access garbage-collected objects](FutureFeatures.md#gcdom-integration),
    those objects will be shared with JS, and not live in a walled-off world of their own.

## Why not just use LLVM bitcode as a binary format?

The [LLVM](http://llvm.org/) compiler infrastructure has a lot to recommend it: it has an existing intermediate
representation (LLVM IR) and binary encoding format (bitcode). It has code generation backends targeting
many architectures is actively developed and maintained by a large community. In fact [PNaCl](http://gonacl.com)
already uses LLVM as a basis for its binary format. However the goals and requirements that LLVM was designed
to meet are subtly mismatched with those of WebAssembly.

WebAssembly has several requirements and goals for its IR and binary encoding:
 * Portability: The IR must be the same for every machine architecture.
 * Stability: The IR and binary encoding must not change over time (or change only in ways that can
   be kept backward-compatible).
 * Small encoding: The representation of a program should be as small as possible for transmission over
   the Internet.
 * Fast decoding: The binary format should be fast to decompress and decode for fast startup of programs.
 * Fast compiling: The IR should be fast to compile (and suitable for either AOT- or JIT-compilation) for fast
   startup of programs.
 * Minimal [nondeterminism](Nondeterminism.md): The behavior of programs should be as predictable and
   deterministic as possible (and should be the same on every architecture, a stronger form of the
   portability requirement stated above).

LLVM IR is meant to make compiler optimizations easy to implement, and to represent the constructs
and semantics required by C, C++, and other languages on a large variety of operating systems and
architectures. This means that by default the IR is not portable (the same program has different
representations for different architectures) or stable (it changes over time as optimization and
language requirements change). It has representations for a huge variety of information that is
useful for implementing mid-level compiler optimizations but is not useful for code generation (but
which represents a large surface area for codegen implementers to deal with).  It also has undefined
behavior (largely similar to that of C and C++) which makes some classes of optimization feasible or
more powerful, but which can lead to unpredictable behavior at runtime.  LLVM's binary format
(bitcode) was designed for temporary on-disk serialization of the IR for link-time optimization, and
not for stability or compressibility (although it does have some features for both of those).

None of these problems are insurmountable. For example PNaCl defines a small portable
[subset](https://developer.chrome.com/native-client/reference/pnacl-bitcode-abi) of the IR
with reduced undefined behavior, and a stable version of the bitcode encoding. It also employs several
techniques to improve startup performance. However, each customization, workaround, and special solution
means less benefit from the common infrastructure. We believe that by taking our experience with LLVM and
designing an IR and binary encoding for our goals and requirements, we can do much better than adapting a
system designed for other purposes.

Note that this discussion applies to use of LLVM IR as a standardized format. LLVM's clang frontend
and midlevel optimizers can still be used to generate WebAssembly code from C and C++, and will use
LLVM IR in their implementation similarly to how PNaCl and Emscripten do today.

## Why is there no fast-math mode with relaxed floating point semantics?

Optimizing compilers commonly have fast-math flags which permit the compiler to relax the rules around floating point in order to optimize more aggressively. This can including assuming that NaNs or infinities don't occur, ignoring the difference between negative zero and positive zero, making algebraic manipulations which change how rounding is performed or when overflow might occur, or replacing operations with approximations that are cheaper to compute.

These optimizations effectively introduce nondeterminism; it isn't possible to determine how the code will behave without knowing the specific choices made by the optimizer. This often isn't a serious problem in native code scenarios, because all the nondeterminism is resolved by the time native code is produced. Since most hardware doesn't have floating point nondeterminism, developers have an opportunity to test the generated code, and then count on it behaving consistently for all users thereafter.

WebAssembly implementations run on the user side, so there is no opportunity for developers to test the final behavior of the code. Nondeterminism at this level could cause distributed WebAssembly programs to behave differently in different implementations, or change over time. WebAssembly does have [some nondeterminism](Nondeterminism.md) in cases where the tradeoffs warrant it, but fast-math flags are not believed to be important enough:

 * Many of the important fast-math optimizations happen in the mid-level optimizer of a compiler, before WebAssembly code is emitted. For example, loop vectorization that depends on floating point reassociation can still be done at this level if the user applies the appropriate fast-math flags, so WebAssembly programs can still enjoy these benefits. As another example, compilers can replace floating point division with floating point multiplication by a reciprocal in WebAssembly programs just as they do for other platforms.

 * When WebAssembly [adds an FMA operation](FutureFeatures.md#additional-floating-point-operations), optimizations like folding multiply and add into FMA will be possible by having compilers produce two versions of key code sequences, one with FMA, and one without, and selecting between them using [feature testing](FeatureTest.md).

 * WebAssembly doesn't include its own math functions like sin, cos, exp, pow, and so on. WebAssembly's strategy for such functions is to allow them to be implemented as library routines in WebAssembly itself (note that x86's sin and cos instructions are slow and imprecise and are generally avoided these days anyway). Users wishing to use faster and less precise math functions on WebAssembly can simply select a math library implementation which does so.

 * Most of the individual floating point operations that WebAssembly does have already map to individual fast instructions in hardware. Telling `add`, `sub`, or `mul` they don't have to worry about NaN for example doesn't make them any faster, because NaN is handled quickly and transparently in hardware on all modern platforms.

 * WebAssembly has no floating point traps, status register, dynamic rounding modes, or signalling NaNs, so optimizations that depend on the absense of these features are all safe.
