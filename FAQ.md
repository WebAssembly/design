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
   streaming) is critical to providing a good cold-load user experience.

2. A new standard makes it *much easier* to add 
   [new features](FutureFeatures.md) by avoiding the extremely nuanced requirements of
   [AOT](http://asmjs.org/spec/latest/#ahead-of-time-compilation)-[compilability](https://blog.mozilla.org/luke/2014/01/14/asm-js-aot-compilation-and-startup-performance/) 
   necessary for asm.js combined with the requirement that asm.js run well on
   all browsers (including those that do not have specific asm.js optimizations).

Of course, every new standard introduces new costs (maintenance, attack surface,
code size) that must be offset by the benefits. WebAssembly minimizes costs by
having a design that allows (though not requires) a browser to implement
WebAssembly inside its *existing* JS engine (thereby reusing the JS engine's
existing compiler backend, ES6 module loading frontend, security sandboxing
mechanisms and other supporting VM components). Thus, in cost, WebAssembly
should be comparable to a big new JS feature, not a fundamental extension to
the browser model.

Comparing the two, even for engines which already 
[optimize asm.js](https://blog.mozilla.org/luke/2015/02/18/microsoft-announces-asm-js-optimizations/#asmjs-opts),
the benefits outweigh the costs.

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
     the [Stream API](https://www.w3.org/TR/streams-api/).
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

WebAssembly initially focuses on [C/C++](CAndC++.md), and an experimental
backend is being built using [clang/LLVM](http://llvm.org). As WebAssembly
evolves it will support more languages which often use non-LLVM compilers.

Even for C/C++ language support we hope that other compilers, such as
[GCC](https://gcc.gnu.org), gain support for WebAssembly. The WebAssembly
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

Existing Emscripten users will get the option to build their projects to WebAssembly, by
flipping a flag. Initially this will be using a [polyfill](Polyfill.md), converting
Emscripten's asm.js output to WebAssembly, and eventually Emscripten will utilize
the [WebAssembly LLVM backend](https://github.com/WebAssembly/llvm) project, which aims to
create a clean, new WebAssembly backend for LLVM (the open source compiler that powers Emscripten) with the
intention of submitting to upstream. This painless transition is enabled by the
[high-level goal](HighLevelGoals.md) that WebAssembly integrate well with the Web platform (including 
allowing synchronous calls into and out of JS) which makes WebAssembly compatible with Emscripten's 
current asm.js compilation model.

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

