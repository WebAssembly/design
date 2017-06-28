# WebAssembly 的长久目标

1. 定义一个[可移植](Portability.md)、体积小、加载快的[二进制格式](MVP.md#binary-format)作为编译结果。通过充分发挥通用硬件的能力（包括[移动设备](https://en.wikipedia.org/wiki/Mobile_device)以及[物联网](https://en.wikipedia.org/wiki/Internet_of_Things)），使其在大多数平台上能达到原生的执行效率。
2. 逐步制定与完善：
    * 主要针对 [C/C++](CAndC++.md)，提供一个和 [asm.js](http://asmjs.org) 有大致相同功能的该标准[最小可行性产品(MVP)](MVP.md)；
    * [其他特性 :unicorn:][未来特性]，首先关注在线程、零成本异常处理和单指令流多数据流等关键功能特性上，然后优先考虑通过反馈和经验总结的其他特性，比如对非 C／C++ 编程语言的支持。
    
3. 被设计为可以与现有的 [Web 平台](Web.md)完美结合并在其中运行：
    * 维护无版本、[特性可测试](FeatureTest.md)、向后兼容的 Web 演变过程；
    * 和 JavaScript 执行在相同的语意环境中；
    * 允许和 JavaScript 相互的同步调用；
    * 严格遵守同源策略以及浏览器安全策略；
    * 和 JavaScript 一样，可以访问相同的 Web API 去调用浏览器的功能；以及
    * 定义一个可与二进制格式相互转化的人类可编辑的文本格式，并且支持查看源码的功能。
    
4. 被设计为也可以支持[非浏览器嵌入](NonWeb.md)的运行形式。
5. 创造一个伟大的平台：
    * 为 WebAssembly 构建一个新的 LLVM 后端环境和伴随的 Clang 端口（[为什么首选 LLVM?](FAQ.md#which-compilers-can-i-use-to-build-webassembly-programs)）；
    * 推广面向 WebAssembly 的其他编译器和工具；以及
    * 启用其他有用的[工具](Tooling.md)。

[未来特性]: FutureFeatures.md
