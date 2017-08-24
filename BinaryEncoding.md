# Binary Encoding

This document describes the [portable](Portability.md) binary encoding of the WebAssembly modules.

The binary encoding is a dense representation of module information that enables
small files, fast decoding, and reduced memory usage.
See the [rationale document](Rationale.md#why-a-binary-encoding) for more detail.

[:unicorn:][future general] = Planned [future][future general] feature

The encoding is split into three layers:

* **Layer 0** is a simple binary encoding of the bytecode instructions and related data structures.
  The encoding is dense and trivial to interact with, making it suitable for
  scenarios like JIT, instrumentation tools, and debugging.
* **Layer 1** [:unicorn:][future compression] provides structural compression on top of layer 0, exploiting
  specific knowledge about the nature of the syntax tree and its nodes.
  The structural compression introduces more efficient encoding of values,
  rearranges values within the module, and prunes structurally identical
  tree nodes.
* **Layer 2** [:unicorn:][future compression] Layer 2 applies generic compression algorithms, like [gzip](http://www.gzip.org/) and [Brotli](https://datatracker.ietf.org/doc/draft-alakuijala-brotli/), that are already available in browsers and other tooling.

Most importantly, the layering approach allows development and standardization to
occur incrementally. For example, Layer 1 and Layer 2 encoding techniques can be
experimented with by application-level decompressing to the layer below. As
compression techniques  stabilize, they can be standardized and moved into native
implementations.

See
[proposed layer 1 compression :unicorn:][future compression]
for a proposal for layer 1 structural compression.


# Data types

## Numbers

### `uintN`
An unsigned integer of _N_ bits,
represented in _N_/8 bytes in [little endian](https://en.wikipedia.org/wiki/Endianness#Little-endian) order.
_N_ is either 8, 16, or 32.

### `varuintN`
A [LEB128](https://en.wikipedia.org/wiki/LEB128) variable-length integer, limited to _N_ bits (i.e., the values [0, 2^_N_-1]),
represented by _at most_ ceil(_N_/7) bytes that may contain padding `0x80` bytes.

Note: Currently, the only sizes used are `varuint1`, `varuint7`, and `varuint32`,
where the former two are used for compatibility with potential future extensions.

### `varintN`
A [Signed LEB128](https://en.wikipedia.org/wiki/LEB128#Signed_LEB128) variable-length integer, limited to _N_ bits (i.e., the values [-2^(_N_-1), +2^(_N_-1)-1]),
represented by _at most_ ceil(_N_/7) bytes that may contain padding `0x80` or `0xFF` bytes.

Note: Currently, the only sizes used are `varint7`, `varint32` and `varint64`.

## Instruction Opcodes

In the MVP, the opcodes of [instructions](Semantics.md) are all encoded in a
single byte since there are fewer than 256 opcodes. Future features like
[SIMD](FutureFeatures.md#fixed-width-simd) and [atomics](FutureFeatures.md#threads)
will bring the total count above 256 and so an extension scheme will be
necessary, designating one or more single-byte values as prefixes for multi-byte
opcodes.

## Language Types

All types are distinguished by a negative `varint7` values that is the first byte of their encoding (representing a type constructor):

| Opcode | Type constructor |
|--------|------------------|
| `-0x01` (i.e., the byte `0x7f`) | `i32` |
| `-0x02` (i.e., the byte `0x7e`) | `i64` |
| `-0x03` (i.e., the byte `0x7d`) | `f32` |
| `-0x04` (i.e., the byte `0x7c`) | `f64` |
| `-0x10` (i.e., the byte `0x70`) | `anyfunc` |
| `-0x20` (i.e., the byte `0x60`) | `func` |
| `-0x40` (i.e., the byte `0x40`) | pseudo type for representing an empty `block_type` |

Some of these will be followed by additional fields, see below.

Note: Gaps are reserved for [future :unicorn:][future general] extensions. The use of a signed scheme is so that types can coexist in a single space with (positive) indices into the type section, which may be relevant for future extensions of the type system.

### `value_type`
A `varint7` indicating a [value type](Semantics.md#types). One of:

* `i32`
* `i64`
* `f32`
* `f64`

as encoded above.

### `block_type`
A `varint7` indicating a block signature. These types are encoded as:

* either a [`value_type`](#value_type) indicating a signature with a single result
* or `-0x40` (i.e., the byte `0x40`) indicating a signature with 0 results.

### `elem_type`

A `varint7` indicating the types of elements in a [table](Semantics.md#table).
In the MVP, only one type is available:

* [`anyfunc`](Semantics.md#table)

Note: In the [future :unicorn:][future general], other element types may be allowed.

### `func_type`
The description of a function signature. Its type constructor is followed by an additional description:

| Field | Type | Description |
| ----- | ---- | ----------- |
| form | `varint7` | the value for the `func` type constructor as defined above |
| param_count | `varuint32` | the number of parameters to the function |
| param_types | `value_type*` | the parameter types of the function |
| return_count | `varuint1` | the number of results from the function |
| return_type | `value_type?` | the result type of the function (if return_count is 1) |

Note: In the [future :unicorn:][future multiple return], `return_count` and `return_type` might be generalised to allow multiple values.

## Other Types

### `global_type`
The description of a global variable.

| Field | Type | Description |
| ----- | ---- | ----------- |
| content_type | `value_type` | type of the value |
| mutability | `varuint1` | `0` if immutable, `1` if mutable |

### `table_type`
The description of a table.

| Field | Type | Description |
| ----- | ---- | ----------- |
| element_type | `elem_type` | the type of elements |
| limits | `resizable_limits` | see [below](#resizable_limits) |

### `memory_type`
The description of a memory.

| Field | Type | Description |
| ----- | ---- | ----------- |
| limits | `resizable_limits` | see [below](#resizable_limits) |

### `external_kind`
A single-byte unsigned integer indicating the kind of definition being imported or defined:

* `0` indicating a `Function` [import](Modules.md#imports) or [definition](Modules.md#function-and-code-sections)
* `1` indicating a `Table` [import](Modules.md#imports) or [definition](Modules.md#table-section)
* `2` indicating a `Memory` [import](Modules.md#imports) or [definition](Modules.md#linear-memory-section)
* `3` indicating a `Global` [import](Modules.md#imports) or [definition](Modules.md#global-section)

### `resizable_limits`
A packed tuple that describes the limits of a
[table](Semantics.md#table) or [memory](Semantics.md#resizing):

| Field | Type | Description |
| ----- |  ----- | ----- |
| flags | `varuint1` | `1` if the maximum field is present, `0` otherwise |
| initial | `varuint32` | initial length (in units of table elements or wasm pages) |
| maximum | `varuint32`? | only present if specified by `flags` |

Note: In the [future :unicorn:][future threads], the "flags" field may be changed to `varuint32`, e.g., to include a flag for sharing between threads.

### `init_expr`
The encoding of an [initializer expression](Modules.md#initializer-expression)
is the normal encoding of the expression followed by the `end` opcode as a
delimiter.

Note that `get_global` in an initializer expression can only refer to immutable
imported globals and all uses of `init_expr` can only appear after the Imports
section.

# Module structure

The following documents the current prototype format. This format is based on and supersedes the v8-native prototype format, originally in a [public design doc](https://docs.google.com/document/d/1-G11CnMA0My20KI9D7dBR6ZCPOBCRD0oCH6SHCPFGx0/edit?usp=sharing).

## High-level structure

The module starts with a preamble of two fields:

| Field | Type | Description |
| ----- |  ----- | ----- |
| magic number | `uint32` |  Magic number `0x6d736100` (i.e., '\0asm') |
| version | `uint32` | Version number, `0x1` |

The module preamble is followed by a sequence of sections.
Each section is identified by a 1-byte *section code* that encodes either a known section or a custom section.
The section length and payload data then follow.
Known sections have non-zero ids, while custom sections have a `0` id followed by an identifying string as
part of the payload.

| Field | Type | Description |
| ----- |  ----- | ----- |
| id | `varuint7` | section code |
| payload_len  | `varuint32` | size of this section in bytes |
| name_len | `varuint32` ? | length of `name` in bytes, present if `id == 0` |
| name | `bytes` ? | section name: valid UTF-8 byte sequence, present if `id == 0` |
| payload_data  | `bytes` | content of this section, of length `payload_len - sizeof(name) - sizeof(name_len)` |

Each known section is optional and may appear at most once. Custom sections all have the same `id` (0), and can be named non-uniquely (all bytes composing their names may be identical).

Custom sections are intended to be used for debugging information, future evolution, or third party extensions. For MVP, we use a specific custom section (the [Name Section](#name-section)) for debugging information.

If a WebAssembly implementation interprets the payload of any custom section during module validation or compilation, errors in that payload must not invalidate the module.

Known sections from the list below may not appear out of order, while custom sections may be interspersed before, between, as well as after any of the elements of the list, in any order. Certain custom sections may have their own ordering and cardinality requirements. For example, the [Name section](#name-section) is expected to appear at most once, immediately after the Data section. Violation of such requirements may at most cause an implementation to ignore the section, while not invalidating the module.

The content of each section is encoded in its `payload_data`.

| Section Name | Code | Description |
| ------------ | ---- | ----------- |
| [Type](#type-section) | `1` | Function signature declarations |
| [Import](#import-section) | `2` | Import declarations |
| [Function](#function-section) | `3` | Function declarations |
| [Table](#table-section) | `4` | Indirect function table and other tables |
| [Memory](#memory-section) | `5` | Memory attributes |
| [Global](#global-section) | `6` | Global declarations |
| [Export](#export-section) | `7` | Exports |
| [Start](#start-section) | `8` | Start function declaration |
| [Element](#element-section) | `9` | Elements section |
| [Code](#code-section) | `10` | Function bodies (code) |
| [Data](#data-section) | `11` | Data segments |

The end of the last present section must coincide with the last byte of the
module. The shortest valid module is 8 bytes (`magic number`, `version`,
followed by zero sections).

### Type section

The type section declares all function signatures that will be used in the module.

| Field | Type | Description |
| ----- | ---- | ----------- |
| count | `varuint32` | count of type entries to follow |
| entries | `func_type*` | repeated type entries as described [above](#func_type) |

Note: In the [future :unicorn:][future types],
this section may contain other forms of type entries as well, which can be distinguished by the `form` field of the type encoding.

### Import section

The import section declares all imports that will be used in the module.

| Field | Type | Description |
| ----- | ---- | ----------- |
| count | `varuint32` | count of import entries to follow |
| entries | `import_entry*` | repeated import entries as described below |

#### Import entry

| Field | Type | Description |
| ----- | ---- | ----------- |
| module_len | `varuint32` | length of `module_str` in bytes |
| module_str | `bytes` | module name: valid UTF-8 byte sequence |
| field_len | `varuint32` | length of `field_str` in bytes |
| field_str | `bytes` | field name: valid UTF-8 byte sequence |
| kind | `external_kind` | the kind of definition being imported |

Followed by, if the `kind` is `Function`:

| Field | Type | Description |
| ----- | ---- | ----------- |
| type | `varuint32` | type index of the function signature |

or, if the `kind` is `Table`:

| Field | Type | Description |
| ----- | ---- | ----------- |
| type | `table_type` | type of the imported table |

or, if the `kind` is `Memory`:

| Field | Type | Description |
| ----- | ---- | ----------- |
| type | `memory_type` | type of the imported memory |

or, if the `kind` is `Global`:

| Field | Type | Description |
| ----- | ---- | ----------- |
| type | `global_type` | type of the imported global |

Note that, in the MVP, only immutable global variables can be imported.

### Function section

The function section _declares_ the signatures of all functions in the
module (their definitions appear in the [code section](#code-section)).

| Field | Type | Description |
| ----- | ---- | ----------- |
| count | `varuint32` | count of signature indices to follow |
| types | `varuint32*` | sequence of indices into the type section |

### Table section

The encoding of a [Table section](Modules.md#table-section):

| Field | Type | Description |
| ----- |  ----- | ----- |
| count | `varuint32` | indicating the number of tables defined by the module |
| entries | `table_type*` | repeated `table_type` entries as described [above](#table_type) |

In the MVP, the number of tables must be no more than 1.

### Memory section

ID: `memory`

The encoding of a [Memory section](Modules.md#linear-memory-section):

| Field | Type | Description |
| ----- |  ----- | ----- |
| count | `varuint32` | indicating the number of memories defined by the module |
| entries | `memory_type*` | repeated `memory_type` entries as described [above](#memory_type) |

Note that the initial/maximum fields are specified in units of
[WebAssembly pages](Semantics.md#linear-memory).

In the MVP, the number of memories must be no more than 1.

### Global section

The encoding of the [Global section](Modules.md#global-section):

| Field | Type | Description |
| ----- | ---- | ----------- |
| count | `varuint32` | count of global variable entries |
| globals | `global_variable*` | global variables, as described below |

#### Global Entry

Each `global_variable` declares a single global variable of a given type, mutability
and with the given initializer.

| Field | Type | Description |
| ----- | ---- | ----------- |
| type | `global_type` | type of the variables |
| init | `init_expr` | the initial value of the global |

Note that, in the MVP, only immutable global variables can be exported.

### Export section

The encoding of the [Export section](Modules.md#exports):

| Field | Type | Description |
| ----- | ---- | ----------- |
| count | `varuint32` | count of export entries to follow |
| entries | `export_entry*` | repeated export entries as described below |

#### Export entry

| Field | Type | Description |
| ----- | ---- | ----------- |
| field_len | `varuint32` | length of `field_str` in bytes |
| field_str | `bytes` | field name: valid UTF-8 byte sequence |
| kind | `external_kind` | the kind of definition being exported |
| index | `varuint32` | the index into the corresponding [index space](Modules.md) |

For example, if the "kind" is `Function`, then "index" is a
[function index](Modules.md#function-index-space). Note that, in the MVP, the
only valid index value for a memory or table export is 0.

### Start section

The start section declares the [start function](Modules.md#module-start-function).

| Field | Type | Description |
| ----- | ---- | ----------- |
| index | `varuint32` | start [function index](Modules.md#function-index-space) |

### Element section

The encoding of the [Elements section](Modules.md#elements-section):

| Field | Type | Description |
| ----- | ---- | ----------- |
| count | `varuint32` | count of element segments to follow |
| entries | `elem_segment*` | repeated element segments as described below |

a `elem_segment` is:

| Field | Type | Description |
| ----- | ---- | ----------- |
| index | `varuint32` | the [table index](Modules.md#table-index-space) (0 in the MVP) |
| offset | `init_expr` | an `i32` initializer expression that computes the offset at which to place the elements |
| num_elem | `varuint32` | number of elements to follow |
| elems | `varuint32*` | sequence of [function indices](Modules.md#function-index-space) |

### Code section

ID: `code`

The code section contains a body for every function in the module.
The count of function declared in the [function section](#function-section)
and function bodies defined in this section must be the same and the `i`th
declaration corresponds to the `i`th function body.

| Field | Type | Description |
| ----- | ---- | ----------- |
| count | `varuint32` | count of function bodies to follow |
| bodies | `function_body*` | sequence of [Function Bodies](#function-bodies) |

### Data section

The data section declares the initialized data that is loaded
into the linear memory.

| Field | Type | Description |
| ----- | ---- | ----------- |
| count | `varuint32` | count of data segments to follow |
| entries | `data_segment*` | repeated data segments as described below |

a `data_segment` is:

| Field | Type | Description |
| ----- | ---- | ----------- |
| index | `varuint32` | the [linear memory index](Modules.md#linear-memory-index-space) (0 in the MVP) |
| offset | `init_expr` | an `i32` initializer expression that computes the offset at which to place the data |
| size | `varuint32` | size of `data` (in bytes) |
| data | `bytes` | sequence of `size` bytes |

### Name section

Custom section `name` field: `"name"`

The name section is a [custom section](#high-level-structure).  It is therefore
encoded with id `0` followed by the name string `"name"`.  Like all custom
sections, this section being malformed does not cause the validation of the
module to fail. It is up to the implementation how it handles a malformed or
partially malformed name section. The WebAssembly implementation is also free to
choose to read and process this section lazily, after the module has been
instantiated, should debugging be required.

The name section may appear only once, and only after the [Data
section](#Data-section).  The expectation is that, when a binary WebAssembly
module is viewed in a browser or other development environment, the data in this
section will be used as the names of functions and locals in the [text
format](TextFormat.md).

The name section contains a sequence of name subsections:

| Field | Type | Description |
| ----- | ---- | ----------- |
| name_type | `varuint7` | code identifying type of name contained in this subsection |
| name_payload_len | `varuint32` | size of this subsection in bytes |
| name_payload_data | `bytes` | content of this section, of length `name_payload_len` |

Since name subsections have a given length, unknown or unwanted subsections can
be skipped over by an engine. The current list of valid `name_type` codes are:

| Name Type                   | Code | Description                          |
| --------------------------- | ---- | ------------------------------------ |
| [Module](#module-name)      | `0`  | Assigns a name to the module         |
| [Function](#function-names) | `1`  | Assigns names to functions           |
| [Local](#local-names)       | `2`  | Assigns names to locals in functions |


When present, subsections must appear in this order and at most once. The
end of the last subsection must coincide with the last byte of the name
section to be a well-formed name section.

#### Module name

The module name subsection assigns a name to the module itself. It simply
consists of a single string:

| Field    | Type        | Description                   |
| -------- | ----------- | ----------------------------- |
| name_len | `varuint32` | length of `name_str` in bytes |
| name_str | `bytes`     | UTF-8 encoding of the name    |

#### Name Map

In the following subsections, a `name_map` is encoded as:

| Field | Type        | Description                          |
| ----- | ----------- | ------------------------------------ |
| count | `varuint32` | number of `naming` in names          |
| names | `naming*`   | sequence of `naming` sorted by index |

where a `naming` is encoded as:

| Field    | Type        | Description                    |
| -------- | ----------- | ------------------------------ |
| index    | `varuint32` | the index which is being named |
| name_len | `varuint32` | length of `name_str` in bytes  |
| name_str | `bytes`     | UTF-8 encoding of the name     |

#### Function names

The function names subsection is a `name_map` which assigns names to
a subset of the [function index space](Modules.md#function-index-space)
(both imports and module-defined).

Each function may be named at most once.  Naming a function more than once
results in the section being malformed.

However, names need not be unique.  The same name may be given for multiple
functions. This is common for C++ programs where the multiple compilation units
that comprise a binary can contain local functions with the same name.

#### Local names

The local names subsection assigns `name_map`s to a subset of functions in the
[function index space](Modules.md#function-index-space) (both imports and
module-defined). The `name_map` for a given function assigns names to a
subset of local variable indices.

| Field | Type           | Description                               |
| ----- | -------------- | ----------------------------------------- |
| count | `varuint32`    | count of `local_names` in funcs           |
| funcs | `local_names*` | sequence of `local_names` sorted by index |

where a `local_name` is encoded as:

| Field | Type | Description |
| ----- | ---- | ----------- |
| index | `varuint32` | the index of the function whose locals are being named |
| local_map | `name_map` | assignment of names to local indices |

# Function Bodies

Function bodies consist of a sequence of local variable declarations followed by
[bytecode instructions](Semantics.md). Instructions are encoded as an
[opcode](#instruction-opcodes) followed by zero or more *immediates* as defined
by the tables below. Each function body must end with the `end` opcode.

| Field | Type | Description |
| ----- | ---- | ----------- |
| body_size | `varuint32` | size of function body to follow, in bytes |
| local_count | `varuint32` | number of local entries |
| locals | `local_entry*` | local variables |
| code | `byte*` | bytecode of the function |
| end | `byte` | `0x0b`, indicating the end of the body |

#### Local Entry

Each local entry declares a number of local variables of a given type.
It is legal to have several entries with the same type.

| Field | Type | Description |
| ----- | ---- | ----------- |
| count | `varuint32` | number of local variables of the following type |
| type | `value_type` | type of the variables |


## Control flow operators ([described here](Semantics.md#control-flow-structures))

| Name | Opcode | Immediates | Description |
| ---- | ---- | ---- | ---- |
| `unreachable` | `0x00` | | trap immediately |
| `nop` | `0x01` | | no operation |
| `block` | `0x02` | sig : `block_type` | begin a sequence of expressions, yielding 0 or 1 values |
| `loop` | `0x03` |  sig : `block_type` | begin a block which can also form control flow loops |
| `if` | `0x04` | sig : `block_type` | begin if expression |
| `else` | `0x05` | | begin else expression of if |
| `end` | `0x0b` | | end a block, loop, or if |
| `br` | `0x0c` | relative_depth : `varuint32` | break that targets an outer nested block |
| `br_if` | `0x0d` | relative_depth : `varuint32` | conditional break that targets an outer nested block |
| `br_table` | `0x0e` | see below | branch table control flow construct |
| `return` | `0x0f` | | return zero or one value from this function |

The _sig_ fields of `block` and `if` operators specify function signatures
which describe their use of the operand stack.

The `br_table` operator has an immediate operand which is encoded as follows:

| Field | Type | Description |
| ---- | ---- | ---- |
| target_count | `varuint32` | number of entries in the target_table |
| target_table | `varuint32*` | target entries that indicate an outer block or loop to which to break |
| default_target | `varuint32` | an outer block or loop to which to break in the default case |

The `br_table` operator implements an indirect branch. It accepts an optional value argument
(like other branches) and an additional `i32` expression as input, and
branches to the block or loop at the given offset within the `target_table`. If the input value is
out of range, `br_table` branches to the default target.

Note: Gaps in the opcode space, here and elsewhere, are reserved for
[future :unicorn:][future general] extensions.

## Call operators ([described here](Semantics.md#calls))

| Name | Opcode | Immediates | Description |
| ---- | ---- | ---- | ---- |
| `call` | `0x10` | function_index : `varuint32` | call a function by its [index](Modules.md#function-index-space) |
| `call_indirect` | `0x11` | type_index : `varuint32`, reserved : `varuint1` | call a function indirect with an expected signature |

The `call_indirect` operator takes a list of function arguments and as the last
operand the index into the table. Its `reserved` immediate is for
[future :unicorn:][future multiple tables] use and must be `0` in the MVP.

## Parametric operators ([described here](Semantics.md#type-parametric-operators))

| Name | Opcode | Immediates | Description |
| ---- | ---- | ---- | ---- |
| `drop` | `0x1a` | | ignore value |
| `select` | `0x1b` | | select one of two values based on condition |

## Variable access ([described here](Semantics.md#local-variables))

| Name | Opcode | Immediates | Description |
| ---- | ---- | ---- | ---- |
| `get_local` | `0x20` | local_index : `varuint32` | read a local variable or parameter |
| `set_local` | `0x21` | local_index : `varuint32` | write a local variable or parameter |
| `tee_local` | `0x22` | local_index : `varuint32` | write a local variable or parameter and return the same value |
| `get_global` | `0x23` | global_index : `varuint32` | read a global variable |
| `set_global` | `0x24` | global_index : `varuint32` | write a global variable |

## Memory-related operators ([described here](Semantics.md#linear-memory-accesses))

| Name | Opcode | Immediate | Description |
| ---- | ---- | ---- | ---- |
| `i32.load` | `0x28` | `memory_immediate` | load from memory |
| `i64.load` | `0x29` | `memory_immediate` | load from memory |
| `f32.load` | `0x2a` | `memory_immediate` | load from memory |
| `f64.load` | `0x2b` | `memory_immediate` | load from memory |
| `i32.load8_s` | `0x2c` | `memory_immediate` | load from memory |
| `i32.load8_u` | `0x2d` | `memory_immediate` | load from memory  |
| `i32.load16_s` | `0x2e` | `memory_immediate` | load from memory |
| `i32.load16_u` | `0x2f` | `memory_immediate` | load from memory |
| `i64.load8_s` | `0x30` | `memory_immediate` | load from memory |
| `i64.load8_u` | `0x31` | `memory_immediate` | load from memory |
| `i64.load16_s` | `0x32` | `memory_immediate` | load from memory |
| `i64.load16_u` | `0x33` | `memory_immediate` | load from memory |
| `i64.load32_s` | `0x34` | `memory_immediate` | load from memory |
| `i64.load32_u` | `0x35` | `memory_immediate` | load from memory |
| `i32.store` | `0x36` | `memory_immediate` | store to memory |
| `i64.store` | `0x37` | `memory_immediate` | store to memory |
| `f32.store` | `0x38` | `memory_immediate` | store to memory |
| `f64.store` | `0x39` | `memory_immediate` | store to memory |
| `i32.store8` | `0x3a` | `memory_immediate` | store to memory |
| `i32.store16` | `0x3b` | `memory_immediate` | store to memory |
| `i64.store8` | `0x3c` | `memory_immediate` | store to memory |
| `i64.store16` | `0x3d` | `memory_immediate` | store to memory |
| `i64.store32` | `0x3e` | `memory_immediate` | store to memory |
| `current_memory` | `0x3f` | reserved : `varuint1` | query the size of memory |
| `grow_memory` | `0x40` | reserved : `varuint1` | grow the size of memory |

The `memory_immediate` type is encoded as follows:

| Name | Type | Description |
| ---- | ---- | ---- |
| flags | `varuint32` | a bitfield which currently contains the alignment in the least significant bits, encoded as `log2(alignment)` |
| offset | `varuint32` | the value of the offset |

As implied by the `log2(alignment)` encoding, the alignment must be a power of 2.
As an additional validation criteria, the alignment must be less or equal to
natural alignment. The bits after the
`log(memory-access-size)` least-significant bits must be set to 0. These bits
are reserved for [future :unicorn:][future threads] use
(e.g., for shared memory ordering requirements).

The `reserved` immediate to the `current_memory` and `grow_memory` operators is
for [future :unicorn:][future multiple tables] use and must be 0 in the MVP.

## Constants ([described here](Semantics.md#constants))

| Name | Opcode | Immediates | Description |
| ---- | ---- | ---- | ---- |
| `i32.const` | `0x41` | value : `varint32` | a constant value interpreted as `i32` |
| `i64.const` | `0x42` | value : `varint64` | a constant value interpreted as `i64` |
| `f32.const` | `0x43` | value : `uint32` | a constant value interpreted as `f32` |
| `f64.const` | `0x44` | value : `uint64` | a constant value interpreted as `f64` |

## Comparison operators ([described here](Semantics.md#32-bit-integer-operators))

| Name | Opcode | Immediate | Description |
| ---- | ---- | ---- | ---- |
| `i32.eqz`  | `0x45` | | |
| `i32.eq` | `0x46` | | |
| `i32.ne` | `0x47` | | |
| `i32.lt_s` | `0x48` | | |
| `i32.lt_u` | `0x49` | | |
| `i32.gt_s` | `0x4a` | | |
| `i32.gt_u` | `0x4b` | | |
| `i32.le_s` | `0x4c` | | |
| `i32.le_u` | `0x4d` | | |
| `i32.ge_s` | `0x4e` | | |
| `i32.ge_u` | `0x4f` | | |
| `i64.eqz`  | `0x50` | | |
| `i64.eq` | `0x51` | | |
| `i64.ne` | `0x52` | | |
| `i64.lt_s` | `0x53` | | |
| `i64.lt_u` | `0x54` | | |
| `i64.gt_s` | `0x55` | | |
| `i64.gt_u` | `0x56` | | |
| `i64.le_s` | `0x57` | | |
| `i64.le_u` | `0x58` | | |
| `i64.ge_s` | `0x59` | | |
| `i64.ge_u` | `0x5a` | | |
| `f32.eq` | `0x5b` | | |
| `f32.ne` | `0x5c` | | |
| `f32.lt` | `0x5d` | | |
| `f32.gt` | `0x5e` | | |
| `f32.le` | `0x5f` | | |
| `f32.ge` | `0x60` | | |
| `f64.eq` | `0x61` | | |
| `f64.ne` | `0x62` | | |
| `f64.lt` | `0x63` | | |
| `f64.gt` | `0x64` | | |
| `f64.le` | `0x65` | | |
| `f64.ge` | `0x66` | | |

## Numeric operators ([described here](Semantics.md#32-bit-integer-operators))

| Name | Opcode | Immediate | Description |
| ---- | ---- | ---- | ---- |
| `i32.clz` | `0x67` | | |
| `i32.ctz` | `0x68` | | |
| `i32.popcnt` | `0x69` | | |
| `i32.add` | `0x6a` | | |
| `i32.sub` | `0x6b` | | |
| `i32.mul` | `0x6c` | | |
| `i32.div_s` | `0x6d` | | |
| `i32.div_u` | `0x6e` | | |
| `i32.rem_s` | `0x6f` | | |
| `i32.rem_u` | `0x70` | | |
| `i32.and` | `0x71` | | |
| `i32.or` | `0x72` | | |
| `i32.xor` | `0x73` | | |
| `i32.shl` | `0x74` | | |
| `i32.shr_s` | `0x75` | | |
| `i32.shr_u` | `0x76` | | |
| `i32.rotl` | `0x77` | | |
| `i32.rotr` | `0x78` | | |
| `i64.clz` | `0x79` | | |
| `i64.ctz` | `0x7a` | | |
| `i64.popcnt` | `0x7b` | | |
| `i64.add` | `0x7c` | | |
| `i64.sub` | `0x7d` | | |
| `i64.mul` | `0x7e` | | |
| `i64.div_s` | `0x7f` | | |
| `i64.div_u` | `0x80` | | |
| `i64.rem_s` | `0x81` | | |
| `i64.rem_u` | `0x82` | | |
| `i64.and` | `0x83` | | |
| `i64.or` | `0x84` | | |
| `i64.xor` | `0x85` | | |
| `i64.shl` | `0x86` | | |
| `i64.shr_s` | `0x87` | | |
| `i64.shr_u` | `0x88` | | |
| `i64.rotl` | `0x89` | | |
| `i64.rotr` | `0x8a` | | |
| `f32.abs` | `0x8b` | | |
| `f32.neg` | `0x8c` | | |
| `f32.ceil` | `0x8d` | | |
| `f32.floor` | `0x8e` | | |
| `f32.trunc` | `0x8f` | | |
| `f32.nearest` | `0x90` | | |
| `f32.sqrt` | `0x91` | | |
| `f32.add` | `0x92` | | |
| `f32.sub` | `0x93` | | |
| `f32.mul` | `0x94` | | |
| `f32.div` | `0x95` | | |
| `f32.min` | `0x96` | | |
| `f32.max` | `0x97` | | |
| `f32.copysign` | `0x98` | | |
| `f64.abs` | `0x99` | | |
| `f64.neg` | `0x9a` | | |
| `f64.ceil` | `0x9b` | | |
| `f64.floor` | `0x9c` | | |
| `f64.trunc` | `0x9d` | | |
| `f64.nearest` | `0x9e` | | |
| `f64.sqrt` | `0x9f` | | |
| `f64.add` | `0xa0` | | |
| `f64.sub` | `0xa1` | | |
| `f64.mul` | `0xa2` | | |
| `f64.div` | `0xa3` | | |
| `f64.min` | `0xa4` | | |
| `f64.max` | `0xa5` | | |
| `f64.copysign` | `0xa6` | | |

## Conversions ([described here](Semantics.md#datatype-conversions))

| Name | Opcode | Immediate | Description |
| ---- | ---- | ---- | ---- |
| `i32.wrap/i64` | `0xa7` | | |
| `i32.trunc_s/f32` | `0xa8` | | |
| `i32.trunc_u/f32` | `0xa9` | | |
| `i32.trunc_s/f64` | `0xaa` | | |
| `i32.trunc_u/f64` | `0xab` | | |
| `i64.extend_s/i32` | `0xac` | | |
| `i64.extend_u/i32` | `0xad` | | |
| `i64.trunc_s/f32` | `0xae` | | |
| `i64.trunc_u/f32` | `0xaf` | | |
| `i64.trunc_s/f64` | `0xb0` | | |
| `i64.trunc_u/f64` | `0xb1` | | |
| `f32.convert_s/i32` | `0xb2` | | |
| `f32.convert_u/i32` | `0xb3` | | |
| `f32.convert_s/i64` | `0xb4` | | |
| `f32.convert_u/i64` | `0xb5` | | |
| `f32.demote/f64` | `0xb6` | | |
| `f64.convert_s/i32` | `0xb7` | | |
| `f64.convert_u/i32` | `0xb8` | | |
| `f64.convert_s/i64` | `0xb9` | | |
| `f64.convert_u/i64` | `0xba` | | |
| `f64.promote/f32` | `0xbb` | | |

## Reinterpretations ([described here](Semantics.md#datatype-conversions))

| Name | Opcode | Immediate | Description |
| ---- | ---- | ---- | ---- |
| `i32.reinterpret/f32` | `0xbc` | | |
| `i64.reinterpret/f64` | `0xbd` | | |
| `f32.reinterpret/i32` | `0xbe` | | |
| `f64.reinterpret/i64` | `0xbf` | | |

[future general]: FutureFeatures.md
[future multiple return]: FutureFeatures.md#multiple-return
[future threads]: FutureFeatures.md#threads
[future types]: FutureFeatures.md#more-table-operators-and-types
[future multiple tables]: FutureFeatures.md#multiple-tables-and-memories
[future compression]: https://github.com/WebAssembly/decompressor-prototype/blob/master/CompressionLayer1.md
