# Binary Encoding

This document describes the [portable](Portability.md) binary encoding of the
[Abstract Syntax Tree](AstSemantics.md) nodes.

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
* **Layer 2** applies generic compression techniques, already available
  in browsers and other tooling. Algorithms as simple as gzip can deliver
  good results, but more sophisticated algorithms like 
  [LZHAM](https://github.com/richgel999/lzham_codec) and
  [Brotli](https://datatracker.ietf.org/doc/draft-alakuijala-brotli/) are able
  to deliver dramatically smaller files.

Most importantly, the layering approach allows development and standardization to
occur incrementally, even though production-quality implementations will need to
implement all of the layers.

# Data types

### uint8
A single-byte unsigned integer.

### uint16
A two-byte little endian unsigned integer.

### uint32
A four-byte little endian unsigned integer.

### varuint32
A [LEB128](https://en.wikipedia.org/wiki/LEB128) variable-length integer, limited to uint32 payloads. Provides considerable size reduction.

# Definitions

### Pre-order encoding
Refers to an approach for encoding syntax trees, where each node begins with an identifier
followed by any arguments or child nodes. Pre-order trees can be decoded iteratively or 
recursively. Alternative approaches include post-order trees and table representations.

* Examples
  * Given a simple AST node: `struct I32Add { AstNode *left, *right; }`
    * First write the opcode of `I32Add` (uint8)
    * Then recursively write the left and right nodes.

  * Given a call AST node: `struct Call { uint32_t callee; vector<AstNode*> args; }`
    * First write the opcode of `Call` (uint8)
    * Then write the (variable-length) integer `Call::callee` (varuint32)
    * Then recursively write each argument node, where arity is determined by looking up `callee` in a table of signatures

### Strings
Strings referenced by the module (i.e. function names) are encoded as null-terminated [UTF8](http://unicode.org/faq/utf_bom.html#UTF8).

# Module structure

The following documents the current prototype format. This format is based on and supersedes the v8-native prototype format, originally in a [public design doc](https://docs.google.com/document/d/1-G11CnMA0My20KI9D7dBR6ZCPOBCRD0oCH6SHCPFGx0/edit?usp=sharing).

## High-level structure
A module contains a sequence of sections, each identified by a *section identifier byte* whose encodings
are listed below.

### Memory section
The memory section declares the size and characteristics of the memory associated with the module.
A module may contain at most one memory section.

| Field | Type | Description |
| ----- |  ----- | ----- | 
| id = `0x00` | `uint8` | section identifier for memory |
| min_mem_size | `uint8` | minimize memory size as a power of `2` |
| max_mem_size | `uint8` | maximum memory size as a power of `2` |
| exported | `uint8` | `1` if the memory is visible outside the module |

### Signatures section
The signatures section declares all function signatures that will be used in the module.
A module may contain at most one signatures section.

| Field | Type | Description |
| ----- |  ----- | ----- | 
| id = `0x01` | `uint8` | section identifier for signatures |
| count | `varuint32` | count of signature entries to follow | 
| entries | `signature_entry*` | repeated signature entries as described below |

#### Signature entry
| Field | Type | Description |
| ----- |  ----- | ----- | 
| param_count | `uint8` | the number of parameters to the function |
| return_type | `local_type?` | the return type of the function, with `0` indicating no return type |
| param_types | `local_type*` | the parameter types of the function, with<br>`1` indicating type `i32`<br>`2` indicating type `i64`<br>`3` indicating type `f32`<br>`4` indicating type `f64` |

### Functions section
The Functions section declares the functions in the module and must be preceded by a [Signatures](#signatures-section) section. A module may contain at most one functions section.

| Field | Type | Description |
| ----- |  ----- | ----- | 
| id = `0x02` | `uint8` | section identifier for functions |
| count | `varuint32` | count of function entries to follow | 
| entries | `function_entry*` | repeated function entries as described below |

#### Function Entry

Each function entry describes a function that can be optionally named, imported and/or exported. Non-imported functions
must contain a function body.

| Field | Type |  Present?  | Description |
| ----- |  ----- |  ----- |  ----- | 
| flags | `uint8` | always | flags indicating attributes of a function <br>bit `0` : a name is present<br>bit `1` : the function is an import<br>bit `2` : the function has local variables<br>bit `3` : the function is an export |
| signature | `uint16` | always | index into the Signature section |
| name | `uint32` | `flags[0] == 1` | name of the function as an offset within the module |
| i32 count | `uint16` | `flags[2] == 1` | number of i32 local variables |
| i64 count | `uint16` | `flags[2] == 1` | number of i64 local variables |
| f32 count | `uint16` | `flags[2] == 1` | number of f32 local variables |
| f64 count | `uint16` | `flags[2] == 1` | number of f64 local variables |
| body size | `uint16` | `flags[0] == 0` | size of function body to follow, in bytes |
| body | `bytes` | `flags[0] == 0` | function body |

### Data Segments section
The data segemnts section declares the initialized data that should be loaded into the linear memory.
A module may only contain one data segments section.

| Field | Type | Description |
| ----- |  ----- | ----- | 
| id = `0x04` | `uint8` | section identifier for data segments |
| count | `varuint32` | count of data segments to follow | 
| entries | `data_segment*` | repeated data segments as described below |

* ```varuint32```: The number of data segments in the section.
* For each data segment:
  - ```uint32```: The base address of the data segment in memory.
  - ```uint32```: The offset of the data segment's data in the file.
  - ```uint32```: The size of the data segment (in bytes)
  - ```uint8```: ```1``` if the segment's data should be automatically loaded into memory at module load time.

### Indirect Function Table section
The indirect function table section declares the size and entries of the indirect function table, which consist
of indexes into the [Functions](#functions-section) section.
This section must be preceded by a [Functions](#functions-section) section.

| Field | Type | Description |
| ----- |  ----- | ----- | 
| id = `0x05` | `uint8` | section identifier for indirect function table |
| count | `varuint32` | count of entries to follow | 
| entries | `uint16*` | repeated indexes into the function table |

### End section
This indicates the end of the sections. Additional data that follows this section marker is not interpreted
by the decoder. It can used, for example, to store function names or data segment bodies.

| Field | Type | Description |
| ----- |  ----- | ----- | 
| id = `0x06` | `uint8` | section identifier for end of sections |


### Nonstandard: Globals section
A module may only contain one globals section. This section is currently for V8 internal use.

| Field | Type | Description |
| ----- |  ----- | ----- |
| id = `0x03` | `uint8` | section identifier for globals |
| count | `varuint32` | count of global variable entries to follow | 
| entries | `global_variable*` | repeated global variable entries as described below |

#### Global Variable entry

A global variable entry describes a variable outside the WASM linear memory. A Global variable can only be loaded and
stored to from dedicated instructions.

| Field | Type | Description |
| ----- |  ----- | ----- | 
| name | `uint32` | name of the global variable as an offset to a string in the module | 
| type | `uint8` | the type of the global, as a memory type |
| exported | `uint8` | a boolean indicating whether the global variable is exported |

### WorkInProgress: WLL section

| Field | Type | Description |
| ----- |  ----- | ----- |
| id = `0x11` | `uint8` | section identifier for globals |
| size | `varuint32` | size of this section in bytes | 
| body | `bytes` | contents of this section |
