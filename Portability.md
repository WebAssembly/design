# Portability

WebAssembly's [binary format](BinaryEncoding.md) is designed to be executable
efficiently on a variety of operating systems and instruction set architectures,
[on the web](Web.md) and [off the web](NonWeb.md). The standardization process
will identify *tier 1* platforms on which WebAssembly is expected to be
implementable efficiently in a fully conformant manner, taking into account
[allowed implementation variants](IncompletelySpecifiedBehavior.md).

WebAssembly portability assumes:

* 8-bit bytes.
* Little-endian byte ordering.
* Up to 4GiB of addressable memory in a 32-bit address space.  Heaps bigger than
  4GiB in a 64-bit address space
  [may be added later](FutureFeatures.md#Heaps-bigger-than-4GiB).
* Hardware and operating system with process isolation, memory protection, and
  no-execute support for code pages.
* An execution environment which offers forward progress guarantees to all
  threads of execution (even when executing in a non-parallel manner).

In all of the above cases, an implementation may opt to emulate behavior that
the host hardware or operating system doesn't offer.

Developer-exposed APIs (such as POSIX) are expected to be portable at a
source-code level through WebAssembly libraries which will use
[feature detection](FeatureTest.md). These libraries aren't necessarily
standardized: WebAssembly will follow the
[extensible web manifesto](https://extensiblewebmanifesto.org)'s lead and expose
low-level capabilities that expose the possibilities of the underlying platform
as closely as possible.
