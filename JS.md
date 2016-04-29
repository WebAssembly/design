# JavaScript API

In the [MVP](MVP.md), the only way to access WebAssembly on the Web is through
an explicit JS API which is defined below. (In the future, WebAssembly may also
be loaded and run directly from an HTML `<script type='module'>` tag—and
any other Web API that loads ES6 modules via URL—as part of 
[ES6 Module integration](Modules.md#integration-with-es6-modules).)

*Note: current experimental WebAssembly implementations expose a single
all-in-one function `Wasm.instantiateModule(bytes, imports)` which is used
by the current [demo](http://webassembly.github.io/demo). This function is
basically equivalent to `new WASM.Instance(new WASM.Module(bytes), imports)`
as defined below and will be removed at some point in the future.*

## The `WASM` object

The `WASM` object is the initial value of the `WASM` property of the global
object. Like the `Math` and `JSON` objects, the `WASM` object is a plain
JS object (not a constructor or function) that acts like a namespace and has the
following properties:

### Constructor Properties of the `WASM` object

The following intrinsic objects are added:

* `WASM.Module` : the [`WASM.Module` constructor](#wasmmodule-constructor)
* `WASM.Instance` : the [`WASM.Instance` constructor](#wasminstance-constructor)
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
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise) with a 
`WASM.CompileError`.

The asynchronous compilation is logically performed on a copy of the state of
the given `BufferSource` captured during the call to `compile`; subsequent mutations
of the `BufferSource` after `compile` return do not affect ongoing compilations.

In the [future](FutureFeatures.md#streaming-compilation), this function can be
extended to accept a [stream](https://streams.spec.whatwg.org), thereby enabling
asynchronous, background, streaming compilation.

## `WASM.Module` Objects

A `WASM.Module` object represents the stateless result of compiling a
WebAssembly binary-format module and contains one internal slot:
 * [[Module]] : an [`Ast.module`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/ast.ml#L208)
   which is the spec definition of a validated module AST

### `WASM.Module` Constructor

The `WASM.Module` constructor has the signature:
```
new Module(BufferSource bytes)
```
If the NewTarget is `undefined`, a `TypeError` exception is thrown (i.e., this
constructor cannot be called as a function without `new`).

If the given `bytes` argument is not a
[`BufferSource`](https://heycam.github.io/webidl/#common-BufferSource),
a `TypeError` exception is thrown.

Otherwise, this function performs synchronous compilation of the `BufferSource`:
* The byte range delimited by the `BufferSource` is first logically decoded into
  an AST according to [BinaryEncoding.md](BinaryEncoding.md) and then validated
  according to the rules in [spec/check.ml](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/check.ml#L325).
* The spec `string` values inside `Ast.module` are decoded as UTF8 as described in 
  [Web.md](Web.md#function-names).
* On success, a new `WASM.Module` instance is returned with [[Module]] set to
  the validated `Ast.module`.
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

## `WASM.Instance` Objects

A `WASM.Instance` object represents the instantiation of a `WASM.Module`
into a [realm](http://tc39.github.io/ecma262/#sec-code-realms) and has one
internal slot:
* [[Instance]] : an [`Eval.instance`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/eval.ml#L15)
  which is the WebAssembly spec definition of an instance

as well as one plain data property (configurable, writable, enumerable)
added by the constructor:
* exports : a [Module Namespace Object](http://tc39.github.io/ecma262/#sec-module-namespace-objects)

### `WASM.Instance` Constructor

The `WASM.Instance` constructor has the signature:
```
new Instance(moduleObject [, importObject])
```
If the NewTarget is `undefined`, a `TypeError` exception is thrown (i.e., this
constructor cannot be called as a function without `new`).

If `moduleObject` is not a `WASM.Module` instance, a `TypeError` is thrown.

Let `module` be the [`Ast.module`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/ast.ml#L211)
`moduleObject.[[Module]]`.

If the `importObject` parameter is not `undefined` and `Type(importObject)` is
not Object, a `TypeError` is thrown. If the list of 
[`module.imports`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/ast.ml#L215)
is not empty and `Type(importObject)` is not Object, a `TypeError` is thrown.

Let `imports` by an initially-empty list of JS functions.

For each [`import`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/kernel.ml#L135)
`i` in `module.imports`:
* Let `v` be the resultant value of performing
  [`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`importObject`, [`i.module_name`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/kernel.ml#L139)).
* If [`i.func_name`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/kernel.ml#L140)
  is not the empty string:
  * If `Type(v)` is not Object, throw a `TypeError`.
  * Let `v` instead be the value of performing [`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`v`, `i.func_name`)
* If `IsCallable(v)` is `false`, throw a `TypeError`.
* Append `v` to `imports`.

Let `instance` be the result of evaluating 
[`Eval.init`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/eval.ml#L314)
with arguments `module` and `imports`.
Note: this synchronously executes the [`start`](Modules.md#module-start-function)
function, if present.

Let `exports` be an initially-empty list of (string, JS function) pairs.
Let `exportedFunctions` be an initially-empty map from function indices (integers) to
JS functions.

For each [exported function](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/kernel.ml#L128)
`f` in `module.exports`:
* Let `index` be the exported function index of `f`.
* If `index` is not already present in `exportedFunctions`, add a mapping
  from `index` to the result of creating a new 
  [Exported Function](#exported-function-exotic-objects) Exotic Object (given `instance` and `index`).
* Append the pair (`f.name`, `exportedFunctions[index]`) to `exports`

Let `moduleRecord` be a new [WebAssembly Module Record](#webassembly-module-record) (given `exports`).

Let `exportStrings` be the projected list of only the first (string) components of `exports`.
Let `moduleNamespace` be the result of calling 
[`ModuleNamespaceCreate(moduleRecord, exportStrings)`](http://tc39.github.io/ecma262/#sec-modulenamespacecreate).
Set `moduleRecord.[[Namespace]]` to `moduleNamespace`.

Return a new `WASM.Instance` object initializing `[[Instance]]` = `instance` and `exports` = `moduleNamespace`.

### WebAssembly Module Record

[Abstract Module Record](http://tc39.github.io/ecma262/#sec-abstract-module-records)
is a spec-internal concept used to define ES6 modules. This abstract class currently
has one concrete subclass, [Source Text Module Record](http://tc39.github.io/ecma262/#sec-source-text-module-records)
which corresponds to a normal ES6 module. These interfaces are used to define the
[process of loading a module on the Web](https://html.spec.whatwg.org/multipage/webappapis.html#integration-with-the-javascript-module-system).

When WebAssembly gets [ES6 Module integration](Modules.md#integration-with-es6-modules), 
a new *WebAssembly Module Record* subclass would be added which would specify
the right thing to do for WebAssembly modules as part of the overall loading process.

Until then, the specification of [Module Namespace Exotic Objects](http://tc39.github.io/ecma262/#sec-module-namespace-exotic-objects),
(used for the `WASM.Instance` `exports` property) still needs to refer to *some*
vestigial Module Record as part of the specification of the
[\[\[Get\]\]](http://tc39.github.io/ecma262/#sec-module-namespace-exotic-objects-get-p-receiver)
method.

More work is needed to flesh out the precise spec interaction here, but the basic
idea is to create a [Module Environment Record](http://tc39.github.io/ecma262/#sec-module-environment-records)
from `exports` as the [[Environment]] of a new WebAssembly Module Record.

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
    WASM.compile(buffer)
).then(module => {
    var importObj = {
        m: {import1: () => console.log("hello, ")},
        import2: () => console.log("world!\n")
    };
    var instance = new WASM.Instance(module, importObj); // "hello, "
    instance.exports.f(); // "world!"
});
```

## TODO

* `WASM.Memory`: imports, exports
* `WASM.Table`: imports, exports
* `WASM.Module` `exports`/`imports` properties (reflection)
