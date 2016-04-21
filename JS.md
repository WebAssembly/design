# JavaScript API

In the [MVP](MVP.md), the only way to access WebAssembly on the Web is through
an explicit JS API which is defined below. (In the future, WebAssembly may also
be loaded and run directly from an HTML `<script type='module'>` tag—and
any other Web API that loads ES6 modules via URL—as part of 
[ES6 Module integration](Modules.md#integration-with-es6-modules).)

## The `WASM` object

The `WASM` object is the initial value of the `WASM` property of the global
object. Like the `Math` and `JSON` objects, the `WASM` object is a plain
JS object (not a constructor or function) that acts like a namespace and has the
following properties:

### Constructor Properties of the `WASM` object

* `WASM.Module` : see [`WASM.Module` objects](#wasmmodule-objects)
* `WASM.Instance` : see [`WASM.Instance` objects](#wasminstance-objects)
* `WASM.CompileError` : a [NativeError](http://tc39.github.io/ecma262/#sec-nativeerror-object-structure)
   which indicates an error during WebAssembly decoding or validation
* `WASM.RuntimeError` : a [NativeError](http://tc39.github.io/ecma262/#sec-nativeerror-object-structure)
   which indicates an error while running WebAssembly code

### Function Properties of the `WASM` object

#### `WASM.compile`

The `compile` function has the signature:
```
Promise<WASM.Module> compile(BufferSource bytes)
```
If the given `bytes` argument is not a
[`BufferSource`](https://heycam.github.io/webidl/#common-BufferSource),
the returned `Promise` is [rejected](http://tc39.github.io/ecma262/#sec-rejectpromise)
with a `TypeError`.

Otherwise, this function starts an asychronous task to compile a `WASM.Module`
as described in the [`WASM.Module` constructor](#wasmmodule-constructor).
On success, the `Promise` is [fulfilled](http://tc39.github.io/ecma262/#sec-fulfillpromise)
with the resulting `WASM.Module` instance. On failure, the `Promise` is 
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise) with the 
`WASM.CompileError`.

The asynchronous compilation is logically performed on a copy of the state of
the given `BufferSource` captured during the call to `compile`; subsequent mutations
of the `BufferSource` after `compile` return do not affect ongoing compilations.

In the [future](FutureFeatures.md#streaming-compilation), this function can be
extended to accept a [stream](https://streams.spec.whatwg.org), thereby enabling
asynchronous, background, streaming compilation.

## `WASM.Module` Objects

A `WASM.Module` object represents the stateless result of compiling a
WebAssembly binary-format module. A `WASM.Module` logically
contains a single [`Ast.module`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/ast.ml#L211).

### `WASM.Module` Constructor

The `WASM.Module` constructor has the signature:
```
new Module(`BufferSource` bytes)
```
If the given `bytes` argument is not a
[`BufferSource`](https://heycam.github.io/webidl/#common-BufferSource),
a `TypeError` exception is thrown.

Otherwise, this function performs synchronous compilation of the `BufferSource`:
* The byte range delimited by the `BufferSource` is first logically decoded into
  an AST according to [BinaryEncoding.md](BinaryEncoding.md) and then validated
  according to the rules in [spec/check.ml](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/check.ml).
* The spec `string` values inside `Ast.module` are decoded as UTF8 as described in 
  [Web.md](Web.md#function-names).
* On success, a new `WASM.Module` instance is returned that contains the
  validated [`Ast.module`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/ast.ml#L211).
* On failure, a new `WASM.CompileError` is thrown.

### Structured Clone of a `WASM.Module`

A `WASM.Module` is a
[cloneable object](https://html.spec.whatwg.org/multipage/infrastructure.html#cloneable-objects)
which means it can be cloned between windows/workers and also
stored/retrieved into/from an [IDBObjectStore](https://w3c.github.io/IndexedDB/#object-store).
The semantics of a structured clone is as-if the binary source, from which the
`WASM.Module` was compiled, were cloned and recompiled into the target realm.
Engines should attempt to share/reuse internal compiled code when performing
a structured clone although, in corner cases like CPU upgrade or browser
update, this may not be possible and full recompilation may be necessary.

Given the above engine optimizations, structured cloning provides developers
explicit control over both compiled-code caching and cross-window/worker code
sharing.

### `WASM.Module.prototype.instantiate`

The `instantiate` method has the signature:
```
WASM.Instance instantiate([importObject])
```
If the `this` value of `instantiate` is not a `WASM.Module` instance, a
`TypeError` is thrown.

Let `m` be the [`Ast.module`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/ast.ml#L211)
associated with the `WASM.Module`.

If the `importObject` parameter is not `undefined` and `Type(importObject)` is
not Object, a `TypeError` is thrown. If the list of 
[`m.imports`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/ast.ml#L218)
is not empty and `Type(importObject)` is not Object, a `TypeError` is thrown.

Let `imports` by an initially-empty list of JS functions.

For each [`import`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/kernel.ml#L133)
`i` in `m.imports`:
* Let `v` be the resultant value of performing
  [`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`importObject`, [`i.module_name`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/kernel.ml#L137)).
* If [`i.func_name`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/kernel.ml#L138)
  is not the empty string:
  * If `Type(v)` is not Object, throw a `TypeError`.
  * Let `v` instead be the value of performing [`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`v`, `i.func_name`)
* If `IsCallable(v)` is `false`, throw a `TypeError`.
* Append `v` to `imports`.

Let `instance` be the result of evaluating 
[`Eval.init`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/eval.ml#L314)
with arguments `m` and `imports`.
Note: this synchronously executes the [`start`](Modules.md#module-start-function)
function, if present.

Let `exportObject` be a new, plain empty JS object.

For each [exported function](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/kernel.ml#L130)
`f` in `m.exports`:
* Let `index` be the function index of `f`.
* If `f` is the first export of `index`, let `v` be a new 
  [Exported Function](#exported-function-exotic-objects) Exotic Object given `instance` and `index`.
* Otherwise, let `v` be the previously-created Exported Function Exotic Object.
* Add a plain data property to `exportObject` whose property name is given by `f` and
  whose value is `v`.

Return a new `WASM.Instance` instance containing `instance` where the initial value of
the [`exports`](#wasminstance-exports-property) data property is
`exportObject`.

## Exported Function Exotic Objects

Functions exported by WebAssembly modules are reflected in JS via a new kind
of *Exported Function* 
[Exotic Object](http://tc39.github.io/ecma262/#sec-built-in-exotic-object-internal-methods-and-slots).
Like [Bound Function](http://tc39.github.io/ecma262/#sec-bound-function-exotic-objects) Exotic Object,
Exported Functions do not have the normal function internal slots but instead have:
 * [[Instance]] : the [`Eval.instance`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/eval.ml#L15)
   containing the exported function
 * [[FunctionIndex]] : the index of the function inside the module

as well as the internal slots required of all builtin functions:
 * [[Prototype]] : [%FunctionPrototype%](http://tc39.github.io/ecma262/#sec-well-known-intrinsic-objects)
 * [[Extensible]] : `true`
 * [[Realm]] : the [current Realm Record](http://tc39.github.io/ecma262/#current-realm)
 * [[ScriptOrModule]] : [`GetActiveScriptOrModule`](http://tc39.github.io/ecma262/#sec-getactivescriptormodule)

Exported Functions also have the following data properties:
* the `length` property is set to the exported function's signature's arity 
* the `name` is set to `index` as a Number value

WebAssembly Exported Functions have a `[[Call]](this, argValues)` method defined as:
 * Let `argTypes` be the list of value types defined by the signature of [[FunctionIndex]].
 * Let `args` be an empty list of coerced values.
 * For each value type `t` in `argTypes` and value `v` in `argValues`:
   * Append to `args` `v` coerced to `t` as follows:
     * coerce `v` to `i32` via [`ToInt32(v)`](http://tc39.github.io/ecma262/#sec-toint32)
     * throw a `TypeError` if `t` is `i64`
     * coerce `v` to `f32` by first applying [`ToNumber(v)`](http://tc39.github.io/ecma262/#sec-tonumber)
       and then converting the resulting IEEE754 64-bit double to a 32-bit float using `roundTiesToEven`
     * coerce `v` to `f64` via [`ToNumber(v)`](http://tc39.github.io/ecma262/#sec-tonumber)
 * Perform [`Eval.invoke`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/eval.ml#L327)
   passing [[Instance]], [[FunctionIndex]], and `args`.
 * Coerce the result of `Eval.invoke` as follows:
   * if the return value is `None`, return `undefined`
   * interpret `i32` as a signed integer and convert that integer to a Number value
   * throw a `TypeError` if returning an `i64`
   * return `f32`/`f64` as Number values, possibly performing
     [canonicalization of NaNs](http://tc39.github.io/ecma262/#sec-setvalueinbuffer)

Exported Functions do not have a [[Construct]] method and thus it is not possible to 
call one with the `new` operator.

## `WASM.Instance` Objects

A `WASM.Instance` object represents the activation/instantiation of a single
`WASM.Module` into a single [realm](http://tc39.github.io/ecma262/#sec-code-realms)
with a tuple of realm-specific JS import values. A `WASM.Instance` logically
contains a single [`Eval.instance`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/eval.ml#L15).

### `WASM.Instance` Constructor

Attempting to call `WASM.Instance` as a function or constructor directly from JS
throws a `TypeError`. `WASM.Instance` instances can only be created indirectly from
JS by calling [`instantiate`](JS.md#wasmmoduleprototypeinstantiate).

### `WASM.Instance` `exports` property

`WASM.Instance` objects are created with an initial (writable, configurable,
non-enumerable) `exports` data property whose value is the JS Array of exported
JS Functions created by [`instantiate`](JS.md#wasmmoduleprototypeinstantiate).

## Sample API Usage

Given `demo.was` (encoded to `demo.wasm`):
```lisp
(module
    (import $i1 "m" "import1")
    (import $i2 "import2" "")
    (func $main (call_import $i1))
    (start $main)
    (func $f (call_import $i2))
    (export "f" $f)
)
```
and the following JavaScript, run in a browser:
```javascript
fetch('demo.wasm').then(response =>
    response.arrayBuffer()
).then(buffer =>
    WASM.compile(new Uint8Array(buffer))
).then(module => {
    var importObj = {
        m: {import1: () => console.log("hello, ")},
        import2: () => console.log("world!\n")
    };
    var instance = module.instantiate(importObj); // "hello, "
    instance.exports.f(); // "world!"
});
```

## TODO

* `WASM.Memory`: imports, exports
* `WASM.Table`: imports, exports
* `WASM.Module` `exports`/`imports` properties (reflection)
