# Binary Encoding

This document describes the binary encoding of the AST nodes defined in the [AST semantics](AstSemantics.md). 
For the context of this document, see the [Binary format](V1.md#binary-format) section of the [v.1 doc](V1.md).

*The current [polyfill](https://github.com/WebAssembly/polyfill) is a prototype and thus incomplete and not 
representative of an actual WebAssembly binary format. However, there are a few key design choices in the 
prototype format that significantly and demonstrably impact size and decode speed that are described below 
as a starting point for defining a real standard binary format.*

## Building blocks

### Variable-length integers
 * 31% size reduction before compression, 7% size reduction after compression.  
 * [LEB128](http://en.wikipedia.org/wiki/LEB128) except limited to uint32_t payloads.

### Constant pool
 * 7% size reduction before compression, 2% size reduction after compression.
 * Use a module-level constant pool for i32, f32, f64 literals.
 * Still want to allow literals as immediates if:
   * there is only one use of the literal
   * the variable-length-encoded literal pool index is bigger than the variable-length-encoded literal.
   * otherwise, the constant pool is only 5.6% win uncompressed / 0% win compressed.

### Context-dependent index spaces
 * With a pre-order encoding, a node is always encountered in a *context*: statement, i32-producing
   expression, f32-producing expression, etc.
 * Different contexts can reuse the same indices since there is never an ambiguity.
   * For example, I32::Add can have the same index as F32::Add.
   * This avoids opcodes spilling over into requiring >1 bytes.
   * When SIMD is added, new opcodes would mostly be in new type contexts, so still fit in a single byte.
 * This also enables the next technique:

### Fold immediates into the opcodes
 * 26% size reduction before compression, 5% size reduction after compression.
 * In the opcode byte:
   * High bit determines whether it is a normal op or an op-with-immediate.
     * If normal op: the remaining 7 bits are the opcode
     * If op-with-immediate: the next 2 bits are the opcode, the remaining 5 are the immediate
 * Current set of ops-with-immediate: Stmt::{SetLoc,SetGlo}, {I32,F32,F64}::{GetLoc,LitPool,LitImm}
   * How more than 4?  Context-dependent index spaces lets, e.g., I32WithImm overlap F32WithImm.
 * Just GetLoc+SetLoc accounts for 30-40% of total bytes in a module, and each has an immediate.
   * Isn't this high?  No, it's actually lower than a normal register-based bytecode which has to
     spend bytes on input/output operands for *every* instruction.
   * 98% of functions have < 32 variables so 5 bits of immediate is a good fit.
 * Literals (pool and immediate) account for ~10% of bytes.
   * Sorting constant pool by use count allows 50% of LitPool ops to use a folded immediate.

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
* Use a preorder encoding of the AST
  * Efficient single-pass validation+compilation and polyfill
  * Allows context-dependent index spaces (described above)
* The data of a node (if there is any), is written immediately after the opcode and before child nodes
  * The opcode statically determines what follows, so no generic metadata is necessary.
* Examples
  * Given a simple AST node: `struct I32Add { AstNode *left, *right; }`
    * First write the opcode of `I32Add` (1 byte)
    * Then recursively write the left and right nodes.
  * Given a call AST node: `struct Call { uint32_t callee; vector<AstNode*> args; }`
    * First write the opcode of `Call` (1 byte)
    * Then write the (variable-length) integer `Call::callee` (1-5 bytes)
    * Then recursively write each arg node (arity is determined by looking up `callee` in table of signatures)
  * Given an AST node with foldable immediate: `struct GetLoc { uint32_t index; }`
    * If `GetLoc::index` < 32, write a single byte as described [above](BinaryEncoding.md#fold-immediates-into-opcodes).
    * Otherwise, write the opcode of `GetLoc` and the variable length integer `GetLoc::index`.

## Further ideas
* Macro layer on top of serialized AST (allow the logical equivalent of `#define ADD1(x) (x+1)`)
  * A simple variant would be just having nullary macros (`#define ADDX1 (x+1)`)
