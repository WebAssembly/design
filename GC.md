# GC / DOM / Web API Integration [:unicorn:][future general]

## NOTE: This is a [future :unicorn:][future general] feature! ##

After the [MVP](MVP.md), to realize the [high-level goals](HighLevelGoals.md)
of (1) integrating well with the existing Web platform and (2) supporting
languages other than C++, WebAssembly needs to be able to:

* reference DOM and other Web API objects directly from WebAssembly code;
* call Web APIs (passing primitives or DOM/GC/Web API objects) directly from
  WebAssembly without calling through JavaScript; and
* efficiently allocate and manipulate GC objects directly from WebAssembly
  code.

The following document is a high-level sketch of one approach for implementing
the above goals. *Consider the contents incomplete and expect change over
time.*

An important constraint is that, while WebAssembly should allow tight
integration with the [Web](Web.md), it should not bake in details
or Web standards dependencies that prevent execution in a 
[non-Web embedding](NonWeb.md). This suggests a design (called
[opaque reference types](GC.md#opaque-reference-types) below) that hides the
details of JavaScript and WebIDL behind Web-embedding-specific builtin modules.
On the other hand, WebAssembly can define a set of [native GC](GC.md#native-gc)
primitives that allowed portable GC code to be written regardless of the
host environment.

## Opaque reference types

The first feature is to extend [module imports](Modules.md#imports-and-exports)
to allow modules to import *opaque reference types*. "Opaque" means that the
reference type itself has no structural content and does not, e.g., define any
methods or fields. Once imported, an opaque reference type can be used in the
signature of other imported functions. Thus, the point of an opaque reference
type is to be passed to and returned from exported functions.

Reference types are allowed to be used as the types of locals, parameters
and return types. Additionally, references would be allowed as operands to
operators that treat their values as black boxes (`br`, `block`, etc.).
A new `dynamic_cast` operator would be added to allow checked
casting from any opaque reference type to any other opaque reference type.
Whether the cast succeeds is up to the host environment; WebAssembly itself
will define no a priori subtyping relationship.

For reasons of safety and limiting nondeterminism, imported opaque reference
types would not be able to be loaded from or stored to linear memory where they
could otherwise be arbitrarily aliased as integers. Instead, a new set of
operators would be added for allocating, deallocating, loading and storing
from integer-indexed cells that could hold references and were not aliasable by
linear memory.

With opaque reference types expressed as imports, host environments can provide
access to various kinds of reference-counted or garbage-collected host-defined
objects via builtin modules. While this design does not mandate a JavaScript VM
or browser, it does allow natural integration with both
[JavaScript](GC.md#js-integration) and [WebIDL](GC.md#webidl-integration)
in a Web environment.

### JavaScript integration

Using [opaque reference types](GC.md#opaque-reference-types),
JavaScript values could be made accessible to WebAssembly code through a builtin
`js` module providing:

* an exported `string` opaque reference type and exported functions
  to allocate, query length, and index `string` values;
* an exported `object` opaque reference type and exported functions
  that correspond with the ES5 meta-object protocol including the 
  ability to `[[Call]]` function objects;
* further exported opaque reference types for symbols and value 
  types (including SIMD);
* an exported `value` opaque reference type with exported functions for
  constructing `value`s from integers, floats, `object`s, `string`s, etc and
  with exported functions for querying the type of a `value` and extracting the
  abovementioned payload types.

Since a browser's WebAssembly engine would have full knowledge of the `js`
builtin module, it should be able to optimize string/object accesses as well as
a normal JavaScript JIT compiler (perhaps even using the same JIT compiler).

### WebIDL integration

Using [opaque reference types](GC.md#opaque-reference-types), it would be
possible to allow direct access to DOM and Web APIs by mapping their
[WebIDL](http://www.w3.org/TR/WebIDL) interfaces to WebAssembly builtin module 
signatures. In particular:

* WebIDL interfaces (like 
  [WebGLRenderingContextBase](https://www.khronos.org/registry/webgl/specs/latest/1.0/#5.14)
  or [WebGLTexture](https://www.khronos.org/registry/webgl/specs/latest/1.0/#5.9))
  would map to exported [opaque reference types](GC.md#opaque-reference-types);
* methods of WebIDL interfaces would map to exported functions where the
  receiver was translated into an explicit argument and WebIDL value
  types were mapped to appropriate [value types](Semantics.md#types)
  (e.g., [bindTexture](https://www.khronos.org/registry/webgl/specs/latest/1.0/#5.14)
  would translate to `void (WebGLRenderingContextBase, int32, WebGLTexture?)`).

This high-level description glosses over many important details about WebIDL:

First, the WebIDL spec contains many JavaScript-specific details that are
unnecessary in a WebAssembly context. In particular, there are basically three
components specified by a WebIDL interface:

1. a signature declaration composed of language-independent data types (like
   IEEE754 doubles and floats);
2. a set of basic wellformedness checks that are executed on the arguments of
   the signature declared in (1); and
3. a JavaScript-specific algorithm that maps the arbitrary set of JavaScript
   values passed to a WebIDL invocation to the signature declared by (1) and
   checked by (2).

(1) and (2) of the WebIDL spec are meaningful to WebAssembly, but (3)
would effectively be skipped.

Another important issue is mapping WebIDL values types that aren't simple
[primitive types](http://www.w3.org/TR/WebIDL/#dfn-primitive-type):

* [Dictionary types](http://www.w3.org/TR/WebIDL/#idl-dictionary)
  would [appear](http://www.w3.org/TR/WebIDL/#es-dictionary) to require
  JavaScript objects but are actually defined as values such that they can
  be (and are, in various browser implementations) flattened to C structs.
  Thus, a natural WebAssembly binding would be to map dictionaries to structs
  in linear memory passed by reference (integer offset).
* The same goes for [sequence types](http://www.w3.org/TR/WebIDL/#idl-sequence).
* [Enumeration types](http://www.w3.org/TR/WebIDL/#es-enumeration) could be
  mapped to canonical integers.
* [Union types](http://www.w3.org/TR/WebIDL/#idl-union) could be handled in
  multiple ways. One option is to treat the union type itself as an importable
  opaque reference type (when all the elements are themselves reference types).
  Another option is to introduce an overload of each signature for each element
  of the union type such that all calls passed a single element type and the
  full Union Type was never explicitly represented in WebAssembly.
* [Callback function types](http://www.w3.org/TR/WebIDL/#es-callback-function)
  could map to a `(function pointer, environment pointer)` closure pair.

Overall, the goal of mapping WebIDL to WebAssembly builtin modules is to avoid
the need to define a duplicate WebAssembly interface for all Web APIs.  In
practice, some WebIDL patterns may have an unnatural or inefficient mapping
into WebAssembly such that new overloads and best practices would need to be
adopted. Over time, though, these rough edges would be ironed out leaving the
long term benefit of defining Web APIs with a single interface and ensuring
that JavaScript and WebAssembly always had access to the same raw functionality.

## Native GC

In contrast to *opaque* reference types, a second feature would be to allow
direct GC allocation and field access from WebAssembly code through
*non-opaque* reference types.

There is a lot of the design left to
consider for this feature, but a few points of tentative agreement are:

* To avoid baking in a single language's object model, define low-level GC
  primitives (viz., structs and arrays) and allow the source language compiler
  to build up features like virtual dispatch and access control.
* GC struct and array types would have associated *struct/array reference
  types* that were similar to and symmetric with 
  [opaque reference types](GC.md#opaque-reference-types)
  (just not opaque).
* The GC heap would be semantically distinct from linear memory and thus
  the fields of GC objects could safely hold reference types (unlike linear
  memory).
* The GC struct and array types could be passed to and from JavaScript
  by reflecting the WebAssembly GC objects in JavaScript using the 
  [Typed Objects](https://github.com/nikomatsakis/typed-objects-explainer/)
  proposal.

[future general]: FutureFeatures.md
