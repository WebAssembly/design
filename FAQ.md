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
  [获得访问垃圾回收对象的能力 :unicorn:][future dom]时，那些对象将会被分享给 JavaScript, 而不是活在自己封闭的世界里。


## 为什么不直接使用 LLVM 位码作为二进制格式？

[LLVM](http://llvm.org/) 的编译器构造有很多地方值得推荐：它具有现成的中间表示 (LLVM IR) 和二进制编码格式 (bitcode). 它有针对许多架构的代码生成后端，并由大型社区积极开发和维护。 事实上，[PNaCl](http://gonacl.com) 已经使用 LLVM 作为二进制格式的基础。然而，LLVM 旨在满足的目标和要求和 WebAssembly 相比会略微不匹配。

WebAssembly 对其指令集架构 (ISA) 和二进制编码有一些要求和目标：

* 可移植性：对于每个机器架构，ISA 必须相同。
* 稳定性：ISA 和二进制编码不能随着时间的推移而改变（或只能以可向后兼容的方式更改）。
* 小编码：程序的表示应尽可能小，以便通过互联网进行传输。
* 快速解码：为了快速启动程序，二进制格式应该快速解压缩和解码。
* 快速编译：ISA 在编译上应该够快（适合于 AOT 或 JIT 编译），以使程序能够快速启动。
* 最小[非确定性](Nondeterminism.md)：程序的行为应尽可能可以预测和具备确定性（在每个架构上应该是相同的，上述提到的可移植性要求的一种更强的形式）。

LLVM IR旨在使编译器的优化易于实现，并可代表在各种各样的操作系统和架构上表示 C，C++ 和其他语言所需的结构和语义。这意味着 IR 默认是不可移植的（同一个程序对于不同架构会有不同的表示）或稳定的（随着时间推移，会随着优化和语言要求的变化而变化）。它对各样的信息表示非常有用，这有利于实现中级编译器优化但对生成代码没有帮助（但它代表了很大一块代码生成/执行者可以处理的内容）。它也有未定义的行为（很大程度上类似于C和C ++），这使得一些优化类可行或更强大，但在运行时可能会导致不可预测的行为。
LLVM 的二进制格式（位代码）被设计用于 IR 临时磁盘序列化的链接时间优化，而不是稳定性或可压缩性（尽管它们确实有两个功能）。

这些问题都不是无法克服的。 例如PNaCl定义了一个小的具有减少未定义行为的IR的可移植[子集](https://developer.chrome.com/native-client/reference/pnacl-bitcode-abi)以及位码编码的稳定版本。它还采用几种技术来改善启动性能。 然而，每个定制、解决方法和特殊解决方案意味着从共同基础设施中获的更少的收益。我们相信利用我们开发 LLVM 的经验和为我们目标和要求设计的 IR 和二进制编码，我们可以比适应以其他目的为设计的系统做得更好。

请注意，本讨论适用于使用 LLVM IR 作为标准化格式。LLVM 的 clang 前端和中级优化器仍然可以使用从 C 和 C++ 生成的 WebAssembly 代码，并在他们的实现中使用 LLVM IR, 就像如今 PNaCl 和 Emscripten 的做法一样。


## 为什么没有松弛浮点语义的 fast-math 模式？

优化编译器通常会允许编译器使用的 fast-math 标志来放宽浮点规则以达到更积极的优化效果。这包括假设 NaN 或者 infinities 不会发生，忽略负零和正零之间的差异，在代数操作时可能遇到的如何执行舍入或溢出、使用近似替换运算符的计算开销会更低。

这些优化有效地引入了非确定性；在不知道优化器具体如何选择的情况下是不可能确定代码会如何表现的。这通常不是本机代码场景中的一个严重问题，因为所有的非确定性都是由本机代码产生时所决定的。由于大多数硬件没有浮点非确定性，开发人员有机会测试生成的代码，然后依靠它使之后所有用户的行为一致。

WebAssembly 的实现在用户一端运行，所以开发者没有机会来测试代码的最终表现。这个层次的非确定性可能导致分布式 WebAssembly 程序在不同的实现下拥有不同的行为表现，或随时间推移而变化。权衡之下，WebAssembly 确实有[一些非确定性](Nondeterminism.md)，但是，fast-math 标记被认为是不够重要的：

 * 在发布 WebAssembly 代码之前，许多重要的 fast-math 优化发生在编译器的中级优化器中。例如，如果用户应用适当的 fast-math 标志，则依赖于浮点重新关联的循环向量化仍然可以在此级别完成，因此 WebAssembly 程序仍然可以享受这些好处。作为另一个例子，编译器可以在 WebAssembly 程序中用浮点乘法替换浮点除数，就像在其他平台上一样。
 * 中级编译器的优化也可以通过在一个 WebAssembly [JIT 库](JITLibrary.md) 中实现而得到增强。这将允许他们执行优化，同时获取[目标信息](FeatureTest.md)和关于源程序语义的信息，例如 fast-math 标记。例如，如果添加了宽于128位的SIMD类型，那么预计会出现允许 WebAssembly 代码来决定在给定平台上要使用哪个 SIMD 类型的功能测试。
 * 当 WebAssembly [添加一个 FMA 操作符 :unicorn:][future floating point]，将有可能折叠乘法和添加序列到 FMA 操作符。
 * WebAssembly 不包括自身的数学函数比如 `sin`, `cos`, `exp`,
   `pow` 等等。对于这些函数，WebAssembly 的策略是允许他们以库程序的方式被实现进 WebAssembly （请注意 x86 架构的 `sin` 和 `cos` 指令是缓慢且不精确的，这些日子以来任何形式下都需要避免使用他们）。希望在 WebAssembly 上使用更快更精确的数学函数的用户可以简单地选择一个符合要求的数学库实现。
* WebAssembly 的大部分浮点运算符已经映射到硬件中的单个快速指令。他们不必担心例如 `add`, `sub` 或者 `mul` 遇到 NaN 还有使它们更快的地方，因为 NaN 在所有现代平台上的硬件上的处理都是速度且透明的。
 * WebAssembly 没有浮点陷阱、状态寄存器、动态舍入模式或 NaN 信号，所以依赖于不存在这些功能的优化都是安全的。


## 那么 `mmap` 又怎样呢？

[`mmap`](http://pubs.opengroup.org/onlinepubs/009695399/functions/mmap.html)
系统调用有很多有用的功能。虽然这些都被打包成POSIX中的一个重载系统调用程序，WebAssembly 将此功能解包为多个运算符：

* 通过 [`grow_memory`](Semantics.md#resizing) 运算符 MVP 开始拥有增长线性内存的能力；
* 提议的[未来功能 :unicorn:][future memory control]将允许应用程序修改保护和将页面映射更改为连续范围 `0` 到 `memory_size` 的值。

上述列表中缺少 `mmap` 的一个重要特征是分配不相交的虚拟地址范围的能力。这种遗漏的原因是：

* 上述功能足以允许用户级别的 libc 去实现完整、兼容的 `mmap`, 这似乎是不连续的内存分配（但是在底层，只是协调使用 `memory_resize` 和 `mprotect`/`map_file`/`map_shmem`/`madvise`）。
* 允许不连续的虚拟地址分配的好处是如果它允许引擎将 WebAssembly 模块的线性内存与其他内存分配在同一进程中进行交错（以减轻虚拟地址空间碎片）。这有两个问题：

  - 这种与无关分配的交错目前尚未进行有效的安全验证，以防止一个模块破坏其堆外的数（参见
    [#285](https://github.com/WebAssembly/design/pull/285) 中的讨论）。
  -这种交错将需要分配非确定性，而非确定性是 WebAssembly 通常尝试[去避免](Nondeterminism.md)的事情。


## 为什么要有 wasm32 和 wasm64, 而不是只有一个抽象的 `size_t`?

维护抽象 `size_t` 所需的线性内存数量也需要通过抽象过程来确定，然后将线性内存地址空间分割成不同块会更加复杂。每块的大小取决于有多少 `size_t` 大小的对象被存储
在里面。这在理论上是可行的，但会增加复杂性并且在应用程序启动时要做更多的工作。

此外，允许应用程序静态地知道指针大小可以允许它们被更激进的优化。当优化器具有对 Bitwidth 的全面了解时，可以更好地折叠和简化整数表达式。而且，知道各种类型的内存大小和布局允许人们知道在各种指针类型中有多少尾随零。

此外，C 和 C++ 与抽象 `size_t` 的概念深深地冲突。因为可以参与到类型检查中，像 `sizeof` 这样的构造需要在编译器前端进行全面的评估。即使在这之前，通常有预定义的宏来指示指针大小，允许代码在最早的汇编阶段专门处理指针大小。一旦专门处理完成，信息就会丢失，其他的尝试会引入抽象

最后，如果需求增长且实用性允许的话，未来仍然有可能添加一个抽象的 `size_t`.


## 为什么要有 wasm32 和 wasm64, 而不是只用 8 字节作为存储指针？

大量应用程序不需要高达4 GiB的内存。强制所有这些应用程序为他们存储的每个指针使用8个字节会显着增加他们需要的内存量，并降低它们利用重要的硬件资源的效率，如缓存、内存和带宽。

这里的动机和表现效果应该和那些为 Linux 推动 [x32 ABI](https://en.wikipedia.org/wiki/X32_ABI) 发展的基本相同。

即便 Knuth 也认为这是值得的，他告诉了我们对这个问题的看法，见 [a flame about 64-bit pointers](http://www-cs-faculty.stanford.edu/~uno/news08.html).

## 我能否使用专有的平台 API（比如说 Android / iOS）？

是的，但是它取决于 _WebAssembly embedder_. 在浏览器中，你可以像使用常规 JavaScript 那样访问相同的 HTML5 和其他浏览器特定的 API. 然而，如果某个第三方实现提供了一个 wasm VM 作为 [“应用执行平台”](NonWeb.md)，它也许提供了对[特定平台 APIs](Portability.md#api) 的访问权限，比如说 Android / iOS. 

[future features]: FutureFeatures.md
[future dom]: FutureFeatures.md#gcdom-integration
[future floating point]: FutureFeatures.md#additional-floating-point-operators
[future memory control]: FutureFeatures.md#finer-grained-control-over-memory
