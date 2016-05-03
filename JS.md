# JavaScript API

In the [MVP](MVP.md), the only way to access WebAssembly on the Web is through
an explicit JS API which is defined below. (In the future, WebAssembly may also
be loaded and run directly from an HTML `<script type='module'>` tag—and
any other Web API that loads ES6 modules via URL—as part of 
[ES6 Module integration](Modules.md#integration-with-es6-modules).)

*Note: current experimental WebAssembly implementations expose a single
all-in-one function `Wasm.instantiateModule(bytes, imports)` which is used
by the current [demo](http://webassembly.github.io/demo). This function is
basically equivalent to 
`new WebAssembly.Instance(new WebAssembly.Module(bytes), imports)`
as defined below and will be removed at some point in the future.*


## The `WebAssembly` object

The `WebAssembly` object is the initial value of the `WebAssembly` property of
the global object. Like the `Math` and `JSON` objects, the `WebAssembly` object
is a plain JS object (not a constructor or function) that acts like a namespace
and has the following properties:


### Constructor Properties of the `WebAssembly` object

The following intrinsic objects are added:

* `WebAssembly.Module` : the [`WebAssembly.Module` constructor](#webassemblymodule-constructor)
* `WebAssembly.Instance` : the [`WebAssembly.Instance` constructor](#webassemblyinstance-constructor)
* `WebAssembly.Memory` : the [`WebAssembly.Memory` constructor](#webassemblymemory-constructor)
* `WebAssembly.Table` : the [`WebAssembly.Table` constructor](#webassemblytable-constructor)
* `WebAssembly.CompileError` : a [NativeError](http://tc39.github.io/ecma262/#sec-nativeerror-object-structure)
   which indicates an error during WebAssembly decoding or validation
* `WebAssembly.RuntimeError` : a [NativeError](http://tc39.github.io/ecma262/#sec-nativeerror-object-structure)
   which indicates an error while running WebAssembly code


### Function Properties of the `WebAssembly` object


#### `WebAssembly.compile`

The `compile` function has the signature:
```
Promise<WebAssembly.Module> compile(BufferSource bytes)
```
If the given `bytes` argument is not a
[`BufferSource`](https://heycam.github.io/webidl/#common-BufferSource),
the returned `Promise` is [rejected](http://tc39.github.io/ecma262/#sec-rejectpromise)
with a `TypeError`.

Otherwise, this function starts an asychronous task to compile a `WebAssembly.Module`
as described in the [`WebAssembly.Module` constructor](#webassemblymodule-constructor).
On success, the `Promise` is [fulfilled](http://tc39.github.io/ecma262/#sec-fulfillpromise)
with the resulting `WebAssembly.Module` instance. On failure, the `Promise` is 
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise) with a 
`WebAssembly.CompileError`.

The asynchronous compilation is logically performed on a copy of the state of
the given `BufferSource` captured during the call to `compile`; subsequent mutations
of the `BufferSource` after `compile` return do not affect ongoing compilations.

In the [future](FutureFeatures.md#streaming-compilation), this function can be
extended to accept a [stream](https://streams.spec.whatwg.org), thereby enabling
asynchronous, background, streaming compilation.


## `WebAssembly.Module` Objects

A `WebAssembly.Module` object represents the stateless result of compiling a
WebAssembly binary-format module and contains one internal slot:
 * [[Module]] : an [`Ast.module`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/ast.ml#L208)
   which is the spec definition of a validated module AST


### `WebAssembly.Module` Constructor

The `WebAssembly.Module` constructor has the signature:
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
  [Web.md](Web.md#names).
* On success, a new `WebAssembly.Module` instance is returned with [[Module]] set to
  the validated `Ast.module`.
* On failure, a new `WebAssembly.CompileError` is thrown.


### Structured Clone of a `WebAssembly.Module`

A `WebAssembly.Module` is a
[cloneable object](https://html.spec.whatwg.org/multipage/infrastructure.html#cloneable-objects)
which means it can be cloned between windows/workers and also
stored/retrieved into/from an [IDBObjectStore](https://w3c.github.io/IndexedDB/#object-store).
The semantics of a structured clone is as-if the binary source, from which the
`WebAssembly.Module` was compiled, were cloned and recompiled into the target realm.
Engines should attempt to share/reuse internal compiled code when performing
a structured clone although, in corner cases like CPU upgrade or browser
update, this may not be possible and full recompilation may be necessary.

Given the above engine optimizations, structured cloning provides developers
explicit control over both compiled-code caching and cross-window/worker code
sharing.


## `WebAssembly.Instance` Objects

A `WebAssembly.Instance` object represents the instantiation of a 
`WebAssembly.Module` into a
[realm](http://tc39.github.io/ecma262/#sec-code-realms) and has one
internal slot:
* [[Instance]] : an [`Eval.instance`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/eval.ml#L15)
  which is the WebAssembly spec definition of an instance

as well as one plain data property (configurable, writable, enumerable)
added by the constructor:
* exports : a [Module Namespace Object](http://tc39.github.io/ecma262/#sec-module-namespace-objects)


### `WebAssembly.Instance` Constructor

The `WebAssembly.Instance` constructor has the signature:
```
new Instance(moduleObject [, importObject])
```
If the NewTarget is `undefined`, a `TypeError` exception is thrown (i.e., this
constructor cannot be called as a function without `new`).

If `moduleObject` is not a `WebAssembly.Module` instance, a `TypeError` is thrown.

Let `module` be the [`Ast.module`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/ast.ml#L211)
`moduleObject.[[Module]]`.

If the `importObject` parameter is not `undefined` and `Type(importObject)` is
not Object, a `TypeError` is thrown. If the list of 
[`module.imports`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/ast.ml#L215)
is not empty and `Type(importObject)` is not Object, a `TypeError` is thrown.

Let `importedFunctions`, `importedMemories` and `importedTables` be initially-empty
lists of JS functions, `WebAssembly.Memory` objects, and `WebAssembly.Table`
objects, respectively.

For each [`import`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/kernel.ml#L135)
`i` in `module.imports`:
* Let `v` be the resultant value of performing
  [`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`importObject`, [`i.module_name`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/kernel.ml#L139)).
* If [`i.export_name`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/kernel.ml#L140)
  is not the empty string:
  * If `Type(v)` is not Object, throw a `TypeError`.
  * Let `v` instead be the value of performing [`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`v`, `i.export_name`)
* If `i` is a function export:
  * If `IsCallable(v)` is `false`, throw a `TypeError`.
  * Otherwise, append `v` to `importedFunctions`.
* If `i` is a memory export:
  * If `v` is not a [`WebAssembly.Memory` object](#webassemblymemory-objects),
    throw a `TypeError`.
  * Otherwise, append `v` to `importedMemories`.
* If `i` is a table export:
  * If `v` is not a [`WebAssembly.Table` object](#webassemblytable-objects),
    throw a `TypeError`.
  * Otherwise, append `v` to `importedTables`.

(Note: the ML spec currently doesn't have multiple kinds of exports; we assume here
it will be extended in the future.)

Let `instance` be the result of evaluating 
[`Eval.init`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/eval.ml#L319)
with arguments `module`, `importedFunctions`, `importedMemories` and
`importedTables`. Note: this synchronously executes the
[`start`](Modules.md#module-start-function) function, if present.

(Note: in the ML spec, `Eval.init` currently doesn't accept imported memories or
tables; we assume here it will be extended in the future.)

Let `exports` be an initially-empty list of (string, JS value) pairs.

Let `exportedFunctions` be a map whose initial contents are mappings from
the array indices of `importedFunctions` to `importedFunctions[i]`.

Let `exportedMemories` be a map whose initial contents are mappings from
the array indices of `exportedMemories` to `exportedMemories[i]`.

Let `exportedTables` be a map whose initial contents are mappings from
the array indices of `exportedTables` to `exportedTables[i]`.

For each [export](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/kernel.ml#L128)
`e` in `module.exports`:
* If `e` is a function export:
  * Let `i` be the function index of `e`.
  * If `i` is not already present in `exportedFunctions`:
    * Let `v` be the result of creating a new [Exported Function](#exported-function-exotic-objects)
      Exotic Object (given `instance` and `i`).
    * Add a mapping from `i` to `v` in `exportedFunctions`.
  * Otherwise, let `v` be `exportedFunctions[i]`.
* If `e` is a memory export:
  * Let `i` be the memory index of `e`.
  * If `i` is not already present in `exportedMemories`:
    * Let `memory` be the `i`th linear memory in the 
      [memory index space](Modules.md#index-spaces) of `instance`.
    * Let `v` be the result of [`CreateMemoryObject`](#creatememoryobject)`(memory)`.
    * Add a mapping from `i` to `v` in `exportedMemories`.
  * Otherwise, let `v` be `exportedMemories[i]`.
* If `e` is a table export:
  * Let `i` be the table index of `e`.
  * If `i` is not already present in `exportedTables`:
    * Let `table` be the `i`th table in the 
      [table index space](Modules.md#index-spaces) of `instance`.
    * Let `values` be an initially-empty list of Function Objects.
    * For each function index `fi` in `table`:
      * If `fi` is `None`, then append `null` to `values`.
      * Otherwise, if `fi` is not already present in `exportedFunctions`:
        * Let `f` be the result of creating a new
          [Exported Function](#exported-function-exotic-objects)
          Exotic Object (given `instance` and `fi`).
        * Add a mapping from `fi` to `f` in `exportedFunctions`.
        * Append `f` to `values`.
      * Otherwise, append `exportedFunctions[i]` to `values`.
    * Let `v` be a new `WebAssembly.Table` instance with [[Table]]
      set to `table` and [[Values]] set to `values`.
    * Add a mapping from `i` to `v` in `exportedTables`.
  * Otherwise, let `v` be `exportedTables[i]`.
* Append the pair (`e.name`, `v`) to `exports`

(Note: the ML spec currently doesn't have table exports or give all exports an
index; we assume here it will be extended in the future.)

Let `moduleRecord` be a new [WebAssembly Module Record](#webassembly-module-record)
(given `exports`).

Let `exportStrings` be the projected list of only the first (string) components
of `exports`. Let `moduleNamespace` be the result of calling 
[`ModuleNamespaceCreate(moduleRecord, exportStrings)`](http://tc39.github.io/ecma262/#sec-modulenamespacecreate).
Set `moduleRecord.[[Namespace]]` to `moduleNamespace`.

Return a new `WebAssembly.Instance` object setting `[[Instance]]` to `instance`
and `exports` to `moduleNamespace`.


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
(used for the `WebAssembly.Instance` `exports` property) still needs to refer to *some*
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


## `WebAssembly.Memory` Objects

A `WebAssembly.Memory` object contains a single [linear memory](AstSemantics.md#linear-memory)
which can be simultaneously referenced by multiple `Instance` objects. Each
`Memory` object has two internal slots:
 * [[Memory]] : a [`Memory.memory`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/memory.mli)
 * [[BufferObject]] : the current `ArrayBuffer` whose [[ArrayBufferByteLength]]
   matches the current byte length of [[Memory]]


### `WebAssembly.Memory` Constructor

The `WebAssembly.Memory` constructor has the signature:
```
new Memory(memoryDescriptor)
```
If the NewTarget is `undefined`, a `TypeError` exception is thrown (i.e., this
constructor cannot be called as a function without `new`).

If `Type(memoryDescriptor)` is not Object, a `TypeError` is thrown.

Let `initial` be [`ToInteger`](http://tc39.github.io/ecma262/#sec-tointeger)([`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`memoryDescriptor`, `"initial"`)).

If [`HasProperty`](http://tc39.github.io/ecma262/#sec-hasproperty)(`"maximum"`),
then let `maximum` be [`ToInteger`](http://tc39.github.io/ecma262/#sec-tointeger)([`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`memoryDescriptor`, `"maximum"`)).
Otherwise, let `maximum` be `None`.

Let `memory` be the result of calling 
[`Memory.create`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/memory.mli#L18)
given arguments `initial` and `maximum`. Note that `initial` and `maximum` are
specified in units of WebAssembly pages (64KiB).

(Note: the ML spec currently doesn't implement the maximum memory limit; we
assume here it will be extended in the future.)

Return the result of [`CreateMemoryObject`](#creatememoryobject)(`memory`).


### CreateMemoryObject

Given a [`Memory.memory`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/memory.mli)
`m`, to create a `WebAssembly.Memory`:

Let `buffer` be a new `ArrayBuffer` whose
[[[ArrayBufferData]]](http://tc39.github.io/ecma262/#sec-properties-of-the-arraybuffer-prototype-object)
aliases `m` and whose 
[[[ArrayBufferByteLength]]](http://tc39.github.io/ecma262/#sec-properties-of-the-arraybuffer-prototype-object)
is set to the byte length of `m`.

Return a new `WebAssembly.Memory` instance with [[Memory]] set to `m` and
[[BufferObject]] set to `buffer`.


### `WebAssembly.Memory.prototype.grow`

Let `M` be the `this` value. If `M` is not a `WebAssembly.Memory`,
a `TypeError` is thrown.

This method performs a [`grow_memory`](AstSemantics.md#resizing) on
`M.[[Memory]]`, having first performed [`ToInteger`](http://tc39.github.io/ecma262/#sec-tointeger)
on the first argument. On failure, a `WebAssembly.RuntimeError` is thrown.

If `M.[[Memory]].maximum` is `None`, perform
[`DetachArrayBuffer`](http://tc39.github.io/ecma262/#sec-detacharraybuffer)(`M.[[BufferObject]]`).

In either case, assign to `M.[[BufferObject]]` a new `ArrayBuffer` whose
[[[ArrayBufferData]]](http://tc39.github.io/ecma262/#sec-properties-of-the-arraybuffer-prototype-object)
aliases `M.[[Memory]]` and whose 
[[[ArrayBufferByteLength]]](http://tc39.github.io/ecma262/#sec-properties-of-the-arraybuffer-prototype-object)
is set to the new byte length of `M.[[Memory]]`.


### `WebAssembly.Memory.prototype.buffer`

This is an accessor property whose [[Set]] is Undefined and whose [[Get]]
accessor function performs the following steps:

If `this` is not a `WebAssembly.Memory`, a `TypeError` is thrown. Otherwise
return `M.[[BufferObject]]`.


## `WebAssembly.Table` Objects

A `WebAssembly.Table` object contains a single [table](AstSemantics.md#tables)
which can be simultaneously referenced by multiple `Instance` objects. Each
`Table` object has one internal slot:
 * [[Table]] : a [`Table.table`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/table.ml)
 * [[Values]] : an array whose elements are either `null` or Function Objects


### `WebAssembly.Table` Constructor

The `WebAssembly.Table` constructor has the signature:
```
new Table(tableDescriptor)
```
If the NewTarget is `undefined`, a `TypeError` exception is thrown (i.e., this
constructor cannot be called as a function without `new`).

If `Type(tableDescriptor)` is not Object, a `TypeError` is thrown.

Let `element` be the result of [ToTypeDescriptor](#totypedescriptor)([`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`tableDescriptor`, `"element"`)).

Let `initial` be [`ToInteger`](http://tc39.github.io/ecma262/#sec-tointeger)([`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`tableDescriptor`, `"initial"`)).

If [`HasProperty`](http://tc39.github.io/ecma262/#sec-hasproperty)(`"maximum"`),
then let `maximum` be [`ToInteger`](http://tc39.github.io/ecma262/#sec-tointeger)([`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`tableDescriptor`, `"maximum"`)).
Otherwise, let `maximum` be None.

Let `table` be the result of calling `Table.create` given arguments `element`,
`initial` and `maximum`.

(Note: the ML spec currently represents tables as a single `int list` of
function indices; we assume here it will be extended in the future with
a more general `Table` similar to 
[`Memory`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/memory.mli)
that also includes an `element` field of type 
[`Type.func_type option`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/types.ml#L5).)

Let `values` be a new empty array of `initial` elements, all with value
`null`.

Return a new `WebAssemby.Table` instance with [[Table]] set to `table` and
[[Values]] set to `values`.


### ToTypeDescriptor

This is an abstract function which, given a value `td`, returns either
[`Type.func_type`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/types.ml#L5) 
or `None`. (Note: when tables are generalized to hold non-functions in the 
[future](FutureFeatures.md#more-table-operators-and-types), the return type of
`ToTypeDescriptor` would return a more general type.)

TODO: pick the best way to describe a signature. Maybe some JSON-like object?
Maybe something more future-compatible with [Typed Objects](https://github.com/tschneidereit/typed-objects-explainer/)?

If `td` is the string `"function"`, return `None`.

Otherwise throw a `TypeError`.


### `WebAssembly.Table.prototype.grow`

This method calls `Table.grow`, having performed
[`ToInteger`](http://tc39.github.io/ecma262/#sec-tointeger) on the first argument.
On failure, a `WebAssembly.RuntimeError` is thrown.

(Note: the ML spec currently doesn't support resizing tables; we assume here it
will be extended in the future to have a `grow` operation similar to 
[`Memory.grow`](https://github.com/WebAssembly/spec/blob/master/ml-proto/spec/memory.mli#L21).)


### `WebAssembly.Table.prototype.get`

This method has the following signature
```
get(index)
```
Let `T` be the `this` value. If `T` is not a `WebAssembly.Table`, a `TypeError`
is thrown.

Let `i` be the result of [`ToInteger`](http://tc39.github.io/ecma262/#sec-tointeger)(`index`).

Return `T.[[Values]][i]`.


### `WebAssembly.Table.prototype.set`

This method has the following signature
```
set(index, value)
```

Let `T` be the `this` value. If `T` is not a `WebAssembly.Table`, a `TypeError`
is thrown.

If [`IsCallable`](http://tc39.github.io/ecma262/#sec-iscallable)(`value`) is 
false and `Type(value)` is not Null, throw a type error.

If `value` is a [Exported Function Exotic Object](#exported-function-exotic-objects)
and if `table.element` is not `None` and `T.element` does not
precisely match the signature of `v.[[FunctionIndex]]` in `v.[[Instance]].[[Module]]`
throw a `TypeError`.

Let `i` be the result of [`ToInteger`](http://tc39.github.io/ecma262/#sec-tointeger)(`index`).

Set `T.[[Values]][i]` to `value`.

Return Undefined.


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
    WebAssembly.compile(buffer)
).then(module => {
    var importObj = {
        m: {import1: () => console.log("hello, ")},
        import2: () => console.log("world!\n")
    };
    var instance = new WebAssembly.Instance(module, importObj); // "hello, "
    instance.exports.f(); // "world!"
});
```

## TODO
* `WebAssembly.Module` `exports`/`imports` properties (reflection)
* JS API for cyclic imports
