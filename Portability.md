# Portability

WebAssembly's [binary format](BinaryEncoding.md) is designed to be executable
efficiently on a variety of operating systems and instruction set architectures,
[on the web](Web.md) and [off the web](NonWeb.md).

Execution environments which, despite
[allowed implementation variants](IncompletelySpecifiedBehavior.md), don't offer
the following characteristics may be able to execute WebAssembly modules
nonetheless. In some cases they may have to emulate behavior that the host
hardware or operating system don't offer so that WebAssembly modules execute
*as-if* the behavior were supported. This sometimes will lead to poor
performance.

As WebAssembly's standardization goes forward we expect to formalize these
requirements, and how WebAssembly will adapt to new platforms that didn't
necessarily exist when WebAssembly was first designed.

WebAssembly portability assumes that execution environments offer the following
characteristics:

* 8-bit bytes.
* Addressable at a byte memory granularity.
* Support unaligned memory accesses or reliable trapping that allows software
  emulation thereof.
* Little-endian byte ordering.
* Up to 4GiB of addressable memory in a 32-bit address space. Heaps bigger than
  4GiB in a 64-bit address space
  [may be added later](FutureFeatures.md#Heaps-bigger-than-4GiB).
* Enforce secure isolation between WebAssembly modules and other modules or
  processes executing on the same machine.
* An execution environment which offers forward progress guarantees to all
  threads of execution (even when executing in a non-parallel manner).

Developer-exposed APIs (such as POSIX) are expected to be portable at a
source-code level through WebAssembly libraries which will use
[feature detection](FeatureTest.md). These libraries aren't necessarily
standardized: WebAssembly will follow the
[extensible web manifesto](https://extensiblewebmanifesto.org)'s lead and expose
low-level capabilities that expose the possibilities of the underlying platform
as closely as possible. WebAssembly therefore standardizes a lower-level
abstraction layer, and expects libraries to offer portable APIs.
