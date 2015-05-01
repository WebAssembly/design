# Binary Encoding

This document describes the binary encoding of the AST nodes defined in the [AST semantics](AstSemantics.md). 
For the context of this document, see the [Binary format](V1.md#binary-format) section of the [v.1 doc](V1.md).

*The current [polyfill](https://github.com/WebAssembly/polyfill) is a prototype and thus incomplete and not 
representative of an actual WebAssembly binary format. However, there are a few key design choices in the 
prototype format that significantly and demonstrably impact size and decode speed that are described below 
as a starting point for defining a real standard binary format.*

## Building blocks

### Variable-length integers

* LEB, except only for a uint32_t: explain

### Serialized opcode

* Currently, a single byte
* Reserve a bit to allow variable-length opcodes?  No, just reserve a single 8-bit value: explain.

### Constant pool

* Table of variable length integers, fixed-width floats/doubles.
* Still want immediate-literals because many single-use constants.
* Use heuristic to pick.

## Global structure

* A module contains:
  * a header followed by
  * a table containing, for each section, its type, offset (within the module), and byte length, followed by
  * a sequence of sections.
* A section contains:
  * a header followed by
  * the section contents (specific to the section type)
* A code section contains:
  * the generic section header
  * a table containing, for each function, it's signature, offset (within the seciton), sorted by offset, followed by
  * a sequence of functions
* A function contains:
  * a table containing, for each type, how many locals are indexed by the function body of that type
  * the serialized AST

## Serialized AST

* Preorder
  * Good for efficient single-pass validation+compilation: explain.
  * Good for polyfill: explain.
  * Allows type-segregated index spaces
  * Basic scheme: parent opcode, immediates (variable length), children
  * Variations: variable arity, ...
* Type-segregated index spaces
  * Take advantage of type inherent in parent nodes.
  * Keep index spaces small.
  * Good for immediate folding: need tiny index space to make room for immediate.
* Immediate folding.
  * 30-40% of bytes are get/setloc: explain why
  * Massive size reduction by folding immediate into these ops
  * Another 10% are literals, so another significant reduction

## Example of serialized AST

Show several example ASTs as SExprs, then serialization.

## Macro layer

Idea (not in prototype): can we have a macro layer on top of raw serialized AST?
The idea is to take advantage of program semantics to reduce redundancy that a
generic compression algorithm would miss because of differences in names (which a macro
can be parameterized over).
