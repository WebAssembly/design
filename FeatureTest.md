See [rationale](Rationale.md#feature-testing---motivating-scenarios) for motivating scenarios.

# Feature Test

[PostMVP](PostMVP.md), applications will be able to query which features are
supported via [`has_feature` or a similar API](PostMVP.md#feature-testing). This
accounts for the pragmatic reality that features are shipped in different orders
at different times by different engines.

What follows is a sketch of what such a feature testing capability could look
like.

Since some WebAssembly features add operators and all WebAssembly code in a
module is validated ahead-of-time, the usual JavaScript feature detection
pattern:
```
if (foo)
    foo();
else
    alternativeToFoo();
```
won't work in WebAssembly (if `foo` isn't supported, `foo()` will fail to
validate).

Instead, applications may use one of the following strategies:

1. Compile several versions of a module, each assuming different feature support
   and use `has_feature` tests to determine which version to load.

2. During the ["specific" layer decoding](BinaryEncoding.md), which will happen
   in user code in the MVP *anyway*, use `has_feature` to determine which features
   are supported and then translate unsupported feature use into either a polyfill
   or a trap.

Both of these options could be automatically provided by the toolchain and
controlled by compiler flags. Since `has_feature` is a constant expression,
it can be constant-folded by WebAssembly engines.

To illustrate, consider 4 examples:

* [`i32.min_s`](FutureFeatures.md#additional-integer-operators) - Strategy 2
  could be used to translate `(i32.min_s lhs rhs)` into an equivalent expression
  that stores `lhs` and `rhs` in locals then uses `i32.lt_s` and `select`.
* [Threads](PostMVP.md#threads) - If an application uses `#ifdef` extensively
  to produce thread-enabled/disabled builds, Strategy 1 would be appropriate.
  However, if the application was able to abstract use of threading to a few
  primitives, Strategy 2 could be used to patch in the right primitive 
  implementation.
* [`mprotect`](FutureFeatures.md#finer-grained-control-over-memory) - If engines
  aren't able to use OS signal handling to implement `mprotect` efficiently,
  `mprotect` may become a permanently optional feature. For uses of `mprotect`
  that are not necessary for correctness (but rather just catching bugs),
  `mprotect` could be replaced with `nop`. If `mprotect` was necessary for
  correctness but an alternative strategy existed that did not rely on
  `mprotect`, `mprotect` could be replaced with an `abort()` call, relying on
  the application to test `(has_feature "mprotect")` to avoid calling the
  `abort()`. The `has_feature` query could be exposed to C++ code via
  the existing `__builtin_cpu_supports`.
* [SIMD](PostMVP.md#fixed-width-simd) - When SIMD operators have a good-enough
  polyfill, e.g., `f32x4.fma` via `f32x4.mul`/`add`, Strategy 2 could be used 
  (similar to the `i32.min_s` example above). However, when a SIMD feature has no
  efficient polyfill (e.g., `f64x2`, which introduces both operators *and*
  types), alternative algorithms need to be provided and selected at load time.

As a hypothetical (not implemented) example polyfilling the SIMD `f64x2`
feature, the C++ compiler could provide a new function attribute that indicated
that one function was an optimized, but feature-dependent, version of another
function (similar to the
[`ifunc` attribute](https://gcc.gnu.org/onlinedocs/gcc-4.7.2/gcc/Function-Attributes.html#index-g_t_0040code_007bifunc_007d-attribute-2529),
but without the callback):
```
#include <xmmintrin.h>
void foo(...) {
  __m128 x, y;           // -> f32x4 locals
  ...
  x = _mm_add_ps(x, y);  // -> f32x4.add
  ...
}
void foo_f64x2(...) __attribute__((optimizes("foo","f64x2"))) {
  __m256 x, y;           // -> f64x2 locals
  ...
  x = _m_add_pd(x, y);   // -> f64x2.add
  ...
}
...
foo(...);                 // calls either foo or foo_f64x2
```
In this example, the toolchain could emit both `foo` and `foo_f64x2` as
function definitions in the "specific layer" binary format. The load-time
polyfill would then replace `foo` with `foo_f64x2` if
`(has_feature "f64x2")`. Many other strategies are possible to allow finer or
coarser granularity substitution. Since this is all in userspace, the strategy
can evolve over time.

See also the [better feature testing support](FutureFeatures.md#better-feature-testing-support)
future feature.
