# FAQ

## 为什么在已经有 asm.js 的基础上还要建立一个新的标准？

... 尤其是当 JavaScript 即将迎来 pthreads ([Mozilla pthreads][], [Chromium pthreads][]) 和
SIMD ([simd.js][], [Chromium SIMD][], [simd.js in asm.js][]) 的时候。

  [Mozilla pthreads]: https://blog.mozilla.org/javascript/2015/02/26/the-path-to-parallel-javascript/
  [Chromium pthreads]: https://groups.google.com/a/chromium.org/forum/#!topic/blink-dev/d-0ibJwCS24
  [simd.js]: https://hacks.mozilla.org/2014/10/introducing-simd-js/
  [Chromium SIMD]: https://groups.google.com/a/chromium.org/forum/#!topic/blink-dev/2PIOEJG_aYY
  [simd.js in asm.js]: http://discourse.specifiction.org/t/request-for-comments-simd-js-in-asm-js/676

WebAssembly 具有如下两个主要的优点：

1. 为 WebAssembly 设计的二进制格式可以被原生解码，这比 JavaScript 的解析要快很多（[实验][]表明至少要快20倍以上）。在移动设备上，较大的编译后代码*仅仅解析*就需要占用 20-40 秒，所以原生解码（特别是为了比 gzip 更好的压缩效果时需要与其他技术例如 [streaming][] 结合的时候）对于提供良好的零负载用户体验是非常重要的。

2. 为了避免同步 asm.js 针对 [AOT][]-[编译][]的约束，并保证在没有[针对 asm.js 的特殊优化][]的设备上也有好的性能表现，用一个新的标准使得我们能够*更容易*的添加这些[特性 :unicorn:][future features]，这需要具有原生层面的性能表现能力。

  [实验]: BinaryEncoding.md#why-a-binary-encoding-instead-of-a-text-only-representation
  [streaming]: https://www.w3.org/TR/streams-api/
  [AOT]: http://asmjs.org/spec/latest/#ahead-of-time-compilation
  [编译]: https://blog.mozilla.org/luke/2014/01/14/asm-js-aot-compilation-and-startup-performance/
  [针对 asm.js 的特殊优化]: https://blog.mozilla.org/luke/2015/02/18/microsoft-announces-asm-js-optimizations/#asmjs-opts

当然，每一个新标准势必都会引入抵消其带来便利之外的新的开销（维护成本、攻击面、代码大小等等）。 WebAssembly 通过在设计上允许（尽管不是必须的）浏览器基于*已有*的 JavaScript 引擎（从而可以重用 JavaScript 引擎现有的编译器后端，ES6 在前端的模块化加载，安全沙箱机制和其他支持的 VM 组件）去实现 WebAssembly，来达到最小化其新引入的开销的目的。因此，从根本上说 WebAssembly 应该被认为是 JavaScript 一个重大的新特性，而不是浏览器模型的基础延伸。比较两者，即使是对已经优化过 asm.js 的引擎来说，也是利大于弊的。


## WebAssembly 都有哪些用途？

WebAssembly 在设计的时候就考虑到了[广泛的用途](UseCases.md).


## WebAssembly 是否能实现一些浏览器不支持的原生API (polyfill)?

我们是这样认为的。这里有一个早期[原型](https://github.com/WebAssembly/polyfill-prototype-1)附带演示页面
[[1](https://lukewagner.github.io/AngryBotsPacked), 
[2](https://lukewagner.github.io/PlatformerGamePacked)], 他们展示了从类似 WebAssembly 的二进制格式解码为 asm.js 可以是高效的。而且随着 WebAssembly 设计的变化，[更多](https://github.com/WebAssembly/polyfill-prototype-2) polyfilling 的[例子](https://github.com/WebAssembly/binaryen/blob/master/src/wasm2asm.h)涌现出来。

总体来说，由于浏览器厂商迅速采纳 WebAssembly 使得人们对其发展保持乐观的态度，但这削减了将它作为一个 polyfill 的工作热情。

另一方面也是由于存在替代品，使得将 WebAssembly polyfilling 到 asm.js 变得不是那么紧急，例如，存在一个反向的 polyfill - 能将 [asm.js 编译到 WebAssembly](https://github.com/WebAssembly/binaryen/blob/master/src/asm2wasm.h) -
而且他允许单独构建的版本在 asm.js 或者 WebAssembly 上运行。通过在 emscripten 中[触发开关]((https://github.com/kripken/emscripten/wiki/WebAssembly))，也有可能建立一个拥有 asm.js 和 WebAssembly 两个并行的构建项目，这可以完全避免在客户端的 polyfill 时间消耗。第三点则是，对于非执行代码，可以使用编译的 WebAssembly 解释器例如 [binaryen.js](https://github.com/WebAssembly/binaryen/blob/master/test/binaryen.js/test.js).

即便如此，WebAssembly polyfill 仍然是一个有趣的想法，而且是原则上可行的。

## WebAssembly 只服务于 C/C++ 程序员吗？

正如在[长远目标](HighLevelGoals.md)一章中所说，为了实现最小可行性产品，发展之初会主要关注于 [C/C++](CAndC++.md)。

然而，通过[使用 ES6 的模块化接口集成到 JavaScript 中](Modules.md#integration-with-es6-modules),
web 开发者并不需要通过写 C++ 就可以利用这些别人已经实现的库的优势了；重用一个模块化的 C++ 库就像[在 JavaScript 中调用一个模块](http://jsmodules.io)一样简单。

除 MVP 外，另一个[长远目标](HighLevelGoals.md)是提高 WebAssembly 对 除 C/C++ 之外的编程语言的支持。这包括[允许 WebAssembly 代码去分配和访问垃圾回收 (JavaScript, DOM, Web API) 对象
:unicorn:][future dom]。即使在对垃圾回收对象的支持被添加到 WebAssembly 之前，将一门语言的 VM 编译成 WebAssembly （假设它是由可移植的 C/C++ 编写的）也是可能的，且这已经被证实 ([1](http://ruby.dj), [2](https://kripken.github.io/lua.vm.js/lua.vm.js.html),
[3](https://syntensity.blogspot.com/2010/12/python-demo.html)). 然而，"编译 VM" 的策略增加了分布式代码的大小，缺失了浏览器开发者工具的集成，可能存在有跨语言的循环收集问题以及错过一些需要浏览器集成的优化。

## 当我构建 WebAssembly 程序时该用什么编译器？

WebAssembly 最初关注于 [C/C++](CAndC++.md)，一个新的、干净的 WebAssembly 后端是由 clang/LLVM 开发的，之后可以在基于 LLVM 项目例如 [Emscripten][] 和 [PNaCl][] 中使用。

随着 WebAssembly 的发展，除了 C/C++ 外它会支持越来越多的语言，我们希望其他编译器也能支持它，即使是对 C/C++ 而言，例如 [GCC][]. WebAssembly 工作组发现从支持 LLVM 开始会更加容易，因为他们对来自 [Emscripten][] and [PNaCl][] 工作中的工具链有更多的经验。

  [Emscripten]: http://emscripten.org
  [PNaCl]: http://gonacl.com
  [GCC]: https://gcc.gnu.org

我们希望专有的编译器也可以获得 WebAssembly 的支持，但是对于第三方平台来说，我们让他们自己决定。

[WebAssembly 社区小组][] 会很高兴和更多的编译器平台合作，将他们的付出考虑结合到 WebAssembly 中，和他们共同致力于 ABI 问题。

  [WebAssembly 社区小组]: https://www.w3.org/community/webassembly/


## WebAssembly 在将来会支持通过 Web 查看其源码吗？

是的！WebAssembly 定义了一个用于呈现的[文本格式](TextFormat.md)，这允许开发人员在任何开发者工具中查看 WebAssembly 模块的源码。而且，该文本格式的具体目标是允许开发人员编写用于测试，实验，优化，学习和教学目的的 WebAssembly 模块。事实上，通过摈弃所有 [asm.js 验证所需的强制](http://asmjs.org/spec/latest/#introduction)， WebAssembly 文本格式在读写上会比 asm.js 更加自然。 在浏览器之外，用于文本和二进制相互转换的命令行和在线工具也将随时可用。最后，source maps 的可扩展形式也被考虑成为 WebAssembly
[工具技术](Tooling.md) 之一。


## 对 Emscripten 用户来说有什么新鲜事？

现有的 Emscripten 用户将可以通过配置将他们的项目构建到 WebAssembly. 最初，Emscripten 的 asm.js 输出结果将会转化为 WebAssembly, 但最终 Emscripten 将会在这个处理流程中使用 WebAssembly. 这个平稳的转变过程是通过在[长远目标](HighLevelGoals.md)中设定的目标，即 WebAssembly 和 Web 平台（包括允许同步的和 JavaScript 相互调用）的友好结合来实现的，这使得 WebAssembly 与 Emscripten 当前的 asm.js 编译模型兼容。

## WebAssembly 是否尝试要替代 JavaScript?

不！WebAssembly 是被设计成 JavaScript 的一个完善补充，而不是它的替代品。虽然随着时间的推移，WebAssembly 将能够把很多编程语言都编译到 Web 中，但 JavaScript 不可思议的能力仍将使它保持在 Web 动态语言独一、特有（
[如上所述](FAQ.md#is-webassembly-only-for-cc-programmers)）的地位。此外通过多种配置， JavaScript 将可以和 WebAssembly 一起使用：

* 完整、通过 C++ 编译的应用将利用 JavaScript 将内容结合在一起。
* 围绕一个主要受 WebAssembly 控制的中心画布的 HTML/CSS/JavaScript UI，将会允许开发者利用 web 框架的能力来达到一个可被访问、具有 web 原生的体验。
* 大多数 HTML/CSS/JavaScript 应用结合几个高性能 WebAssembly 模块（例如，绘图，模拟，图像/声音/视频处理，可视化，动画，压缩等等我们今天可以在 asm.js 中看到的例子）能够允许开发者像使用今天我们所用的 JavaScript 库一样去重用流行的 WebAssembly 库。
* 当 WebAssembly
  [获得访问垃圾回收对象的能力 :unicorn:][future dom]时，那些对象将会分享给 JavaScript, 而不是活在自己封闭的世界里。


## 为什么不直接使用 LLVM 位码作为二进制格式？

[LLVM](http://llvm.org/) 的编译器构造有很多地方值得推荐：它具有现成的中间表示 (LLVM IR) 和二进制编码格式 (bitcode). 它有针对许多架构的代码生成后端，并由大型社区积极开发和维护。 事实上，[PNaCl](http://gonacl.com) 已经使用 LLVM 作为二进制格式的基础。然而，LLVM 旨在满足的目标和要求和 WebAssembly 相比会略微不匹配。

WebAssembly 对其指令集架构 (ISA) 和二进制编码有一些要求和目标：

* 可移植性：对于每个机器架构，ISA 必须相同。
* 稳定性：ISA 和二进制编码不能随着时间的推移而改变（或只能以可向后兼容的方式更改）。
* 小编码：程序的表示应尽可能小，以便通过互联网进行传输。
* 快速解码：为了快速启动程序，二进制格式应该快速解压缩和解码。
* 快速编译：ISA 在编译上应该够快（适合于 AOT 或 JIT 编译），以使程序能够快速启动。
* 最小[非确定性](Nondeterminism.md)：程序的行为应尽可能可以预测和具备确定性（在每个架构上应该是相同的，上述提到的可移植性要求的一种更强的形式）。

LLVM IR is meant to make compiler optimizations easy to implement, and to
represent the constructs and semantics required by C, C++, and other languages
on a large variety of operating systems and architectures. This means that by
default the IR is not portable (the same program has different representations
for different architectures) or stable (it changes over time as optimization and
language requirements change). It has representations for a huge variety of
information that is useful for implementing mid-level compiler optimizations but
is not useful for code generation (but which represents a large surface area for
codegen implementers to deal with).  It also has undefined behavior (largely
similar to that of C and C++) which makes some classes of optimization feasible
or more powerful, but which can lead to unpredictable behavior at runtime.
LLVM's binary format (bitcode) was designed for temporary on-disk serialization
of the IR for link-time optimization, and not for stability or compressibility
(although it does have some features for both of those).

None of these problems are insurmountable. For example PNaCl defines a small
portable
[subset](https://developer.chrome.com/native-client/reference/pnacl-bitcode-abi)
of the IR with reduced undefined behavior, and a stable version of the bitcode
encoding. It also employs several techniques to improve startup
performance. However, each customization, workaround, and special solution means
less benefit from the common infrastructure. We believe that by taking our
experience with LLVM and designing an IR and binary encoding for our goals and
requirements, we can do much better than adapting a system designed for other
purposes.

Note that this discussion applies to use of LLVM IR as a standardized
format. LLVM's clang frontend and midlevel optimizers can still be used to
generate WebAssembly code from C and C++, and will use LLVM IR in their
implementation similarly to how PNaCl and Emscripten do today.


## Why is there no fast-math mode with relaxed floating point semantics?

Optimizing compilers commonly have fast-math flags which permit the compiler to
relax the rules around floating point in order to optimize more
aggressively. This can include assuming that NaNs or infinities don't occur,
ignoring the difference between negative zero and positive zero, making
algebraic manipulations which change how rounding is performed or when overflow
might occur, or replacing operators with approximations that are cheaper to
compute.

These optimizations effectively introduce nondeterminism; it isn't possible to
determine how the code will behave without knowing the specific choices made by
the optimizer. This often isn't a serious problem in native code scenarios,
because all the nondeterminism is resolved by the time native code is
produced. Since most hardware doesn't have floating point nondeterminism,
developers have an opportunity to test the generated code, and then count on it
behaving consistently for all users thereafter.

WebAssembly implementations run on the user side, so there is no opportunity for
developers to test the final behavior of the code. Nondeterminism at this level
could cause distributed WebAssembly programs to behave differently in different
implementations, or change over time. WebAssembly does have
[some nondeterminism](Nondeterminism.md) in cases where the tradeoffs warrant
it, but fast-math flags are not believed to be important enough:

 * Many of the important fast-math optimizations happen in the mid-level
   optimizer of a compiler, before WebAssembly code is emitted. For example,
   loop vectorization that depends on floating point reassociation can still be
   done at this level if the user applies the appropriate fast-math flags, so
   WebAssembly programs can still enjoy these benefits. As another example,
   compilers can replace floating point division with floating point
   multiplication by a reciprocal in WebAssembly programs just as they do for
   other platforms.
 * Mid-level compiler optimizations may also be augmented by implementing them
   in a [JIT library](JITLibrary.md) in WebAssembly. This would allow them to
   perform optimizations that benefit from having
   [information about the target](FeatureTest.md) and information about the
   source program semantics such as fast-math flags at the same time. For
   example, if SIMD types wider than 128-bit are added, it's expected that there
   would be feature tests allowing WebAssembly code to determine which SIMD
   types to use on a given platform.
 * When WebAssembly
   [adds an FMA operator :unicorn:][future floating point],
   folding multiply and add sequences into FMA operators will be possible.
 * WebAssembly doesn't include its own math functions like `sin`, `cos`, `exp`,
   `pow`, and so on. WebAssembly's strategy for such functions is to allow them
   to be implemented as library routines in WebAssembly itself (note that x86's
   `sin` and `cos` instructions are slow and imprecise and are generally avoided
   these days anyway). Users wishing to use faster and less precise math
   functions on WebAssembly can simply select a math library implementation
   which does so.
 * Most of the individual floating point operators that WebAssembly does have
   already map to individual fast instructions in hardware. Telling `add`,
   `sub`, or `mul` they don't have to worry about NaN for example doesn't make
   them any faster, because NaN is handled quickly and transparently in hardware
   on all modern platforms.
 * WebAssembly has no floating point traps, status register, dynamic rounding
   modes, or signalling NaNs, so optimizations that depend on the absence of
   these features are all safe.


## What about `mmap`?

The [`mmap`](http://pubs.opengroup.org/onlinepubs/009695399/functions/mmap.html)
syscall has many useful features. While these are all packed into one overloaded
syscall in POSIX, WebAssembly unpacks this functionality into multiple
operators:

* the MVP starts with the ability to grow linear memory via a
  [`grow_memory`](Semantics.md#resizing) operator;
* proposed
  [future features :unicorn:][future memory control] would
  allow the application to change the protection and mappings for pages in the
  contiguous range `0` to `memory_size`.

A significant feature of `mmap` that is missing from the above list is the
ability to allocate disjoint virtual address ranges. The reasoning for this
omission is:

* The above functionality is sufficient to allow a user-level libc to implement
  full, compatible `mmap` with what appears to be noncontiguous memory
  allocation (but, under the hood is just coordinated use of `memory_resize` and
  `mprotect`/`map_file`/`map_shmem`/`madvise`).
* The benefit of allowing noncontiguous virtual address allocation would be if
  it allowed the engine to interleave a WebAssembly module's linear memory with
  other memory allocations in the same process (in order to mitigate virtual
  address space fragmentation). There are two problems with this:

  - This interleaving with unrelated allocations does not currently admit
    efficient security checks to prevent one module from corrupting data outside
    its heap (see discussion in
    [#285](https://github.com/WebAssembly/design/pull/285)).
  - This interleaving would require making allocation nondeterministic and
    nondeterminism is something that WebAssembly generally
    [tries to avoid](Nondeterminism.md).


## Why have wasm32 and wasm64, instead of just an abstract `size_t`?

The amount of linear memory needed to hold an abstract `size_t` would then also
need to be determined by an abstraction, and then partitioning the linear memory
address space into segments for different purposes would be more complex. The
size of each segment would depend on how many `size_t`-sized objects are stored
in it. This is theoretically doable, but it would add complexity and there would
be more work to do at application startup time.

Also, allowing applications to statically know the pointer size can allow them
to be optimized more aggressively. Optimizers can better fold and simplify
integer expressions when they have full knowledge of the bitwidth. And, knowing
memory sizes and layouts for various types allows one to know how many trailing
zeros there are in various pointer types.

Also, C and C++ deeply conflict with the concept of an abstract `size_t`.
Constructs like `sizeof` are required to be fully evaluated in the front-end
of the compiler because they can participate in type checking. And even before
that, it's common to have predefined macros which indicate pointer sizes,
allowing code to be specialized for pointer sizes at the very earliest stages of
compilation. Once specializations are made, information is lost, scuttling
attempts to introduce abstractions.

And finally, it's still possible to add an abstract `size_t` in the future if
the need arises and practicalities permit it.


## Why have wasm32 and wasm64, instead of just using 8 bytes for storing pointers?

A great number of applications don't ever need as much as 4 GiB of memory.
Forcing all these applications to use 8 bytes for every pointer they store would
significantly increase the amount of memory they require, and decrease their
effective utilization of important hardware resources such as cache and memory
bandwidth.

The motivations and performance effects here should be essentially the same as
those that motivated the development of the
[x32 ABI](https://en.wikipedia.org/wiki/X32_ABI) for Linux.

Even Knuth found it worthwhile to give us his opinion on this issue at point,
[a flame about 64-bit pointers](http://www-cs-faculty.stanford.edu/~uno/news08.html).

## Will I be able to access proprietary platform APIs (e.g. Android / iOS)?

Yes but it will depend on the _WebAssembly embedder_. Inside a browser you'll 
get access to the same HTML5 and other browser-specific APIs which are also 
accessible through regular JavaScript. However, if a wasm VM is provided as an 
[“app execution platform”](NonWeb.md) by a specific vendor, it might provide 
access to [proprietary platform-specific APIs](Portability.md#api) of e.g. 
Android / iOS. 

[future features]: FutureFeatures.md
[future dom]: FutureFeatures.md#gcdom-integration
[future floating point]: FutureFeatures.md#additional-floating-point-operators
[future memory control]: FutureFeatures.md#finer-grained-control-over-memory
