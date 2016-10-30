# WebAssembly Design

This repository contains documents describing the design and high-level overview of WebAssembly.

The documents and discussions in this repository are part of the [WebAssembly Community Group](https://www.w3.org/community/webassembly/).

## Overview

WebAssembly or wasm is a new, portable, size- and load-time-efficient format suitable for compilation to the web.

WebAssembly is currently being designed as an open standard by a [W3C Community Group](https://www.w3.org/community/webassembly/) that includes representatives from all major browsers. *Expect the contents of this repository to be in flux: everything is still under discussion.*

- **WebAssembly is efficient and fast**: Wasm [bytecode](Semantics.md) is designed to be encoded in a size- and load-time-efficient [binary format](BinaryEncoding.md). WebAssembly aims to execute at native speed by taking advantage of [common hardware capabilities](Portability.md#assumptions-for-efficient-execution) available on a wide range of platforms.

- **WebAssembly is safe**: WebAssembly describes a [memory-safe](Security.md#memory-safety), sandboxed [execution environment](Semantics.md#linear-memory) that may even be implemented inside existing JavaScript virtual machines.  When [embedded in the web](Web.md), WebAssembly will enforce the same-origin and permissions security policies of the browser.

- **WebAssembly is open and debuggable**: WebAssembly is designed to be pretty-printed in a [textual format](TextFormat.md) for debugging, testing, experimenting, optimizing, learning, teaching, and writing programs by hand. The textual format will be used when [viewing the source](FAQ.md#will-webassembly-support-view-source-on-the-web) of wasm modules on the web.

- **WebAssembly is part of the open web platform**: WebAssembly is designed to maintain the versionless, feature-tested, and backwards-compatible [nature of the web](Web.md). WebAssembly modules will be able to call into and out of the JavaScript context and access browser functionality through the same Web APIs accessible from JavaScript. WebAssembly also supports [non-web](NonWeb.md) embeddings.

## More Information

| Resource                                   | Repository Location      |
|--------------------------------------------|--------------------------|
| High Level Goals                           | [design/HighLevelGoals.md](HighLevelGoals.md) |
| Frequently Asked Questions                 | [design/FAQ.md](FAQ.md)            |
| Language Specification                     | [spec/README.md](https://github.com/WebAssembly/spec)           |

## Design Process & Contributing

The WebAssembly specification is being developed in the [spec repository](https://github.com/WebAssembly/spec/). For now, high-level design discussions should continue to be held in the design repository, via issues and pull requests, so that the specification work can remain focused.

We've mapped out features we expect to ship:

 1. In [the Minimum Viable Product (MVP)](MVP.md);
 2. Closely [after the MVP](PostMVP.md);
 3. In [future versions](FutureFeatures.md).

Join us:

 * in the [W3C Community Group](https://www.w3.org/community/webassembly/)
 * on IRC: `irc://irc.w3.org:6667/#webassembly`
 * by [contributing](Contributing.md)!

When contributing, please follow our [Code of Ethics and Professional Conduct](CodeOfConduct.md).
