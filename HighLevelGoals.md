# WebAssembly High-Level Goals

1. Define a [portable](Portability.md), size- and load-time-efficient
   [binary format](MVP.md#binary-format) to serve as a compilation target which
   can be compiled to execute at native speed by taking advantage of common
   hardware capabilities available on a wide range of platforms, including
   [mobile](https://en.wikipedia.org/wiki/Mobile_device) and
   [IoT](https://en.wikipedia.org/wiki/Internet_of_Things).
2. Specify and implement incrementally:
    * develop new features as independent [proposals];
    * maintain [layering], with a core spec focused on pure sandboxed
      computation, with host interactions factored out into higher
      specification layers;
    * preserve backwards compatibility;
    * prioritize new features according to feedback and experience; and
    * avoid biasing towards any one programming language family.
3. Design to execute within and integrate well with the *existing*
   [Web platform](Web.md):
    * maintain the versionless, [feature-tested](FeatureTest.md) and backwards-compatible evolution story of the Web;
    * execute in the same semantic universe as JavaScript;
    * allow synchronous calls to and from JavaScript;
    * enforce the same-origin and permissions security policies;
    * access browser functionality through the same Web APIs that are accessible
      to JavaScript; and
    * define a human-editable text format that is convertible to and from the
      binary format, supporting View Source functionality.
4. Design to support [non-browser embeddings](NonWeb.md) as well.
5. Make a great platform:
    * promote [compilers and tools targeting WebAssembly];
    * enable other useful [tooling](Tooling.md);
    * maintain a high level of [determinism]; and
    * specify using [formal semantics].

[compilers and tools targeting WebAssembly]: https://webassembly.org/getting-started/developers-guide/
[proposals]: https://github.com/WebAssembly/proposals/
[layering]: https://webassembly.org/specs/
[determinism]: Nondeterminism.md
[formal semantics]: https://github.com/WebAssembly/spec
