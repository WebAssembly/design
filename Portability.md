# 可移植性

WebAssembly的[二进制格式](BinaryEncoding.md)是被设计成可在不同操作系统与指令集上高效执行的，无论在[Web](Web.md)或[非Web](NonWeb.md)环境中。

## 对高效执行的设想

尽管执行环境是[有条件的，本地的，不确定的](Nondeterminism.md)，也不要向WebAssembly提供下述特性。有些情况下为了WebAssembly模块执行，也许不得不模拟一些宿主硬件或操作系统不提供的特性，让它们似乎被支持。这种情况将会导致糟糕的性能。

随着WebAssembly的标准化推进，我们期望将这些需求也能被固定下来。WebAssembly在设计之初就应该适配未来的平台。

WebAssembly可移植性假定执行环境提供下列特性：

* 8位字节。
* 内存可按字节维度寻址。
* 支持未对齐的内存访问或因软件仿真导致的已知缺陷。
* 32位有符号整数有两个补码，64位可选。
* IEEE 754-2008 32位和64位浮点数，除了[一些例外](Semantics.md#floating-point-operators)。
* 小端字节序。
* 可使用32位指针或索引有效处理内存区域。
* WebAssembly64 使用[64位指针或索引](FutureFeatures.md#linear-memory-bigger-than-4-gib)额外支持大于4GB的线性内存。
* 执行在同一机器上的WebAssembly模块与其他模块或进程强制安全隔离
* 提供一个进程担保所有线程的执行的执行环境（即便它们以非并行方式执行）
* 访问8、16、32位自然对齐内存时，可用无锁原子运算符。至少也要包含一个原子级的比较和交换运算符（或同等的加载连接/条件存储）。
* WebAssembly64 访问64位自然对齐内存时，额外需要无锁原子运算符。

## API

WebAssembly没有指定任何API或系统调用，只有一个[导入机制](Modules.md)，需宿主环境定义一组可导入对象。[Web](Web.md)环境中，方法通过由[开放Web平台](https://zh.wikipedia.org/wiki/%E5%BC%80%E6%94%BEWeb)制定的Web APIs访问。[非Web](NonWeb.md)环境中可以执行一些基础的Web APIs，基础的非Web APIs（例如POSIX），或者自己发明一个。

## 源码级别

C/C++级别的可移植性可用过面向一个基础API（例如POSIX）编码达成，也可以依赖编译时（通过`#ifdef`）或运行时（通过 [功能发现](FeatureTest.md)）和动态[加载](Modules.md)/[链接](DynamicLinking.md)）编译器和/或库向宿主环境的可用导入们提供的基础接口达成。