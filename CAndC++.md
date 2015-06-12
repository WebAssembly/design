# Guide for C/C++ developers.

WebAssembly is being designed to support C and C++ code well, right from
the start in [the MVP](MVP.md). The following explains the outlook for
C and C++ developers.

## Porting C and C++ code to WebAssembly

### Platform features

WebAssembly has a pretty conventional ISA. 8-bit bytes, two's complement
integers, little-endian, and a lot of other normal properties. Reasonably
portable C/C++ code should port to WebAssembly without difficultly.

In [the MVP](MVP.md), WebAssembly will have an ILP32 data model, meaning
that `int`, `long`, and pointer types are all 32-bit. The `long long`
type is 64-bit.

In the future, WebAssembly will be extended to support
[64-bit address spaces](FutureFeatures.md#Heaps-bigger-than-4GiB). This
will enable an LP64 data model as well, meaning that `long` and pointer
types will be 64-bit, while `int` is 32-bit. From a C/C++ perspective,
this will be a separate mode from ILP32, with a separate ABI.

### APIs

Libraries providing high-level C/C++ APIs such as the C and C++ standard
libraries, OpenGL, SDL, pthreads, and others are being developed to
support normal C/C++ development. Under the covers, these libraries will
implement their functionality by using low-level facilities provided by
WebAssembly implementations. On [the Web](Web.md), they will utilize
Web APIs. [In other contexts](NonWeb.md), other low-level mechanisms may
be used.

### ABIs

In [the MVP](MVP.md), WebAssembly does not yet have a stable ABI for
libraries. Developers will need to ensure that all code linked into an
application are compiled with the same compiler and options.

In the future, when WebAssembly is extended to support
[dynamic linking](FutureFeatures.md#dynamic-linking), stable ABIs are
expected to be defined in accompaniment.

### Undefined and Implementation-defined Behavior

#### Undefined Behavior

WebAssembly doesn't change the C or C++ languages. Things which cause
undefined behavior in C or C++ are still bugs when compiling for WebAssembly
[even when the corresponding behavior in WebAssembly itself is defined]
(Nondeterminism.md#note-for-users-of-c-c-and-similar-languages). C and C++
optimizers still assume that undefined behavior won't occur, so such bugs
can still lead to surprising behavior.

[Tools are being developed and ported](Tooling.md) to help developers find
and fix such bugs in their code.

#### Implementation-Defined Behavior

Most implementation-defined behavior is dependent on the compiler, rather
than on the underlying platform, but for those details that are dependent on
the platform, they follow naturally from WebAssembly's 8-bit bytes, 32-bit and
64-bit two's complement integers, and
[32-bit and 64-bit IEEE-754-style floating point support]
(AstSemantics.md#floating-point-operations).

## Portability of compiled code

WebAssembly can be efficiently implemented on a wide variety of platforms,
provided they can satisfy certain
[basic expectations](Portability.md#assumptions-for-efficient-execution).

WebAssembly has very limited [nondeterminism](Nondeterminism.md), so it is
expected that compiled WebAssembly programs will behave very consistently
across different implementations, and across different versions of the same
implementation.
