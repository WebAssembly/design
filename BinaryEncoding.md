# Binary Encoding

This document describes the binary encoding of the AST nodes defined in the [AST semantics](AstSemantics.md). 
For the context of this document, see the [Binary format](MVP.md#binary-format) section of the [MVP doc](MVP.md).

The binary encoding is designed to allow fast startup, which includes reducing download size
and allow for quick decoding. Considering the matter of reducing download size, we can see
it as having three layers:

 * The **raw** binary encoding itself, natively decoded by the browser, and to be standardized in
   the MVP of the spec.
 * **Specific** compression to the binary encoding, that is unreasonable to expect a generic
   compression algorithm like gzip to achieve.
   * This is not meant to be standardized, at least not initially, as it can be done with a
     downloaded decompressor that runs as web content on the client, and in particular
     can be implemented in a polyfill. Not standardizing it leaves the binary
     encoding as the only thing a WebAssembly implementation is required to implement,
     which is nice. However, if the benefits are shown to be substantial, this will be
     reconsidered in the future.
   * We can do better than generic compression because we are aware of the AST structure
     and other details:
     * For example, macro compression that [deduplicates AST trees](https://github.com/WebAssembly/spec/issues/58#issuecomment-101863032)
       can focus on AST nodes + their children, thus having `O(nodes)` entities to worry about, compared
       to generic compression which in principle would need to look at `O(bytes*bytes)` entities.
       Such macros would allow the logical equivalent of `#define ADD1(x) (x+1)`, i.e.,
       to be parametrized. Simpler macros (`#define ADDX1 (x+1)`) can implement useful
       features like constant pools.
     * Another example is reordering of functions and some internal nodes, which we know
       does not change semantics, but
       [can improve general compression](http://www.rfk.id.au/blog/entry/cromulate-improve-compressibility/).
 * **Generic** compression, such as gzip, already supported in browsers, or LZMA and other
   compression algorithms, which might be standardized as well.

Each of the three layers work to find compression opportunities to the best of its abilities, without encroaching upon the subsequent layer's compression opportunities.

## Building blocks

### Variable-length integers
 * 31% size reduction before compression, 7% size reduction after compression.
 * [LEB128](http://en.wikipedia.org/wiki/LEB128) except limited to uint32_t payloads.

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
  * a table containing, for each function, it's signature, offset (within the section), sorted by offset, followed by
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

## Backwards Compatibility

As explained above, for size- and decode-efficiency, the binary format will serialize AST nodes,
their contents and children using dense integer indices and without any kind of embedded metadata 
or tagging. This raises the question of how to reconcile the efficient encoding with the 
backwards-compatibility goals.

Specifically, we'd like to avoid the situation where a future version of WebAssembly has features 
F1 and F2 and vendor V1 implements F1, assigning the next logical opcode indices to F1's new
opcodes, and V2 implements F2, assigning the same next logical opcode indices to F2's new opcodes 
and now a single binary has ambiguous semantics if it tries to use either F1 or F2. This type of 
non-linear feature addition is commonplace in JS and Web APIs and is guarded against by 
having unique names for unique features (and associated [conventions](https://hsivonen.fi/vendor-prefixes)).

The current proposal is to maintain both the efficiency of indices in the [serialized AST](BinaryEncoding.md#serialized-ast) and the established
conflict-avoidance practices surrounding string names:
  * The WebAssembly spec doesn't define any global index spaces
    * So, as a general rule, no magic numbers in the spec (other than the literal [magic number](http://en.wikipedia.org/wiki/Magic_number_%28programming%29)).
  * Instead, a module defines its *own* local index spaces of opcodes by providing tables *of names*. 
    * So what the spec *would* define is a set of names and their associated semantics.
  * If the implementation encounters a name it doesn't implement, by default an error is thrown while loading.
    * However, a name *may* include a corresponding polyfill function (identified by index 
      into the function array) to be called if the name isn't natively implemented. (There are a lot
      more details to figure out here.)
  * To avoid (over time) large index-space declaration sections that are largely the same
    between modules, finalized versions of standards would define named baseline index spaces
    that modules could optionally use as a starting point to further refine.
    * For example, to use all of [the MVP](MVP.md) plus [SIMD](EssentialPostMVPFeatures.md#fixed-width-simd)
      the declaration could be "base" followed by the list of SIMD opcodes used.
    * This feature would also be most useful for people handwriting the [text format](MVP.md#text-format).
    * However, such a version declaration does not establish a global "version" for the module
      or affect anything outside of the initialization of the index spaces; decoders would
      remain versionless and simply add cases for new *names* (as with current JS parsers).

