# Guide for C/C++ developers

WebAssembly is being designed to support C and C++ code well, right from
the start in [the MVP](MVP.md). The following explains the outlook for
C and C++ developers.

## Porting C and C++ code to WebAssembly

### Platform features

WebAssembly has a pretty conventional ISA: 8-bit bytes, two's complement
integers, little-endian, and a lot of other normal properties. Reasonably
portable C/C++ code should port to WebAssembly without difficultly.

WebAssembly has 32-bit and 64-bit architecture variants, called wasm32 and
wasm64. wasm32 has an ILP32 data model, meaning that `int`, `long`, and
pointer types are all 32-bit, while the `long long` type is 64-bit. wasm64
has an LP64 data model, meaning that `long` and pointer types will be
64-bit, while `int` is 32-bit.

[The MVP](MVP.md) will support only wasm32; support for wasm64 will be
added in the future to support
[64-bit address spaces :unicorn:][future 64-bit].

`float` and `double` are the IEEE 754-2008 single- and double-precision types,
which are native in WebAssembly. `long double` is the IEEE 754-2008
quad-precision type, which is a software-emulated type. WebAssembly does
not have a builtin quad-precision type or associated operators. The long
double type here is software-emulated in library code linked into WebAssembly
applications that need it.

For performance and compatibility with other platforms, `float` and
`double` are recommended for most uses.

### Language Support

C and C++ language conformance is largely determined by individual compiler
support, but WebAssembly includes all the functionality that popular C and C++
compilers need to support high-quality implementations.

While [the MVP](MVP.md) will be fully functional, additional features enabling
greater performance will be added soon after, including:

 * [Support for multi-threaded execution with shared memory](FutureFeatures.md#threads).

 * [Zero-cost C++ exception handling](FutureFeatures.md#zero-cost-exception-handling).
   C++ exceptions can be implemented without this, but this feature will
   enable them to have lower runtime overhead.

 * Support for [128-bit SIMD](FutureFeatures.md#fixed-width-simd). SIMD will be
   exposed to C/C++ though explicit APIs such as [LLVM's vector extensions]
   and [GCC's vector extensions], auto-vectorization, and emulated APIs from
   other platforms such as `<xmmintrin.h>`.

  [LLVM's vector extensions]: http://clang.llvm.org/docs/LanguageExtensions.html#vectors-and-extended-vectors
  [GCC's vector extensions]: https://gcc.gnu.org/onlinedocs/gcc/Vector-Extensions.html

### APIs

WebAssembly applications can use high-level C/C++ APIs such as the C
and C++ standard libraries, OpenGL, SDL, pthreads, and others, just as
in normal C/C++ development. Under the covers, these libraries
implement their functionality by using low-level facilities provided by
WebAssembly implementations. On [the Web](Web.md), they utilize
Web APIs (for example, OpenGL is executed on WebGL, libc date and
time methods use the browser's Date functionality, etc.).
[In other contexts](NonWeb.md), other low-level mechanisms may be used.

### ABIs

In [the MVP](MVP.md), WebAssembly does not yet have a stable ABI for
libraries. Developers will need to ensure that all code linked into an
application are compiled with the same compiler and options.

In the future, when WebAssembly is extended to support
[dynamic linking](DynamicLinking.md), stable ABIs are
expected to be defined in accompaniment.

### Undefined and Implementation-defined Behavior

#### Undefined Behavior

WebAssembly doesn't change the C or C++ languages. Things which cause
undefined behavior in C or C++ are still bugs when compiling for WebAssembly
[even when the corresponding behavior in WebAssembly itself is defined](Nondeterminism.md#note-for-users-of-c-c-and-similar-languages).
C and C++ optimizers still assume that undefined behavior won't occur,
so such bugs can still lead to surprising behavior.

For example, while unaligned memory access is
[fully defined](Semantics.md#alignment) in WebAssembly, C and C++ compilers
make no guarantee that a (non-packed) unaligned memory access at the source
level is harmlessly translated into an unaligned memory access in WebAssembly.
And in practice, popular C and C++ compilers do optimize on the assumption that
alignment rules are followed, meaning that they don't always preserve program
behavior otherwise.

On WebAssembly, the primary [nondeterminism](Nondeterminism.md) and
[security](Security.md) invariants are always maintained. Demons can't actually
fly out your nose, as that would constitute an escape from the sandbox. And,
callstacks can't become corrupted.

Other than that, programs which invoke undefined behavior at the source language
level may be compiled into WebAssembly programs which do anything else,
including corrupting the contents of the application's linear memory, calling APIs with
arbitrary parameters, hanging, trapping, or consuming arbitrary amounts of
resources (within the limits).

[Tools are being developed and ported](Tooling.md) to help developers find
and fix such bugs in their code.

#### Implementation-Defined Behavior

Most implementation-defined behavior in C and C++ is dependent on the compiler
rather than on the underlying platform. For those details that are dependent
on the platform, on WebAssembly they follow naturally from having 8-bit bytes,
32-bit and 64-bit two's complement integers, and
[32-bit and 64-bit IEEE-754-2008-style floating point support](Semantics.md#floating-point-operators).

## Portability of compiled code

WebAssembly can be efficiently implemented on a wide variety of platforms,
provided they can satisfy certain
[basic expectations](Portability.md#assumptions-for-efficient-execution).

WebAssembly has very limited [nondeterminism](Nondeterminism.md), so it is
expected that compiled WebAssembly programs will behave very consistently
across different implementations, and across different versions of the same
implementation.

[future 64-bit]: FutureFeatures.md#linear-memory-bigger-than-4-gib

