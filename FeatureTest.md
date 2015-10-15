# Feature Test

The [MVP](MVP.md) allows an application to query which post-MVP features are
supported via [`has_feature`](AstSemantics.md#feature-test). This accounts for
the pragmatic reality that features are shipped in different orders at different
times by different engines on the Web.

Since some WebAssembly features add operators and all WebAssembly code in a
module is validated ahead-of-time, the usual JS feature detection pattern:
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

2. During [layer 1 decoding](BinaryEncoding.md), which will happen in user code
   in the MVP *anyway*, use `has_feature` to determine which features are
   supported and then translate unsupported feature use into either a polyfill
   or a trap.

To illustrate, consider 3 representative examples:

* [`i32.min_s`](FutureFeatures.md#additional-integer-operations) - Strategy 2
  could be used to translate `(i32.min_s lhs rhs)` into an equivalent expression
  that stores `lhs` and `rhs` in locals then uses `i32.lt_s` and `conditional`.
* [Threads](PostMVP.md#threads) - If an application uses `#ifdef` extensively
  to produce thread-enabled/disabled builds, Strategy 1 would be appropriate.
  However, if the application was able to abstract use of threading to a few
  primitives, Strategy 2 could be used to patch in the right primitive.
* [`mprotect`](FutureFeatures.md#finer-grained-control-over-memory) - If engines
  aren't able to use OS signal handling to implement `mprotect` efficiently,
  `mprotect` may become a permanently optional feature. For uses of `mprotect`
  that are not necessary for correctness (but rather just catching bugs),
  `mprotect` could be replaced with `nop`. If `mprotect` was necessary for
  correctness but an alternative strategy existed that did not rely on
  `mprotect`, `mprotect` could be replaced with an `abort()` call, relying on
  the application to test `(has_feature "mprotect")` to avoid calling the
  `abort()`.

See also the [better feature testing support](FutureFeatures.md#better-feature-testing-support)
future feature.
