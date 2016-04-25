# Binary Encoding

This document describes the [portable](Portability.md) binary encoding of the WebAssembly modules.

The binary encoding is a dense representation of module information that enables 
small files, fast decoding, and reduced memory usage.
See the [rationale document](Rationale.md#why-a-binary-encoding) for more detail.

The encoding is split into three layers:

* **Layer 0** is a simple pre-order encoding of the AST and related data structures.
  The encoding is dense and trivial to interact with, making it suitable for
  scenarios like JIT, instrumentation tools, and debugging.
* **Layer 1** provides structural compression on top of layer 0, exploiting
  specific knowledge about the nature of the syntax tree and its nodes.
  The structural compression introduces more efficient encoding of values, 
  rearranges values within the module, and prunes structurally identical
  tree nodes.
* **Layer 2** Layer 2 applies generic compression algorithms, like [gzip](http://www.gzip.org/) and [Brotli](https://datatracker.ietf.org/doc/draft-alakuijala-brotli/), that are already available in browsers and other tooling.

Most importantly, the layering approach allows development and standardization to
occur incrementally. For example, Layer 1 and Layer 2 encoding techniques can be
experimented with by application-level decompressing to the layer below. As 
compression techniques  stabilize, they can be standardized and moved into native 
implementations.

# Data types

### uint8
A single-byte unsigned integer.

### uint32
A four-byte little endian unsigned integer.

### varint32
A [Signed LEB128](https://en.wikipedia.org/wiki/LEB128#Signed_LEB128) variable-length integer, limited to int32 values.

### varuint1
A [LEB128](https://en.wikipedia.org/wiki/LEB128) variable-length integer, limited to the values 0 or 1. `varuint1` values may contain leading zeros. (This type is mainly used for compatibility with potential future extensions.)

### varuint32
A [LEB128](https://en.wikipedia.org/wiki/LEB128) variable-length integer, limited to uint32 values. `varuint32` values may contain leading zeros.

### varint64
A [Signed LEB128](https://en.wikipedia.org/wiki/LEB128#Signed_LEB128) variable-length integer, limited to int64 values.

### value_type
A single-byte unsigned integer indicating a [value type](AstSemantics.md#types). These types are encoded as:
* `1` indicating type `i32` 
* `2` indicating type `i64` 
* `3` indicating type `f32` 
* `4` indicating type `f64`


# Definitions

### Pre-order encoding
Refers to an approach for encoding syntax trees, where each node begins with an identifying binary
sequence, then followed recursively by any child nodes. 

* Examples
  * Given a simple AST node: `I32Add(left: AstNode, right: AstNode)`
    * First write the opcode for `I32Add` (uint8)
    * Then recursively write the left and right nodes.

  * Given a call AST node: `Call(callee_index: uint32_t, args: AstNode[])`
    * First write the opcode of `Call` (uint8)
    * Then write the (variable-length) integer `callee_index` (varuint32)
    * Then recursively write each argument node, where arity is determined by looking up `callee_index` in a table of signatures

# Module structure

The following documents the current prototype format. This format is based on and supersedes the v8-native prototype format, originally in a [public design doc](https://docs.google.com/document/d/1-G11CnMA0My20KI9D7dBR6ZCPOBCRD0oCH6SHCPFGx0/edit?usp=sharing).

## High-level structure

The module starts with a preamble of two fields:

| Field | Type | Description |
| ----- |  ----- | ----- |
| magic number | `uint32` |  Magic number `0x6d736100` (i.e., '\0asm') |
| version | `uint32` | Version number, currently 10. The version for MVP will be reset to 1. |

This preamble is followed by a sequence of sections. Each section is identified by an
immediate string. Sections whose identity is unknown to the WebAssembly
implementation are ignored and this is supported by including the size in bytes
for all sections. The encoding of sections is structured as follows:

| Field | Type | Description |
| ----- |  ----- | ----- |
| id_len | `varuint32` | section identifier string length |
| id_str | `bytes` | section identifier string of id_len bytes |
| payload_len  | `varuint32` | size of this section in bytes |
| payload_str  | `bytes` | content of this section, of length payload_len |

Each section is optional and may appear at most once.
Known sections (from this list) may not appear out of order.
The content of each section is encoded in its `payload_str`.

* [Type](#type-section) section
* [Import](#import-section) section
* [Function](#function-section) section
* [Table](#table-section) section
* [Memory](#memory-section) section
* [Export](#export-section) section
* [Start](#start-section) section
* [Code](#code-section) section
* [Data](#data-section) section
* [Name](#name-section) section

The end of the last present section must coincide with the last byte of the
module. The shortest valid module is 8 bytes (`magic number`, `version`,
followed by zero sections).

### Type section

ID: `type`

The type section declares all function signatures that will be used in the module.

| Field | Type | Description |
| ----- |  ----- | ----- |
| count | `varuint32` | count of type entries to follow |
| entries | `type_entry*` | repeated type entries as described below |

#### Type entry
| Field | Type | Description |
| ----- |  ----- | ----- |
| form | `uint8` | `0x40`, indicating a function type |
| param_count | `varuint32` | the number of parameters to the function |
| param_types | `value_type*` | the parameter types of the function |
| return_count | `varuint1` | the number of results from the function |
| return_type | `value_type?` | the result type of the function (if return_count is 1) |

(Note: In the future, this section may contain other forms of type entries as well, which can be distinguished by the `form` field.)

### Import section

ID: `import`

The import section declares all imports that will be used in the module.

| Field | Type | Description |
| ----- |  ----- | ----- |
| count | `varuint32` | count of import entries to follow |
| entries | `import_entry*` | repeated import entries as described below |

#### Import entry
| Field | Type | Description |
| ----- |  ----- | ----- |
| sig_index | `varuint32` | signature index of the import |
| module_len | `varuint32` | module string length |
| module_str | `bytes` | module string of `module_len` bytes |
| function_len | `varuint32` | function string length |
| function_str | `bytes` | function string of `function_len` bytes |

### Function section

ID: `function`

The function section _declares_ the signatures of all functions in the
module (their definitions appear in the [code section](#code-section)).

| Field | Type | Description |
| ----- |  ----- | ----- |
| count | `varuint32` | count of signature indices to follow |
| types | `varuint32*` | sequence of indices into the type section |

### Table section

ID: `table`

The table section defines the module's
[indirect function table](AstSemantics.md#calls).

| Field | Type | Description |
| ----- |  ----- | ----- |
| count | `varuint32` | count of entries to follow |
| entries | `varuint32*` | repeated indexes into the function section |

### Memory section

ID: `memory`

The memory section declares the size and characteristics of the memory
associated with the module.

| Field | Type | Description |
| ----- |  ----- | ----- |
| initial | `varuint32` | initial memory size in 64KiB pages |
| maximum | `varuint32` | maximum memory size in 64KiB pages |
| exported | `uint8` | `1` if the memory is visible outside the module |

### Export section

ID: `export`

The export section declares all exports from the module.

| Field | Type | Description |
| ----- |  ----- | ----- |
| count | `varuint32` | count of export entries to follow |
| entries | `export_entry*` | repeated export entries as described below |

#### Export entry
| Field | Type | Description |
| ----- |  ----- | ----- |
| func_index | `varuint32` | index into the function table |
| function_len | `varuint32` | function string length |
| function_str | `bytes` | function string of `function_len` bytes |

### Start section

ID: `start`

The start section declares the [start function](Modules.md#module-start-function).

| Field | Type | Description |
| ----- |  ----- | ----- |
| index | `varuint32` | start function index |

### Code section

ID: `code`

The code section contains a body for every function in the module.
The count of function declared in the [function section](#function-section)
and function bodies defined in this section must be the same and the `i`th
declaration corresponds to the `i`th function body.

| Field | Type |  Description |
| ----- |  ----- |  ----- |  ----- |
| count | `varuint32` | count of function bodies to follow |
| bodies | `function_body*` | sequence of [Function Bodies](#function-bodies) |

### Data section

ID: `data`

The data section declares the initialized data that is loaded
into the linear memory.

| Field | Type | Description |
| ----- |  ----- | ----- |
| count | `varuint32` | count of data segments to follow |
| entries | `data_segment*` | repeated data segments as described below |

a `data_segment` is:

| Field | Type | Description |
| ----- |  ----- | ----- |
| offset | `varuint32` | the offset in linear memory at which to store the data |
| size | `varuint32` | size of `data` (in bytes) |
| data | `bytes` | sequence of `size` bytes |

### Name section

ID: `name`

The names section does not change execution semantics and a validation error in
this section does not cause validation for the whole module to fail and is
instead treated as if the section was absent. The expectation is that, when a
binary WebAssembly module is viewed in a browser or other development
environment, the names in this section will be used as the names of functions
and locals in the [text format](TextFormat.md).

| Field | Type | Description |
| ----- |  ----- | ----- |
| count | `varuint32` | count of entries to follow |
| entries | `function_names*` | sequence of names |

The sequence of `function_names` assigns names to the corresponding
function index. The count may be greater or less than the actual number of
functions.

#### Function names

| Field | Type | Description |
| ----- |  ----- | ----- |
| fun_name_len | `varuint32` | string length, in bytes |
| fun_name_str | `bytes` | valid utf8 encoding |
| local_count | `varuint32` | count of local names to follow |
| local_names | `local_name*` | sequence of local names |

The sequence of `local_name` assigns names to the corresponding local index. The
count may be greater or less than the actual number of locals.

#### Local name

| Field | Type | Description |
| ----- |  ----- | ----- |
| local_name_len | `varuint32` | string length, in bytes |
| local_name_str | `bytes` | valid utf8 encoding |


# Function Bodies

Function bodies consist of a sequence of local variable declarations followed by a 
dense pre-order encoding of an [Abstract Syntax Tree](AstSemantics.md).
Each node in the abstract syntax tree corresponds to an operator, such as `i32.add` or `if` or `block`.
Operators are encoding by an opcode byte followed by immediate bytes (if any), followed by children 
nodes (if any).

| Name | Opcode |Description |
| ----- | ----- | ----- |
| body size | `varuint32` | size of function body to follow, in bytes |
| local count | `varuint32` | number of local entries |
| locals | `local_entry*` | local variables |
| ast    | `byte*` | pre-order encoded AST |

#### Local Entry

Each local entry declares a number of local variables of a given type.
It is legal to have several entries with the same type.

| Field | Type | Description |
| ----- | ----- | ----- |
| count | `varuint32` | number of local variables of the following type |
| type | `value_type` | type of the variables |


## Control flow operators ([described here](AstSemantics.md#control-flow-structures))

| Name | Opcode | Immediate | Description |
| ---- | ---- | ---- | ---- |
| `nop` | `0x00` | | no operation |
| `block` | `0x01` |  | begin a sequence of expressions, the last of which yields a value |
| `loop` | `0x02` |  | begin a block which can also form control flow loops |
| `if` | `0x03` | | begin if expression |
| `else` | `0x04` | | begin else expression of if |
| `select` | `0x05` | | select one of two values based on condition |
| `br` | `0x06` | relative_depth = `varuint32` | break that targets an outer nested block |
| `br_if` | `0x07` | relative_depth = `varuint32` | conditional break that targets an outer nested block |
| `br_table` | `0x08` | see below | branch table control flow construct |
| `return` | `0x09` | | return zero or one value from this function |
| `unreachable` | `0x0a` | | trap immediately |
| `end` | `0x0f` | | end a block, loop, or if |

Note that there is no explicit `if_else` opcode, as the else clause is encoded with the `else` bytecode.

The `br_table` operator has an immediate operand which is encoded as follows:

| Field | Type | Description |
| ---- | ---- | ---- |
| target_count | `varuint32` | number of targets in the target_table |
| target_table | `uint32*` | target entries that indicate an outer block or loop to which to break |
| default_target | `uint32` | an outer block or loop to which to break in the default case |

The `br_table` operator implements an indirect branch. It accepts an optional value argument
(like other branches) and an additional `i32` expression as input, and 
branches to the block or loop at the given offset within the `target_table`. If the input value is 
out of range, `br_table` branches to the default target.

## Basic operators ([described here](AstSemantics.md#constants))
| Name | Opcode | Immediate | Description |
| ---- | ---- | ---- | ---- |
| `i32.const` | `0x10` | value = `varint32` | a constant value interpreted as `i32` |
| `i64.const` | `0x11` | value = `varint64` | a constant value interpreted as `i64` |
| `f64.const` | `0x12` | value = `uint64` | a constant value interpreted as `f64` |
| `f32.const` | `0x13` | value = `uint32` | a constant value interpreted as `f32` |
| `get_local` | `0x14` | local_index = `varuint32` | read a local variable or parameter |
| `set_local` | `0x15` | local_index = `varuint32` | write a local variable or parameter |
| `call` | `0x16` | function_index = `varuint32` | call a function by its index |
| `call_indirect` | `0x17` | signature_index = `varuint32` | call a function indirect with an expected signature |
| `call_import` | `0x18` | import_index = `varuint32` | call an imported function by its index |

## Memory-related operators ([described here](AstSemantics.md#linear-memory-accesses))

| Name | Opcode | Immediate | Description |
| ---- | ---- | ---- | ---- |
| `i32.load8_s` | `0x20` | `memory_immediate` | load from memory |
| `i32.load8_u` | `0x21` | `memory_immediate` | load from memory  |
| `i32.load16_s` | `0x22` | `memory_immediate` | load from memory |
| `i32.load16_u` | `0x23` | `memory_immediate` | load from memory |
| `i64.load8_s` | `0x24` | `memory_immediate` | load from memory |
| `i64.load8_u` | `0x25` | `memory_immediate` | load from memory |
| `i64.load16_s` | `0x26` | `memory_immediate` | load from memory |
| `i64.load16_u` | `0x27` | `memory_immediate` | load from memory |
| `i64.load32_s` | `0x28` | `memory_immediate` | load from memory |
| `i64.load32_u` | `0x29` | `memory_immediate` | load from memory |
| `i32.load` | `0x2a` | `memory_immediate` | load from memory |
| `i64.load` | `0x2b` | `memory_immediate` | load from memory |
| `f32.load` | `0x2c` | `memory_immediate` | load from memory |
| `f64.load` | `0x2d` | `memory_immediate` | load from memory |
| `i32.store8` | `0x2e` | `memory_immediate` | store to memory |
| `i32.store16` | `0x2f` | `memory_immediate` | store to memory |
| `i64.store8` | `0x30` | `memory_immediate` | store to memory |
| `i64.store16` | `0x31` | `memory_immediate` | store to memory |
| `i64.store32` | `0x32` | `memory_immediate` | store to memory |
| `i32.store` | `0x33` | `memory_immediate` | store to memory |
| `i64.store` | `0x34` | `memory_immediate` | store to memory |
| `f32.store` | `0x35` | `memory_immediate` | store to memory |
| `f64.store` | `0x36` | `memory_immediate` | store to memory |
| `memory_size` | `0x3b` |  | query the size of memory |
| `grow_memory` | `0x39` |  | grow the size of memory |

The `memory_immediate` type is encoded as follows:

| Name | Type | Description |
| ---- | ---- | ---- |
| flags | `varuint32` | a bitfield which currently contains the alignment in the least significant bits, encoded as `log2(alignment)` |
| offset | `varuint32` | the value of the offset |

As implied by the `log2(alignment)` encoding, the alignment must be a power of 2.
As an additional validation criteria, the alignment must be less or equal to 
natural alignment. The bits after the
`log(memory-access-size)` least-significant bits must be set to 0. These bits are reserved for future use
(e.g., for shared memory ordering requirements).

## Simple operators ([described here](AstSemantics.md#32-bit-integer-operators))

| Name | Opcode | Immediate | Description |
| ---- | ---- | ---- | ---- |
| `i32.add` | `0x40` | | |
| `i32.sub` | `0x41` | | |
| `i32.mul` | `0x42` | | |
| `i32.div_s` | `0x43` | | |
| `i32.div_u` | `0x44` | | |
| `i32.rem_s` | `0x45` | | |
| `i32.rem_u` | `0x46` | | |
| `i32.and` | `0x47` | | |
| `i32.or` | `0x48` | | |
| `i32.xor` | `0x49` | | |
| `i32.shl` | `0x4a` | | |
| `i32.shr_u` | `0x4b` | | |
| `i32.shr_s` | `0x4c` | | |
| `i32.rotr` | `0xb6` | | |
| `i32.rotl` | `0xb7` | | |
| `i32.eq` | `0x4d` | | |
| `i32.ne` | `0x4e` | | |
| `i32.lt_s` | `0x4f` | | |
| `i32.le_s` | `0x50` | | |
| `i32.lt_u` | `0x51` | | |
| `i32.le_u` | `0x52` | | |
| `i32.gt_s` | `0x53` | | |
| `i32.ge_s` | `0x54` | | |
| `i32.gt_u` | `0x55` | | |
| `i32.ge_u` | `0x56` | | |
| `i32.clz` | `0x57` | | |
| `i32.ctz` | `0x58` | | |
| `i32.popcnt` | `0x59` | | |
| `i32.eqz`  | `0x5a` | | |
| `i64.add` | `0x5b` | | |
| `i64.sub` | `0x5c` | | |
| `i64.mul` | `0x5d` | | |
| `i64.div_s` | `0x5e` | | |
| `i64.div_u` | `0x5f` | | |
| `i64.rem_s` | `0x60` | | |
| `i64.rem_u` | `0x61` | | |
| `i64.and` | `0x62` | | |
| `i64.or` | `0x63` | | |
| `i64.xor` | `0x64` | | |
| `i64.shl` | `0x65` | | |
| `i64.shr_u` | `0x66` | | |
| `i64.shr_s` | `0x67` | | |
| `i64.rotr` | `0xb8` | | |
| `i64.rotl` | `0xb9` | | |
| `i64.eq` | `0x68` | | |
| `i64.ne` | `0x69` | | |
| `i64.lt_s` | `0x6a` | | |
| `i64.le_s` | `0x6b` | | |
| `i64.lt_u` | `0x6c` | | |
| `i64.le_u` | `0x6d` | | |
| `i64.gt_s` | `0x6e` | | |
| `i64.ge_s` | `0x6f` | | |
| `i64.gt_u` | `0x70` | | |
| `i64.ge_u` | `0x71` | | |
| `i64.clz` | `0x72` | | |
| `i64.ctz` | `0x73` | | |
| `i64.popcnt` | `0x74` | | |
| `i64.eqz`  | `0xba` | | |
| `f32.add` | `0x75` | | |
| `f32.sub` | `0x76` | | |
| `f32.mul` | `0x77` | | |
| `f32.div` | `0x78` | | |
| `f32.min` | `0x79` | | |
| `f32.max` | `0x7a` | | |
| `f32.abs` | `0x7b` | | |
| `f32.neg` | `0x7c` | | |
| `f32.copysign` | `0x7d` | | |
| `f32.ceil` | `0x7e` | | |
| `f32.floor` | `0x7f` | | |
| `f32.trunc` | `0x80` | | |
| `f32.nearest` | `0x81` | | |
| `f32.sqrt` | `0x82` | | |
| `f32.eq` | `0x83` | | |
| `f32.ne` | `0x84` | | |
| `f32.lt` | `0x85` | | |
| `f32.le` | `0x86` | | |
| `f32.gt` | `0x87` | | |
| `f32.ge` | `0x88` | | |
| `f64.add` | `0x89` | | |
| `f64.sub` | `0x8a` | | |
| `f64.mul` | `0x8b` | | |
| `f64.div` | `0x8c` | | |
| `f64.min` | `0x8d` | | |
| `f64.max` | `0x8e` | | |
| `f64.abs` | `0x8f` | | |
| `f64.neg` | `0x90` | | |
| `f64.copysign` | `0x91` | | |
| `f64.ceil` | `0x92` | | |
| `f64.floor` | `0x93` | | |
| `f64.trunc` | `0x94` | | |
| `f64.nearest` | `0x95` | | |
| `f64.sqrt` | `0x96` | | |
| `f64.eq` | `0x97` | | |
| `f64.ne` | `0x98` | | |
| `f64.lt` | `0x99` | | |
| `f64.le` | `0x9a` | | |
| `f64.gt` | `0x9b` | | |
| `f64.ge` | `0x9c` | | |
| `i32.trunc_s/f32` | `0x9d` | | |
| `i32.trunc_s/f64` | `0x9e` | | |
| `i32.trunc_u/f32` | `0x9f` | | |
| `i32.trunc_u/f64` | `0xa0` | | |
| `i32.wrap/i64` | `0xa1` | | |
| `i64.trunc_s/f32` | `0xa2` | | |
| `i64.trunc_s/f64` | `0xa3` | | |
| `i64.trunc_u/f32` | `0xa4` | | |
| `i64.trunc_u/f64` | `0xa5` | | |
| `i64.extend_s/i32` | `0xa6` | | |
| `i64.extend_u/i32` | `0xa7` | | |
| `f32.convert_s/i32` | `0xa8` | | |
| `f32.convert_u/i32` | `0xa9` | | |
| `f32.convert_s/i64` | `0xaa` | | |
| `f32.convert_u/i64` | `0xab` | | |
| `f32.demote/f64` | `0xac` | | |
| `f32.reinterpret/i32` | `0xad` | | |
| `f64.convert_s/i32` | `0xae` | | |
| `f64.convert_u/i32` | `0xaf` | | |
| `f64.convert_s/i64` | `0xb0` | | |
| `f64.convert_u/i64` | `0xb1` | | |
| `f64.promote/f32` | `0xb2` | | |
| `f64.reinterpret/i64` | `0xb3` | | |
| `i32.reinterpret/f32` | `0xb4` | | |
| `i64.reinterpret/f64` | `0xb5` | | |


