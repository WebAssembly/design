# JavaScript API

In the [MVP](MVP.md), the only way to access WebAssembly on the Web is through
an explicit JS API which is defined below.
(In the [future :unicorn:][future general], WebAssembly may also
be loaded and run directly from an HTML `<script type='module'>` tag—and
any other Web API that loads ES6 modules via URL—as part of 
[ES6 Module integration](Modules.md#integration-with-es6-modules).)

## Traps

Whenever WebAssembly semantics specify a [trap](Semantics.md#traps),
a `WebAssembly.RuntimeError` object is thrown. WebAssembly code (currently)
has no way to catch this exception and thus the exception will necessarily
propagate to the enclosing non-WebAssembly caller (either the browser or
JavaScript) where it is handled like a normal JavaScript exception.

If WebAssembly calls JavaScript via import and the JavaScript throws an
exception, the exception is propagated through the WebAssembly activation to the
enclosing caller.

Because JavaScript exceptions can be handled, and JavaScript can continue to
call WebAssembly exports after a trap has been handled, traps do not, in
general, prevent future execution.

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
   which is thrown whenever WebAssembly specifies a [trap](#traps).

### Function Properties of the `WebAssembly` object

#### `WebAssembly.validate`

The `validate` function has the signature:

```
Boolean validate(BufferSource bytes)
```

If the given `bytes` argument is not a
[`BufferSource`](https://heycam.github.io/webidl/#common-BufferSource),
then a `TypeError` is thrown.

Otherwise, this function performs *validation* as defined by the [WebAssembly
specification](https://github.com/WebAssembly/spec/blob/master/interpreter/) and returns `true` if validation succeeded, `false` if validation failed.

#### `WebAssembly.compile`

The `compile` function has the signature:

```
Promise<WebAssembly.Module> compile(BufferSource bytes)
```

If the given `bytes` argument is not a
[`BufferSource`](https://heycam.github.io/webidl/#common-BufferSource),
the returned `Promise` is [rejected](http://tc39.github.io/ecma262/#sec-rejectpromise)
with a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror).

Otherwise, this function starts an asychronous task to compile a `WebAssembly.Module`
as described in the [`WebAssembly.Module` constructor](#webassemblymodule-constructor).
On success, the `Promise` is [fulfilled](http://tc39.github.io/ecma262/#sec-fulfillpromise)
with the resulting `WebAssembly.Module` object. On failure, the `Promise` is 
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise) with a 
`WebAssembly.CompileError`.

The asynchronous compilation is logically performed on a copy of the state of
the given `BufferSource` captured during the call to `compile`; subsequent mutations
of the `BufferSource` after `compile` return do not affect ongoing compilations.

In the [future :unicorn:][future streaming], this function can be
extended to accept a [stream](https://streams.spec.whatwg.org), thereby enabling
asynchronous, background, streaming compilation.

#### `WebAssembly.instantiate`

The `instantiate` function is overloaded based on types of its arguments.
If neither of the following overloads match, then the returned `Promise` is
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise)
with a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror).

```
Promise<{module:WebAssembly.Module, instance:WebAssembly.Instance}>
  instantiate(BufferSource bytes [, importObject])
```

This description applies if the first argument is a 
[`BufferSource`](https://heycam.github.io/webidl/#common-BufferSource).

This function starts an asynchronous task that first compiles a `WebAssembly.Module`
from `bytes` as described in the [`WebAssembly.Module` constructor](#webassemblymodule-constructor)
and then instantiate the resulting `Module` with `importObject` as described in the
[`WebAssembly.Instance` constructor](#webassemblyinstance-constructor).
On success, the `Promise` is [fulfilled](http://tc39.github.io/ecma262/#sec-fulfillpromise)
with a plain JavaScript object pair `{module, instance}` containing the resulting
`WebAssembly.Module` and `WebAssembly.Instance`. On failure, the `Promise` is
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise) with a 
`WebAssembly.CompileError`.

The asynchronous compilation is logically performed on a copy of the state of
the given `BufferSource` captured during the call to `instantiate`; subsequent mutations
of the `BufferSource` after `instantiate` return do not affect ongoing compilations.

```
Promise<WebAssembly.Instance> instantiate(moduleObject [, importObject])
```

This description applies if the first argument is a `WebAssembly.Module` instance.

This function starts an asynchronous task that instantiates a `WebAssembly.Instance`
from `moduleObject` and `importObject` as described in the
[`WebAssembly.Instance` constructor](#webassemblyinstance-constructor).
On success, the `Promise` is [fulfilled](http://tc39.github.io/ecma262/#sec-fulfillpromise)
with the resulting `WebAssembly.Instance` object. On failure, the `Promise` is
[rejected](http://tc39.github.io/ecma262/#sec-rejectpromise) with a 
`WebAssembly.CompileError`.

## `WebAssembly.Module` Objects

A `WebAssembly.Module` object represents the stateless result of compiling a
WebAssembly binary-format module and contains one internal slot:

 * [[Module]] : an [`Ast.module`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/ast.ml#L176)
   which is the spec definition of a module

### `WebAssembly.Module` Constructor

The `WebAssembly.Module` constructor has the signature:

```
new Module(BufferSource bytes)
```

If the NewTarget is `undefined`, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
exception is thrown (i.e., this constructor cannot be called as a function without `new`).

If the given `bytes` argument is not a
[`BufferSource`](https://heycam.github.io/webidl/#common-BufferSource),
a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
exception is thrown.

Otherwise, this function performs synchronous compilation of the `BufferSource`:

* The byte range delimited by the `BufferSource` is first logically decoded 
  according to [BinaryEncoding.md](BinaryEncoding.md) and then validated
  according to the rules in [spec/valid.ml](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/valid.ml#L415).
* The spec `string` values inside `Ast.module` are decoded as UTF8 as described in 
  [Web.md](Web.md#names).
* On success, a new `WebAssembly.Module` object is returned with [[Module]] set to
  the validated `Ast.module`.
* On failure, a new `WebAssembly.CompileError` is thrown.

### `WebAssembly.Module.exports`

The `exports` function has the signature:

```
Array exports(moduleObject)
```

If `moduleObject` is not a `WebAssembly.Module` instance, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
is thrown.

This function returns an `Array` produced by mapping each
[`Ast.export`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/ast.ml#L152)
`e` of [moduleObject.[[Module]].exports](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/ast.ml#L187)
to the Object `{ name: String(e.name), kind: e.ekind }` where `e.name` is [decoded as UTF8](Web.md#names)
and `e.ekind` is mapped to one of the String values `"function"`, `"table"`, `"memory"`, `"global"`.

Note: other fields like `signature` may be added in the future.

### `WebAssembly.Module.imports`

The `imports` function has the signature:

```
Array imports(moduleObject)
```

If `moduleObject` is not a `WebAssembly.Module` instance, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
is thrown.

This function returns an `Array` produced by mapping each
[`Ast.import`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/ast.ml#L167)
`i` of [moduleObject.[[Module]].imports](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/ast.ml#L203)
to the Object `{ module: String(i.module_name), name: String(i.item_name), kind: i.ikind }` where
`i.module_name` and `i.item_name` are  [decoded as UTF8](Web.md#names) and
`i.ikind` is mapped to one of the String values `"function"`, `"table"`, `"memory"`, `"global"`.

Note: other fields like `signature` may be added in the future.

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

* [[Instance]] : an [`Instance.instance`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/instance.ml#L17)
  which is the WebAssembly spec definition of an instance

as well as one plain data property (configurable, writable, enumerable)
added by the constructor:

* exports : a [Module Namespace Object](http://tc39.github.io/ecma262/#sec-module-namespace-objects)

### `WebAssembly.Instance` Constructor

The `WebAssembly.Instance` constructor has the signature:

```
new Instance(moduleObject [, importObject])
```

If the NewTarget is `undefined`, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
exception is thrown (i.e., this
constructor cannot be called as a function without `new`).

If `moduleObject` is not a `WebAssembly.Module` instance, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
is thrown.

Let `module` be the [`Ast.module`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/ast.ml#L176)
`moduleObject.[[Module]]`.

If the `importObject` parameter is not `undefined` and `Type(importObject)` is
not Object, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
is thrown. If the list of 
[`module.imports`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/ast.ml#L186)
is not empty and `Type(importObject)` is not Object, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
is thrown.

Let `funcs`, `memories` and `tables` be initially-empty lists of callable JavaScript objects, `WebAssembly.Memory` objects and `WebAssembly.Table` objects, respectively.

Let `imports` be an initially-empty list of [`external`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/instance.ml#L11) values.

For each [`import`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/ast.ml#L168)
`i` in `module.imports`:

* Let `o` be the resultant value of performing
  [`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`importObject`, [`i.module_name`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/ast.ml#L170)).
* If `Type(o)` is not Object, throw a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror).
* Let `v` be the value of performing [`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`o`, [`i.item_name`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/ast.ml#L171))
* If `i` is a function import:
  * If [`IsCallable(v)`](https://tc39.github.io/ecma262/#sec-iscallable) is `false`,
    throw a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror).
  * If `v` is an [Exported Function Exotic Object](#exported-function-exotic-objects):
    * If the signature of `v` does not match the signature of `i`, throw a 
      [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror).
    * Let `closure` be `v.[[Closure]]`.
  * Otherwise:
    * Let `closure` be a new [host function](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/instance.ml#L9)
      of the given signature
      which calls `v` by coercing WebAssembly arguments to JavaScript arguments
      via [`ToJSValue`](#tojsvalue) and returns the result, if any, by coercing
      via [`ToWebAssemblyValue`](#towebassemblyvalue).
  * Append `v` to `funcs`.
  * Append `closure` to `imports`.
* If `i` is a global import:
  * If `i` is not an immutable global, throw a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror).
  * If `Type(v)` is not Number, throw a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror).
  * Append [`ToWebAssemblyValue`](#towebassemblyvalue)`(v)` to `imports`.
* If `i` is a memory import:
  * If `v` is not a [`WebAssembly.Memory` object](#webassemblymemory-objects),
    throw a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror).
  * Append `v` to `memories`.
  * Append `v.[[Memory]]` to `imports`.
* Otherwise (`i` is a table import):
  * If `v` is not a [`WebAssembly.Table` object](#webassemblytable-objects),
    throw a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror).
  * Append `v` to `tables`.
  * Append `v.[[Table]]` to `imports`.

Let `instance` be the result of creating a new
[`instance`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/instance.ml#L17)
by calling
[`Eval.init`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/eval.ml#L416)
given `module` and `imports`.

Let `exports` be a list of (string, JS value) pairs that is mapped from 
each [external](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/instance.ml#L24) value `e` in `instance.exports` as follows:

* If `e` is a [closure](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/instance.ml#L12) `c`:
  * If there is an [Exported Function Exotic Object](#exported-function-exotic-objects) `func` in `funcs` whose `func.[[Closure]]` equals `c`, then return `func`.
  * (Note: At most one wrapper is created for any closure, so `func` is unique, even if there are multiple occurrances in the list. Moreover, if the item was an import that is already an [Exported Function Exotic Object](#exported-function-exotic-objects), then the original function object will be found. For imports that are regular JS functions, a new wrapper will be created.)
  * Otherwise:
    * Let `func` be an [Exported Function Exotic Object](#exported-function-exotic-objects) created from `c`.
    * Append `func` to `funcs`.
    * Return `func`.
* If `e` is a [global](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/instance.ml#L15) `v`:
  * [Assert](https://tc39.github.io/ecma262/#assert): the global is immutable
    by MVP validation constraint.
  * Return [`ToJSValue`](#tojsvalue)`(v)`.
* If `e` is a [memory](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/instance.ml#L14) `m`:
  * If there is an element `memory` in `memories` whose `memory.[[Memory]]` is `m`, then return `memory`.
  * (Note: At most one `WebAssembly.Memory` object is created for any memory, so the above `memory` is unique, even if there are multiple occurrances in the list. Moreover, if the item was an import, the original object will be found.)
  * Otherwise:
    * Let `memory` be a new `WebAssembly.Memory` object created via [`CreateMemoryObject`](#creatememoryobject) from `m`.
    * Append `memory` to `memories`.
    * Return `memory`.
* Otherwise `e` must be a [table](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/instance.ml#L13) `t`:
  * If there is an element `table` in `tables` whose `table.[[Table]]` is `t`, then return `table`.
  * (Note: At most one `WebAssembly.Table` object is created for any table, so the above `table` is unique, even if there are multiple occurrances in the list. Moreover, if the item was an import, the original object will be found.)
  * Otherwise:
    * Let `values` be a list of JS values that is mapped from `t`'s elements as follows:
      * For an element that is [`uninitialized`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/table.mli#L8):
        * Return `null`.
      * For an element that is a [`closure`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/instance.ml#L7) `c`:
        * If there is an [Exported Function Exotic Object](#exported-function-exotic-objects) `func` in `funcs` whose `func.[[Closure]]` equals `c`, then return `func`.
        * (Note: At most one wrapper is created for a any closure, so `func` is uniquely determined. Moreover, if the item was an import that is already an [Exported Function Exotic Object](#exported-function-exotic-objects), then the original function object will be found. For imports that are regular JS functions, a new wrapper will be created.)
        * Otherwise:
          * Let `func` be an [Exported Function Exotic Object](#exported-function-exotic-objects) created from `c`.
          * Append `func` to `funcs`.
          * Return `func`.
    * Let `table` be a new `WebAssembly.Table` object with [[Table]] set to `t` and [[Values]] set to `values`.
    * Append `table` to `tables`.
    * Return `table`.

Note: For the purpose of the above algorithm, two [closure](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/instance.ml#L7) values are considered equal if and only if:

* Either they are both WebAssembly functions for the same instance and referring to the same function definition.
* Or they are identical host functions (i.e., each host function value created from a JavaScript function is considered fresh).

Let `moduleRecord` be a new [WebAssembly Module Record](#webassembly-module-record)
(given `exports`).

Let `exportStrings` be the projected list of only the first (string) components
of `exports`. Let `moduleNamespace` be the result of calling 
[`ModuleNamespaceCreate(moduleRecord, exportStrings)`](http://tc39.github.io/ecma262/#sec-modulenamespacecreate).
Set `moduleRecord.[[Namespace]]` to `moduleNamespace`.

Let `instanceObject` be a new `WebAssembly.Instance` object setting
`[[Instance]]` to `instance` and `exports` to `moduleNamespace`.

If, after evaluating the `offset` [initializer expression](Modules.md#initializer-expression)
of every [Data](Modules.md#data-section) and [Element](Modules.md#elements-section)
Segment, any of the segments do not fit in their respective Memory or Table, throw a 
[`RangeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-rangeerror).

Apply all Data and Element segments to their respective Memory or Table in the
order in which they appear in the module. Segments may overlap and, if they do,
the final value is the last value written in order. Note: there should be no
errors possible that would cause this operation to fail partway through. After
this operation completes, elements of `instance` are visible and callable
through [imported Tables](Modules.md#imports), even if `start` fails.

If a [`start`](Modules.md#module-start-function) is present, it is evaluated
given `instance`. Any errors thrown by `start` are propagated to the caller.

Return `instanceObject`.

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

 * [[Closure]] : the [closure](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/instance.ml#L7)
   representing the function

as well as the internal slots required of all builtin functions:

 * [[Prototype]] : [%FunctionPrototype%](http://tc39.github.io/ecma262/#sec-well-known-intrinsic-objects)
 * [[Extensible]] : `true`
 * [[Realm]] : the [current Realm Record](http://tc39.github.io/ecma262/#current-realm)
 * [[ScriptOrModule]] : [`GetActiveScriptOrModule`](http://tc39.github.io/ecma262/#sec-getactivescriptormodule)

Exported Functions also have the following data properties:

* the `length` property is set to the exported function's signature's arity 
* the `name` is set to `index` as a Number value

WebAssembly Exported Functions have a `[[Call]](this, argValues)` method defined as:

 * Let `args` be an empty list of coerced values.
 * Let `inArity` be the number of arguments and `outArity` be the number of results in the [`function type`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/eval.ml#L106) of the function's [[Closure]].
 * For all values `v` in `argValues`, in the order of their appearance:
   * If the length of`args` is less than `inArity`, append [`ToWebAssemblyValue`](#towebassemblyvalue)`(v)` to `args`.
 * While the length of `args` is less than `inArity`, append [`ToWebAssemblyValue`](#towebassemblyvalue)`(undefined)` to `args`.
 * Let `ret` be the result of calling [`Eval.invoke`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/eval.ml#L443)
   passing [[Closure]], and `args`.
 * If `outArity` is 0, return `undefined`.
 * Otherwise, return [`ToJSValue`](#tojsvalue)`(v)`, where `v` is the singular element of `ret`.

`[[Call]](this, argValues)` executes in the [[Realm]] of the callee Exported Function. This corresponds to [the requirements of builtin function objects in JavaScript](https://tc39.github.io/ecma262/#sec-built-in-function-objects).

Exported Functions do not have a [[Construct]] method and thus it is not possible to 
call one with the `new` operator.

## `WebAssembly.Memory` Objects

A `WebAssembly.Memory` object contains a single [linear memory](Semantics.md#linear-memory)
which can be simultaneously referenced by multiple `Instance` objects. Each
`Memory` object has two internal slots:

 * [[Memory]] : a [`Memory.memory`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/memory.mli)
 * [[BufferObject]] : the current `ArrayBuffer` whose [[ArrayBufferByteLength]]
   matches the current byte length of [[Memory]]

### `WebAssembly.Memory` Constructor

The `WebAssembly.Memory` constructor has the signature:

```
new Memory(memoryDescriptor)
```

If the NewTarget is `undefined`, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
exception is thrown (i.e., this constructor cannot be called as a function without `new`).

If `Type(memoryDescriptor)` is not Object, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
is thrown.

Let `initial` be [`ToNonWrappingUint32`](#tononwrappinguint32)([`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`memoryDescriptor`, `"initial"`)).

If [`HasProperty`](http://tc39.github.io/ecma262/#sec-hasproperty)(`"maximum"`),
then let `maximum` be [`ToNonWrappingUint32`](#tononwrappinguint32)([`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`memoryDescriptor`, `"maximum"`)).
Otherwise, let `maximum` be `None`.

Let `memory` be the result of calling 
[`Memory.create`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/memory.ml#L68)
given arguments `initial` and `maximum`. Note that `initial` and `maximum` are
specified in units of WebAssembly pages (64KiB).

Return the result of [`CreateMemoryObject`](#creatememoryobject)(`memory`).

### CreateMemoryObject

Given a [`Memory.memory`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/memory.mli#L1)
`m`, to create a `WebAssembly.Memory`:

Let `buffer` be a new `ArrayBuffer` whose
[[[ArrayBufferData]]](http://tc39.github.io/ecma262/#sec-properties-of-the-arraybuffer-prototype-object)
aliases `m` and whose 
[[[ArrayBufferByteLength]]](http://tc39.github.io/ecma262/#sec-properties-of-the-arraybuffer-prototype-object)
is set to the byte length of `m`.

Any attempts to [`detach`](http://tc39.github.io/ecma262/#sec-detacharraybuffer) `buffer` *other* than
the detachment performed by [`m.grow`](#webassemblymemoryprototypegrow) shall throw a 
[`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)

Return a new `WebAssembly.Memory` instance with [[Memory]] set to `m` and
[[BufferObject]] set to `buffer`.

### `WebAssembly.Memory.prototype.grow`

The `grow` method has the signature:

```
grow(delta)
```

Let `M` be the `this` value. If `M` is not a `WebAssembly.Memory`,
a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
is thrown.

Let `d` be [`ToNonWrappingUint32`](#tononwrappinguint32)(`delta`).

Let `ret` be the result of performing a
[`grow_memory`](Semantics.md#resizing) operation given delta `d`.

If `ret` is `-1`, a [`RangeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-rangeerror) is thrown.

Perform [`DetachArrayBuffer`](http://tc39.github.io/ecma262/#sec-detacharraybuffer)(`M.[[BufferObject]]`).

Assign to `M.[[BufferObject]]` a new `ArrayBuffer` whose
[[[ArrayBufferData]]](http://tc39.github.io/ecma262/#sec-properties-of-the-arraybuffer-prototype-object)
aliases `M.[[Memory]]` and whose 
[[[ArrayBufferByteLength]]](http://tc39.github.io/ecma262/#sec-properties-of-the-arraybuffer-prototype-object)
is set to the new byte length of `M.[[Memory]]`.

Return [`ToJSValue`](#ToJSValue)(`ret`).

### `WebAssembly.Memory.prototype.buffer`

This is an accessor property whose [[Set]] is Undefined and whose [[Get]]
accessor function performs the following steps:

If `this` is not a `WebAssembly.Memory`, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
is thrown. Otherwise return `M.[[BufferObject]]`.

## `WebAssembly.Table` Objects

A `WebAssembly.Table` object contains a single [table](Semantics.md#table)
which can be simultaneously referenced by multiple `Instance` objects. Each
`Table` object has two internal slots:

 * [[Table]] : a [`Table.table`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/table.mli#L1)
 * [[Values]] : an array whose elements are either `null` or [Exported Function Exotic Object](#exported-function-exotic-objects)

### `WebAssembly.Table` Constructor

The `WebAssembly.Table` constructor has the signature:

```
new Table(tableDescriptor)
```

If the NewTarget is `undefined`, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
exception is thrown (i.e., this constructor cannot be called as a function without `new`).

If `Type(tableDescriptor)` is not Object, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
is thrown.

Let `element` be the result of calling [`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`tableDescriptor`, `"element"`).
If `element` is not the string `"anyfunc"`, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
is thrown.
(Note: this check is intended to be relaxed in the
[future :unicorn:][future types] to allow different element types.)

Let `initial` be [`ToNonWrappingUint32`](#tononwrappinguint32)([`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`tableDescriptor`, `"initial"`)).

If [`HasProperty`](http://tc39.github.io/ecma262/#sec-hasproperty)(`"maximum"`),
then let `maximum` be [`ToNonWrappingUint32`](#tononwrappinguint32)([`Get`](http://tc39.github.io/ecma262/#sec-get-o-p)(`tableDescriptor`, `"maximum"`)).
Otherwise, let `maximum` be None.

Let `table` be the result of calling 
[`Table.create`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/table.ml#L68)
given arguments `AnyFuncType`, `initial` and `maximum`.

Let `values` be a new empty array of `initial` elements, all with value
`null`.

Return a new `WebAssemby.Table` instance with [[Table]] set to `table` and
[[Values]] set to `values`.

### `WebAssembly.Table.prototype.length`

This is an accessor property whose [[Set]] is Undefined and whose [[Get]]
accessor function performs the following steps:

Let `T` be the `this` value. If `T` is not a `WebAssembly.Table`, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
is thrown.

Return `T.[[Values]].length`.

### `WebAssembly.Table.prototype.grow`

This method calls [`Table.grow`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/table.ml#L79), having performed
[`ToNonWrappingUint32`](#tononwrappinguint32) on the first argument.
On failure, a [`RangeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-rangeerror) is thrown.

### `WebAssembly.Table.prototype.get`

This method has the following signature

```
get(index)
```

Let `T` be the `this` value. If `T` is not a `WebAssembly.Table`, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
is thrown.

Let `i` be the result of [`ToNonWrappingUint32`](#tononwrappinguint32)(`index`).

If `i` is greater or equal than the length of `T.[[Values]]`, a [`RangeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-rangeerror) is thrown.

Return `T.[[Values]][i]`.

### `WebAssembly.Table.prototype.set`

This method has the following signature

```
set(index, value)
```

Let `T` be the `this` value. If `T` is not a `WebAssembly.Table`, a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
is thrown.

If `value` is not an [Exported Function Exotic Object](#exported-function-exotic-objects)
or `null`, throw a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror).

Let `i` be the result of [`ToNonWrappingUint32`](#tononwrappinguint32)(`index`).

If `i` is greater or equal than the length of `T.[[Values]]`, a [`RangeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-rangeerror) is thrown.

If `value` is `null`, let `elem` be `Uninitialized`;
otherwise, let `elem` be `value.[[Closure]]`.

Set `T.[[Table]][i]` to `elem`.

Set `T.[[Values]][i]` to `value`.

Return `undefined`.

## ToJSValue

To coerce a WebAssembly [`value`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/values.ml#L9)
to a JavaScript value:

* given a WebAssembly `i32` is interpreted as a signed integer, converted (losslessly) to an
  IEEE754 double and then returned as a JavaScript Number
* given a WebAssembly `i64`, throw a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
* given a WebAssembly `f32` (single-precision IEEE754), convert (losslessly) to
  a IEEE754 double, [possibly canonicalize NaN](http://tc39.github.io/ecma262/#sec-setvalueinbuffer),
  and return as a JavaScript Number
* given a WebAssembly `f64`, [possibly canonicalize NaN](http://tc39.github.io/ecma262/#sec-setvalueinbuffer)
  and return as a JavaScript Number

If the WebAssembly value is optional, then given `None`, return JavaScript value
`undefined`.

## ToWebAssemblyValue

To coerce a JavaScript value to a given WebAssembly [`value type`](https://github.com/WebAssembly/spec/blob/master/interpreter/spec/types.ml#L3),

* coerce to `i32` via [`ToInt32(v)`](http://tc39.github.io/ecma262/#sec-toint32)
* for `i64`, throw a [`TypeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-typeerror)
* coerce to `f32` by first applying [`ToNumber(v)`](http://tc39.github.io/ecma262/#sec-tonumber)
  and then converting the resulting IEEE754 64-bit double to a 32-bit float using `roundTiesToEven`
* coerce to `f64` via [`ToNumber(v)`](http://tc39.github.io/ecma262/#sec-tonumber)

If the value type is optional, then given `None`, the JavaScript value is
ignored.

## ToNonWrappingUint32

To convert a JavaScript value `v` to an unsigned integer in the range [0, `UINT32_MAX`]:

Let `i` be [`ToInteger`](http://tc39.github.io/ecma262/#sec-tointeger)(`v`).

If `i` is negative or greater than `UINT32_MAX`, 
[`RangeError`](https://tc39.github.io/ecma262/#sec-native-error-types-used-in-this-standard-rangeerror)
is thrown.

Return `i`.

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

* JS API for cyclic imports

[future general]: FutureFeatures.md
[future streaming]: FutureFeatures.md#streaming-compilation
[future types]: FutureFeatures.md#more-table-operators-and-types
