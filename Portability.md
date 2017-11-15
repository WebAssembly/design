# Portability

WebAssembly's [binary format](BinaryEncoding.md) is designed to be executable
efficiently on a variety of operating systems and instruction set architectures,
[on the Web](Web.md) and [off the Web](NonWeb.md).

## Assumptions for Efficient Execution

Execution environments which, despite
[limited, local, nondeterminism](Nondeterminism.md), don't offer
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
* Two's complement signed integers in 32 bits and optionally 64 bits.
* IEEE 754-2008 32-bit and 64-bit floating point, except for
  [a few exceptions](Semantics.md#floating-point-operators).
* Little-endian byte ordering.
* Memory regions which can be efficiently addressed with 32-bit
  pointers or indices.
* wasm64 additionally supports linear memory bigger than
  [4 GiB with 64-bit pointers or indices][future 64-bit].
* Enforce secure isolation between WebAssembly modules and other modules or
  processes executing on the same machine.
* An execution environment which offers forward progress guarantees to all
  threads of execution (even when executing in a non-parallel manner).
* Availability of lock-free atomic memory operators, when naturally aligned, for
  8- 16- and 32-bit accesses. At a minimum this must include an atomic
  compare-and-exchange operator (or equivalent load-linked/store-conditional).
* wasm64 additionally requires lock-free atomic memory operators, when naturally
  aligned, for 64-bit accesses.

## API

WebAssembly does not specify any APIs or syscalls, only an 
[import mechanism](Modules.md) where the set of available imports is defined
by the host environment. In a [Web](Web.md) environment, functionality is
accessed through the Web APIs defined by the
[Web Platform](https://en.wikipedia.org/wiki/Open_Web_Platform).
[Non-Web](NonWeb.md) environments can choose to implement standard Web APIs,
standard non-Web APIs (e.g. POSIX), or invent their own.

## Source-level

Portability at the C/C++ level can be achieved by programming to
a standard API (e.g., POSIX) and relying on the compiler and/or libraries to map
the standard interface to the host environment's available imports either at
compile-time (via `#ifdef`) or run-time (via [feature detection](FeatureTest.md)
and dynamic [loading](Modules.md)/[linking](DynamicLinking.md)).

[future 64-bit]: FutureFeatures.md#linear-memory-bigger-than-4-gib
