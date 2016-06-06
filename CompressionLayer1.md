A Framework For Structural Compression On Web Assembly Binary Files
===================================================================

# Introduction

This document is a design document describing a formal framework for _Layer 1_
structural compression tools for
[binary encoded WASM modules](BinaryEncoding.md).  The concept of
compression (and its inverse decompression) can be thought of as writing a
filter that converts one bitstream to another.  The framework proposed here is
based on the structure of WASM modules, and provides a way of including filter
methods into the module. These filter methods describe how to decompress the
file.

This framework does this is by introducing new filter sections into the binary
WASM module. Each filter section defines a set of methods, via s-expressions,
that define how to decompress WASM modules.

Typically, only a single filter section is introduced as the first section in
the WASM module, to introduce the methods to decompress the WASM
module. However, a WASM module may include several filter sections. The filter
methods defined in a filter section apply until the next filter section is
reached. Succeeding filter sections allows one to update the filter
methods. These updated filter methods are then be applied until the next filter
section.

It should be noted here that we are not describing how to build compression
filters, and the corresponding filter sections. That is left to individual
vendors to supply. By encoding the filter within the binary WASM module,
decompression works independently of how the WASM module was compressed.

# Background

Structural compression comes in many forms. The simplest is to recognize that
WASM modules byte align all of its data. Byte alignment, in general, isn't
minimal.  For example, if a field contains a boolean value, it can be
represented using 1 bit rather than a byte. Storing the field as a bit would
reduce the space requirements (for that field) by 7/8. Hence, using smaller
fixed-width storage can decrease file size.

Beside fixed-length compressions, there are also many forms of
[variable-length encodings](https://en.wikipedia.org/wiki/Variable-length_code)
of data that can compress data further. An example is form is
[Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding). More
specifically, integers are frequently encoded using a
[variable-length quantity](https://en.wikipedia.org/wiki/Variable-length_quantity).
These Compressions typically use
[binary octects](https://en.wikipedia.org/wiki/Binary_code) that encode
arbitrary large integers as a sequence of eight-bit bytes.

[DWARF](https://en.wikipedia.org/wiki/DWARF) files introduces the notions of
[LEB128](https://en.wikipedia.org/wiki/LEB128) for unsigned integers, and
[Signed LEB128](https://en.wikipedia.org/wiki/LEB128) for signed integers. Both
are forms of variable-length quality. Encoding is done by chunking the number
into a sequence of 7-bit values, sorted from least significant bits to most
significant bits. Then, write out each 7-bit value as a byte, by setting the
most significant bit on each 7-bit value, except the last.

Note that the most significant bit can be used to determine if more data is
needed, and allows one to encode the number back together.

Similarly, Signed LEB128 is a generalization of LEB128. Non-negative numbers
encode well using LEB128. On the other hand, negative numbers do not, since the
most significant bit of a negative number is always 1. This results is all bits
of the number being written out in the encoding, resulting in a larger sequence
of bits than you started with.

Therefore, the signed LEB128 sign extend negative numbers to the smallest number
of 7-bit chunks where the most significant bit of the most significant chunk
is 1. The resulting sequence of chunks is then written out using the same format
as LEB128.

[LLVM bitcode files](http://llvm.org/docs/BitCodeFormat.html) use a variant on
the LEB128 encoding, called variable bit rate that allows the chunk size to be
any size. not just 7. They use the notation vbr(N) to denote encodings that use
chunks of size N-1, generating sequences of N bits. The advantage of this scheme
is that numbers can frequently be compressed better than LEB128. The
disadvantage is that the written data is no longer byte aligned.

Similarly, LLVM bitcode files use a different technique to encode signed
integers. It first encodes the number as an absolute value, and then left shifts
it by one, and then sets the least significant bit to 1 if negative.

Another place for compression is to realize that code sections of WASM modules
are basically a list of instructions. These instructions are encoded using byte
opcodes. These opcodes are used to dispatch the reader to read the remaining
bytes of the instruction, based on the opcode value. This framework introduces a
select instruction to perform this task.

It should be noted that using single byte width opcodes may not be the best
choice. First, while some opcodes may require all 8 bits, there may be less that
256 different opcode values. In such cases the opcode can be compressed into a
smaller fixed size.

Although the current WASM instruction only uses one-byte opcodes, future
extensions may require the use of multiple byte opcodes. This framework allows
for this possibility by allowing select instructions to be nested.

Another common form of structural compression is to realize that many
instructions (or even sequences of instructions) have common fields. In such
cases, one can create a new opcode that automatically fills in the common
components. This removes the automatically filled in portions from having to be
explicitly encoded as data.

Note that these new opcodes do not extend the WASM code section opcodes. Those
opcodes are predefined, and built into the decoder of a WASM module. Rather,
they are opcodes only added for decompression. Further, what new opcodes are
introduced may be (dynamically) dependent on the data for the specific WASM
module being decompressed.

The minimally viable framework introduced by this document allows for the types
of compression discussed above. Other compressions are possible, but are not
proposed here. Rather, we assume that this framework can (and should) be
extended with corresponding compelling examples.

It should also be noted that the framework proposed here are generalizations of
the compression techniques used in
[LLVM bitcode files](http://llvm.org/docs/BitCodeFormat.html) . Experience in
[PNaCl bitcode files](http://www.gonacl.com), and its compression tool
pnacl-compress, has shown that compression rates of at least 40% is achievable
(from that of byte-aligning all data). Further, these compressed files are based
on structural compressions, and can be further compressed using common
byte-level compression tools (such as gzip) by an additional 10 to 20 percent.

# Motivation

This section is intended to motivate ways to compress WASM modules, and the
corresponding filter algorithms that will allow the decompression tool to
decompress the generated compressed file.

The following subsections try to introduce the concepts in a tutorial format.

## Compression is applied to sections

A decompressed WASM module, at the top level, introduces a header, followed by a
simple framework to define a sequence of sections.  Each section header contains
a value that defines the length of the section, defining where the section
ends. The next section immediately follows the previous section.  No more
sections are in the module once the end of file is reached.

Compression, within this framework, is only defined on sections. Conceptually, a
section is processed as a bitstream that only contains the bits of that
section. Therefore the algorithm need not know what appears before or after the
section.

Decompression algorithms are associated with the type of section they apply
to. The association is made using a map from the section ID, to the
corresponding algorithm to apply. If that map doesn't define a corresponding
decompression algorithm, no decompression is applied to that section. Note: to
simplify readability, decompression algorithms will simply be called algorithms.

Algorithms to decode sections are explicitly placed inside the compressed file
as 'filter' sections. Filter sections can appear anywhere within the WASM
module.

Typically, there is only one filter section, and occurs as the first section in
the compressed WASM module. However, multiple filter sections are
allowed. Succeeding filter sections can replace algorithms already defined. When
filter sections appear in a compressed WASM module, the algorithms defined by
that filter section applies to all sections between it and the next filter
section (or end of module if no next filter sections exists).

A filter section only needs to define algorithms for the sections that have been
compressed. All other sections are assumed to not be compressed and the
decompression tool will automatically copy them.

An algorithm is modeled as s-expressions. These s-expressions have operators
that correspond to a programming language that describes how to decompress a
compressed WASM module. In general, we will not discuss the details of these
s-expressions in this motivation section. They will be deferred to section
"Proposed Framework -> Translation". Rather, only when the motivation warrants
it, will we introduce specific operators (i.e. functions) that support the
motivated example.

Decompression can be thought of as a filter from the compressed bitstream
(i.e. sequence of zeros and ones) to the uncompressed bitstream (a sequence of
bytes). While it is true that decompressed WASM modules are byte based, it is
sufficient to use a bitstream semantic model, since it simplifies defining the
semantics of decompression by only having one model for input and output files.

It is important to note that the decompression tool defined here is based on
structural compression. What differentiates structural compression (WASM layer
1) and non-structural compression (WASM layer 2) is that non-structural
compression doesn't know anything about the structure of the input. Therefore
the compression can't use that knowledge to improve compression. Examples of a
non-structural compressor is zip a file.

Structural compression, on the other hand, can use structural knowledge of the
data within the file to reduce the size of the corresponding compressed file.

The following subsections of this section introduce ways structural compression
can be used to compress (and hence decompress) WASM modules.

## Structure of algorithms

A decompression algorithm consists of instruction expressions. Each
_instruction_ can be either a _definintion_, an _expression_, or a
_statement_. Definitions define methods, and associates them with names.
Statements define control-flow. Expressions represent actions that define
integers. Integers can be read, written, or read and written values. Reads are
from an input stream while writes are to the output stream.

In addition, an expression can appear anywhere a statement is allowed.

Input (and output) streams are either bits, bytes, integers, or abstract syntax
trees. All expressions (i.e. reads and writes) have different meanings,
depending on the type of stream. This is intentional. This allows the same
algorithm that reads compressed WASM modules to also write out the decompressed
form. That is, even though the bit stream may use fewer bits to represent
numbers, the same expression will generate byte-aligned encodings of integers on
byte streams.

In addition, the basic structural model used by algorithms is that WASM modules
are sequences of integers. Therefore reads to values from the corresponding
internal model, and act like the input stream is a sequence of integers. The
output is format specific, based on the type of output stream.

Decompression algorithms can directly map from an input (bit stream) to the
decompressed output (byte stream). However, they can also be a sequence of
filters. In such cases, the sequence of filters are applied in the order
specified. The output of the previous filter is then feed in as the input for
the next filter.

## Example s-expressions

Before going into motivation in more detail, let's step back for a minute and
focus on how algorithms are written in compressed WASM modules. A simple example
of an s-expression is the following:

```
(define 'type'
  (bit.to.byte
    (loop (varuint32)
           (varuint7)
     (loop (varuint32) (uint8))
     (if (varuint1) (uint8) (void)))))
```

The above s-expression defines how to parse the
[type section of a WASM Module](BinaryEncoding.md#type-section). The
S-expression _define_ assigns its second argument (the bit.to.byte s-expression)
as the algorithm for the section with ID 'type'.

Before going into any detail, it is important to understand the philosophy of
algorithms. An algorithm is used to both parse and generate WASM modules. All
instructions, except control flow (i.e. statements), do both a read and a
write. Conceptually, filters read in a sequence of integers. Control flow
constructs (statements) walk the sequence of integers. The encoding
(i.e. formatting) expressions, when reached, reads the input using the specified
format, and then (immediately) writes the read value to the output.

Unless special logic is added, algorithms do nothing more than copy the input to
the output, since formatting expressions decode (i.e. read) an integer, and then
immediately uses the same format to encode the value and write it to the output
stream.

This pattern is intentional. It allows us to use the same algorithms for
multiple contexts, as we will see in later sections.
   
The _bit.to.byte_ statement defines the structure of the input and output
streams of the filter. _Bit_ defines the input stream as a sequence of bits
while _byte_ defines the output stream as a sequence of bytes. That is, the name
before the _to_ (between _bit_ and _byte_) defines the structure of the input
stream while the name after the _to_ defines the structure of the output stream.

The _loop_ statement is iterated N times. N is defined using its first argument
(an expression). The remaining arguments are a sequence of statements that
define the body of the loop. Note that in this case, the size of the loop is
read from the bitstream to decompress.

It should be noted that when the loop reads value N, it also writes it out. This
guarantees that the loop size is copied into the output file. This copying is
also true for condition tests as well. Again, the key thing to remember is that
most expressions read the value in, and then write it back out.

Expressions _varuint32_, _varuint7_, etc. read/write an encoded integer using
the encoding defined by its name. The same format is used for both reading and
writing.  Expression _void_ does not consume any memory, nor does it represent
any (defined) value.

The nested loop statement reads the list of parameter types for a function. It
is then followed by an if statement that reads (and writes) the return type if
the function signature has one.


As noted earlier, an algorithm not only describes how to parse, but also
describes how to regenerate the WASM module. This concept is built into the
framework. By default two translation functions read and write are defined by
the framework, and operate on expressions. The read function is used to parse
numbers in, and the write function is used to print them back out.

Algorithms are written with respect to decompression. Hence parsing is reading
from the compressed file while printing is writing to the corresponding
decompressed file.

## Compression via the Map s-expression


One way you can compress a section is to use a different integer encoding when
writing than when reading. You may do this because the read encoding uses less
bits than the write encoding. The _map_ expression handles this case. A _map_
has two arguments. The first is how to read a value while the second is how to
write a value.

In the previous example, the return count (parsed using varuint1) is only a zero
or a 1. Hence we can use the map instruction to take advantage of this as
follows:

```
(define 'type'
  (bit.to.byte
    (loop (varuint32)
      (varuint7)
      (loop (varuint32) (uint8))
      (if (map (fixed 1) (varuint1)) (uint8) (void))))
```

Note: The _fixed_ expression reads and writes integers using the number of bits
defined by its argument. In this case we have used 1 bit since zero and 1 can be
written as a single bit.

## Taking advantage of stream types

In the previous section, we showed how one can use the _map_ expression to
change formats between the input _bit_ stream, and the output _byte_ stream.

While the example given is correct, it isn't necessary in this case. The reason
for this is that the notion of

```
(fixed 1)
```

has different meanings on a bit and a byte stream. On a _bit_ stream, the value
will be read/written using a single bit. On a _byte_ stream, where all values
must be byte aligned, the value will be read/written out using 8 bits. This is
equivalent to what format _varuint1_ would generate.

Hence, there was no need for the _map_ expression in this context. Rather, we
can just replace the conditional check as follows:

```
(define 'type'
  (bit.to.byte
    (loop (varuint32)
      (varuint7)
      (loop (varuint32) (uint8))
      (if (fixed 1) (uint8) (void))))
```

## Value injection


If a field is always a constant, one need not add it to the compressed file.
Rather, you can write-inject the value using a _write_ expression.

For example, consider the form field of the previously algorithm for processing
type sections. Currently, the form always has the value 0x40, and is encoded
using varuint7. Rather than saving it in the compressed file, we could have
replaced:

```
(varuint7)
```

with

```
(write 0x40 (varuint7))
```

This expression states to use the constant 0x40 (i.e. read no bits). Then write
it using encoding varuint7. Note that the _write_ expression is a shorthand for
a _map_ expression. That is,

```
(write N E) == (map (i32.const N) E)
```

## Multiple filters

It is important to realize that the input and output of a decompressor are just
encoded versions of a sequence of integers. If you work at the level of the
encoding, it really limits the structural advantage one can use to
compress. Many compression techniques can be improved by using an intermediate
data structure, thereby separating decodings of integers from the implied
structure of WASM modules.

That is, one need not implement a compression filter as a single mapping from an
input bit stream to a corresponding output byte stream. Rather, it can be a
series of filters, where each filter does a particular type of compression.

The simplest form of this is to divide input into two two filters.

- filter1 : bitream -> vector<integer>
- filter2 : vector<integer> -> bitstream

The _filter_ statement takes N statemens. Each statement is a filter, and are
applied sequentially as written, piping the output of the previous statement to
the input of the next statement.

To see this more clearly, reconsider the filter for a type section. Using the
_filter_ statement, one can separate decoding the input from encoding the output
as follows:

```
(define 'type'
  (filter
    (bit.to.int
      (loop (varuint32)
        (varuint7)
        (loop (varuint32) (uint8))
        (if (varuint1) (uint8) (void))))
    (int.to.byte
      (loop (varuint32)
        (varuint7)
        (loop (varuint32) (uint8))
        (if (varuint1) (uint8) (void))))))
```

The statements _bit.to.int_ and _int.to.byte_ describe how read/write
expressions should work, based on the type of stream.

The above example does not look like it buys you anything, just duplicates
code. That is because we have not modified either of the filters. We will
shortly, and when we do, it will become more obvious. The key thing to note is
that the _bit.to.int_ filter reads encoded values and builds a sequence of
integers, and the _int.to.bit_ filter walks that sequence of integers and
encodes them as appropriate. Thus, we have separated how we decode the sequence
of integers from how we encode the sequence of integers.

An _int_ stream assumes an internal stream of integer values (implemented as a
vector<integer>).  In addition, there is an ast stream that allows tree building
(see [Instructions and Asts](#instructions-and-asts)). Conceptually, the ast
stream is a stack of abstract syntax trees.

Note that other than specifying how streams are handled, nothing else had to
change. The reason is that evaluating these statements simply changes the
behaviour of how reads and writes work, when their argument is evaluated. The
behavior of encoding s-expressions are:


* Reading:
  * ast.to.XXX - Pop an ast off the ast stack. Flatten the ast into a sequence
    of integers first. Then return the next integer in the flattened sequence.
  * bits.to.XXX: Decode the bits and return the integer.
  * bytes.to.XXX: Decode the bytes and return the integer.
  * int.to.XXX:  Read the next integer and return it.

* Writing:
  * XXX.to.ast - Push a value onto the ast stack.
  * XXX.to.bits: Encode the integer and write to the bit stream.
  * XXX.to.bytes:  Encode the integer and write to the byte stream.
  * XXX.to.int: Write the integer to the integer stream.

It should be noted in the previous example that the first filter reads the
compressed data, and the second filter writes it out in an uncompressed
form. This separation of the reader and the writer is significant. The (final)
write filter, for a section, will always be the same. Hence, the framework
provides a default write filter. Similarly, the read filter need not know the
complex structure implied by the semantics of WASM. When compressing, one can
simply write out all integers using a single variable-rate encoding.

For example, the decompression algorithm can be simplified to:

```
(define 'type'
  (filter
    (bits.to.int
      (loop.unbounded (vbr 6)))
    (int.to.bytes (eval 'type'))))
```

Some clarification is needed here to fully understand the above example. The
_vbr_ expression uses a variable-bit rate using 6-bit chunks (rather than 8 as
in LEB128). The _loop.unbounded_ statement is like a _loop_ statement, except no
bounds condition (i.e. expression) is used. Rather, end of the input stream
(i.e. section) is used to terminate the loop.

Finally, the _eval_ statement applies the default read/write filter for the
given section name.

## Using default algorithms

In the previous section, we used the eval statement to apply a default algorithm
for processing type sections. The decompressor intentionally provides many
default methods. The rationale is to limit the number of algorithms that need to
be included in the compressed file.

By having predefined, default algorithms, fewer bits need to be downloaded to
the client. In addition, filter algorithms (appearing within the compressed
file) may be interpreted and therefore run more slowing. Default algorithms,
since they are defined once, can be compiled and run much more efficiently.

However, the framework does provide the ability to change default
algorithms. Default algorithms are implemented using the same framework as
sections. That is, one can define a default algorithm using the define
statement. They are called in the following contexts:

* by the driver code when a section with a given ID is found.

* When the eval statement is evaluated.

Currently, the following default algorithms are defined:

* _code.default_ - Default implementation for parsing a code section of a WASM
  module.

* _code.body_- Default implementation for parsing function bodies within the
  code section of a WASM module.

* _code.inst_ Default implementation for parsing an instruction within the code
  section of a WASM module.

* _data.default_ Default implementation for parsing a data section of a WASM
  module.

* _export_ - Default implementation for parsing an export section of a WASM
  module.

* _filter.default_ - Default implementation for parsing a filter section of a
  WASM module.

* _filter.inst_ - Default implementation for parsing an instruction within the
  filter section of a WASM module.

* _function.default_ - Default implementation for parsing a function section of
  a WASM module.

* _import.default_ - Default implementation for parsing an import section of a
  WASM module.

* _memory.default_ - Default implementation for parsing a memory section of a
  WASM module.

* _name.default_ - Default implementation for parsing the name section of a WASM
  module.

* _start.default_ - Default implementation for parsing a start section of a WASM
  module.

* _table.default_ - Default implementation for parsing a table section of a WASM
  module.

* _type.default_ - Default implementation for parsing a type section of a WASM
  module.

## The power of select

This decompression framework provides a concept that implements
opcode-selection, but generalizes it to any integer (not just bytes), and allows
a default case to handle unspecified opcodes. The _select_ is defined using
s-expressions of the form:

```
  (select V D C1 ... CN)
  (case K E1 \.\.\. En)
```

where

- _V_ - is read to get the corresponding selection value.

- _Ci_ - a case s-expression.

- _D_ - The default s-expression to evaluate if no case applies.

- _K_ - A constant integer defining the key.

- _E1 \.\.\. En_ - S-expressions to evaluate if the key matches the selection
  value of the case.

For example, one can write an opcode selector for control flow operators
(appearing in the code section) as:

```
(select (uint8)
  (void)
  (case 0x06 (varuint1) (varuint32))
  (case 0x07 (varuint1) (varuint32))
  (case 0x09 (varuint1)))
```

The _select_ statement can be used more creatively as a sub-filter, converting a
sequence of integers to another sequence of integers. In particular, you can
compress a sub-sequences of integers that appear often by selecting an opcode to
model that sequence. Such filters can be an _int.to.int_ filter, converting one
sequence of integers to another.

Now consider a more general example Assume that the input sequence of integers
contains the following sequence of integers 1000 times:

```
3 2 0
```

Further, assume that the number _591_ does not occur in the input sequence of
integers, and hence can be used as an opcode. We can remove 2000 numbers from
the list of integers when compressing by replacing the sub-sequences of integers
_3, 2, 0_ with the integer _591_. The algorithm to decompress the corresponding
sequence of integers is:

```
(int.to.int
  (loop
    (select (value)
      (void)
      (case 591 (lit 3) (lit 2) (lit 0)))))
```

The _value_ expression reads/writes an integer from/to integer sequence.  The
_lit_ expression returns the integer value of its argument when reading, and
writes the value when writing. That is,

```
(lit n) == (map (i64.const N) (value))
```

Note that other frequent sequences of integers may appear on the decompressed
sequence of integers. Like above, separate unused integers (not appearing in the
input sequence) can be compressed by adding additional cases.

For example, consider that the subsequence

```
1 0 0 1
```

appears 500 times, and the number _691_ does not appear in the input sequence. You
can replace the instances of sequence _1 0 0 1_ with the number 691 (saving
having to write 1500 integers) by updating the selector to:

```
(int.to.int
  (loop
    (select (value)
      (void)
      (case 591 (lit 3) (lit 2) (lit 0))
      (case 691 (lit 1) (lit 0) (lit 0) (lit 1))))
```

If you don't want to find an integer that isn't used, you can always add a
quoting integer and a separate case to handle the quoted integer.

For example, consider that you want to use _791_ as a quoting integer to the above
example.  When compressing, anytime _591_, _691_, or _791_ occurs, you write the quote
number _791_ first. The corresponding algorithm can then be used:

```
(int.to.int
  (loop
    (select (peek (value))
      (value)
      (case 591 (read (value)) (lit 3) (lit 2) (lit 0))
      (case 691 (read (value)) (lit 1) (lit 0) (lit 0)(lit 1))
      (case 791 (read (value)) (value)))))
```

Note that the _peek_ expression reads the next value on the input, but doesn't
advance the input. The _read_ expression is the converse of the _write_
expression. That is, when reading, the argument is used to read the next
integer. When writing, this expression corresponds to a nop.

Also note that we use the _peek_ expression to first see if it is the quoted
character. If it is, we erase it using the _read_ expression in the _case_
statement for value _791_.

It should also be noted that when inserting _lit_ values, the values need be
consecutive. For example, consider the case that the pattern sequence

5, X, 10

where the value _X_ is any integer. Using constant _851_ as an opcode, we can
extend the above example to capture this pattern as follows:

```
(int.to.int
  (loop
    (select (peek (value))
      (value)
      (case 591 (read (value)) (lit 3) (lit 2) (lit 0))
      (case 691 (read (value)) (lit 1) (lit 0) (lit 0) (lit 1))
      (case 791 (read (value)) (value))
      (case 851 (read (value)) (lit 5) (value) (lit 10)))))
```

## Filter Methods

So far, a filter for a section filter has been defined as a single
instruction. This is sufficient to decompress the compressed file. However, it
isn't necessarily small. As the algorithm gets larger, it is more likely that
the same statements/expressions will be used multiple times.

Like many programming languages, this framework has a solution. It has the
notion of methods and method calls.

The encoding is low level, but simple. When defining a filter algorithm, the
_define_ statement can define more than one algorithm. It accepts an arbitrary
sequence of instructions. The first instruction must be a statement, and is
always used as the entry point for the filter to apply to the section. The
remaining instructions can be statements or expressions, and are considered
helper _methods_.

The arguments to the _define_ statement are numbered sequentially, starting at
zero for the first instruction (the entry method). A method is called usint the
_call_ expression and gets a single argument, the index of the method to call.

For example, earlier we showed the following filter for type section:

```
(define 'type'
  (byte.to.byte
    (loop (varuint32)
      (varuint7)
      (loop (varuint32) (uint8))
      (if (fixed 1) (uint8) (void)))))
```

We can factor this into methods entry (0), function signature (1), parameter
types (2), and return type (3) as follows:

```
(define 'type'
  (byte.to.bit
    (loop (varuint32) (call 1))
    (seq (varuint7) (call 2) (call 3))
    (loop (varuint32) (uint8))
    (if (fixed 1) (uint8) (void))))
```

## Extracting Regions

There are several places in WASM modules that define it's corresponding data as
sequence of N bytes. Most are simple (such as the name defined by the sequence
of bytes or the bytes to initialize a global variable). Some are quite complex,
such as function bodies in the code section.

The simple cases can be handled using a loop like the following:

```
(loop (varuint32) (uint8))
```

Function bodies, on the other hand, are complex because the size is parsed
first, and then the sequence of bytes are parsed to construct the corresponding
instructions (or asts) in the function bodies. The problem is that in order to
parse the bytes, we need to convert the notion of the _end of function body_ to
a position within the input stream.

To facilitate this, the following statement is available:

```
(extract (varuint32) S)
```

The first argument defines the number of elements in the input stream (from the
point immediately following the read of the first argument) that should be
treated as the _end of input_ marker when evaluating statement S. Once S has
been evaluated, the special _end of input_ marker is removed, and is restored to
its old value.  Extract expressions can be nested.

Note that the first argument, defining the number of elements, is based on the
type of input stream. This is done since the size comes from the input stream,
and that size doesn't (typically) correspond to the same position on the output
stream. Rather, a nested copy of the input is generated. The nested input then
parsed using an (initially empty) nested output. Once parsing completes, the
output has been generated, a corresponding size measure (based on the nested
output element size) is inserted into the original output stream. Then, the
nested output stream is appended to the original output stream.

Note that the discussion of creating a nested input/output stream may not match
the implementation. The implementation is free to generate in place by saving a
fixed-size LEB128 (5 byte) value, and then back-substitute the actual count
after the extract construct is processed. For small numbers, this may require
adding zero-valued continuation chunks to the backpatched size.

## Instructions and Asts

nstructions and Asts

Most of the code sections of a WASM module is a sequence of opcode based
instructions (defining a function body).  These instructions are then converted
to s-expressions denoting an
[abstract syntax tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree)
(denoted AST). Compressing at the AST level can be very powerful, and can lead
to substantial compression.

This section looks at how the construction of instructions and ASTs are handled
in algorithms. In general, instructions are built using a preorder traversal of
the instruction. Larger expressions are typically built using a postorder
traversal of the nodes in the AST.

To do this, this framework defines an _ast_ stream specifier that treats the
input/output stream of a filter as a stack of values. For an ast stream, values
on the stack can be integers, symbols, and nodes. To make it possible to flatten
an AST back to a sequence of integers, each node records the order the kids in
the node should be walked when flattening. There are two forms: _preorder_ and
_postorder_.

For example, consider building an algorithm for modeling WASM instructions in
the code section of a WASM module.  For this example, we will only look at
modeling _i32.const_, _32.add_, and _i32.mul_ instruction.  The _select_
statement to build corresponding asts can be defined as follows:

```
(loop.unbounded
  (select (value)
    (eval 'code.inst')
    (case 0x10 (value) (preorder 2))     // i32.const
    (case 0x40 (postorder 3))            // i32.add
    (case 0x42 (postorder 3))))          // i32.mul
```

The _preorder_ expression pops off N elements (based on its only argument),
creating a node with the corresponding list of N elements, in the order they
appeared on the stack. The node is then pushed back onto the stack. This
corresponds to reading the values from the stack in preorder (i.e. the lowest
element in the stack is the instruction ID, and the remainder are its
arguments).

Similarly the _postorder_ instruction pops off N elements (based on its only
argument), creating a node with the first value popped of the stack, followed by
the last N-1 elements popped (in the order they appeared on the stack). The node
is then pushed back onto the stack. This corresponds to reading the values from
the stack in postorder (i.e. the instruction ID is the last value, and the kids
appear before the name).


The _eval_ statement uses the (predefined) default implementation to read WASM
code instructions, and build the corresponding tree (when the output is an _ast_
stream). It is provided so that one only need to specify extensions into the
compressed file. However, for this example, we have explicitly overwritten the
_i32.const_, _i32.add_, and _i32.mul_ cases to provide examples of how to build
ASTs in algorithms.

For example, consider the following s-expression:

```
(i32.add (i32.mul (i32.const 1) (i32.const 2)) (i32.const 3))
```

Further, assume that the corresponding intput _int_ stream for the filter algorithm is:

```
Input: 0x10 1 0x10 2 0x42 0x10 3 0x40
Output:
```

Running the above select instruction on this input, case _0x10_ will read
(i.e. push) two integers, updating as follows:

```
input: 0x10 2 0x42 0x10 3 0x40
output: 0x10 1
```

At this point, the _preorder_ expression (of case _0x10_) pops off two elements,
generating the corresponding preorder node (denoted with <> brackets) and
pushes it back onto the stack. The updated streams are:

```
input: 0x10 2 0x42 0x10 3 0x40
output: <0x10 1>
```

Similarly, after the next iteration of the loop the updates the streams as
follows:

```
input: 0x42 0x10 3 0x40
output: <0x10 1> <0x10 2>
```

At this point _case_ statement _0x42_ starts by pushing _0x42_ onto the output
stack.  The _postorder_ expression (of case _0x42_) pops off three elements,
generating the corresponding postorder node (denoted with [] brackets) and
pushes it back onto the stack. The updated streams are:

```
input: 0x10 3 0x40
output: [0x42 <0x10 1> <0x10 2>]
```

After running _case_ statement _0x10_, the streams are updated as follows:

```
input: 0x40
output: (0x42 <0x10 1> <0x10 2>) <0x10 3>
```

Finally, _case_ statement _0x42_ is processed, resulting int the following:

```
input:
output: [0x40 [0x42 <0x10 1> <0x10 2>] <0x10 3>]
```

## Adding Tree Patterns

Tree patterns, within algorithms, take an AST opcode, over N AST arguments, and
replaces that AST with a new AST that uses the N ast arguments.

For example, consider the address calculation of an element within an array. The
AST for this type of address calculation could have the following form:

```
(+ (+ (* C N) M) A)
```

- _A_ is a memory address to an array.
- _C_ is the number of bytes used by each element in the array.
- _N_ is the element within the array being accessed.
- _M_ is the offset to a field within the record stored at that address.

Conceptually, we could create a new meta operator _index_ to capture this as the
instruction:

```
(index C N M A)
```

Using this form as a new opcode, it removes the need to save the times (*) and
two add (+) instrucitons into the compressed file, by replacing them by the new
index instruction.

The problem with tree patterns is that many opcodes do not appear in the input
stream until after the arguments have been pushed onto the stack. Hence, there
is no place to insert the times (*) and the inner add (+) instrucitons into the
AST while reading. This is fixed by changing the output stream from a stack of
values, to a pair of stack of values.

One stack is the _main_ stack. The _main_ stack is the same as described in the
previous section. The other is the _stash_ stack. The _stash_ expression pops
elements off the _main_ stack and pushes them onto the _stash_ stack. The
_unstash_ expression pops elements off the _stash_ stack and pushes them onto the
_main_ stack.

Returning the the index instruction example above, the _stash_ expression lets
us pop off the trees _M_ and _A_ from the main stack. This allows us to insert a
times (\*) instruction over _C_ and _N_ on the top of the _main_ stack. The
_unstash_ expression can then be used to move M back onto the main stack,
allowing us to insert an _add_ (\+). Finally, another _unstash_ expresssion can
be used to move _A_ back onto the main stack so that we can add the top-level
times (*).

More specifically, the following select loop can be used to process simple
address calculations:

```
(loop.unbounded
  (select (peek(value))
    (eval 'code.inst')
    (case 257                        // meta index opcode
      (read (value)) (stash 2) (lit 0x42) (postfix 3)
      (unstash 1) (lit 0x40) (postfix 3)
      (unstash 1) (lit 0x40) (postfix 3))))
```

On need not specify how to flatten the constructed asts back into a sequence of
integers, so that the (default) algorithm can be used to generate the resulting
decompressed WASM module. Rather, the implementation of an _ast_ stream, when
used as an input stream, will automatically flatten the asts as they are found.

## Ast to Ast Filters

Currently, our notation allows ast.to.ast filters. However, the framework
(currently) doesn't allow this form of filter. The rationale is that we have not
yet seen examples that demonstrate the value of such filters.

Should motivating examples for ast.to.ast filters be found, extending this
framework to handle that case will be added at that time.

## Other potential compressions

This section notes that this document doesn't cover some interesting cases of
compression that probably should be added at some point. In particular, there
are two (obvious) additional types of compression that have not been considered
so far.

The first is that some *level 2* compression algorithms use a notion of _sliding
windows_, where the range of that window is dependent on the corresponding
compression algorithm. The
[Brotli](https://datatracker.ietf.org/doc/draft-alakuijala-brotli/) is such an
example.

In such cases, you may want to reorder the input sequence of integers to put
similar things together to improve the performance of the level 2 compression.

The second (obvious) case is to consider not downloading an updated
module. Rather, only send changes between the updated module, and the previously
downloaded module. That is, send a _patch_ instead of a complete copy of the
updated module.

A patch can easily be defined as a sequence of edits (i.e. remove, insert, or
move) on sequences of integers, using the sequence of integers represented by
the previously downloaded module.

Note that these edit moves, if defined, make both kinds of compression
feasable. It is believed that editting statements should be added to this
framework, but is not included in this document.

It is assumed that extensions to handle these cases (and the corresponding
edits) will be added in a future version of this document.

## Streaming

In general, one desirable property of filters is that they be able to stream
both the input and the output. When this is possible, simple buffers can be used
to limit the amount of information stored in local memory.

For bit, byte, and int streams, this is possible because no read or write can
access more than a fixed amount. Hence, using a buffering strategy in the
decompressor is relatively easy.

For ast streams, one can have arbitrarily long unstash operations. Therefore, by
default, one can't stream _ast_ streams. The solution to this problem is the
_flush_ statement. When evaluated, all values on the (main) output stack can be
passed to the next filter. Note that it is an error to try and
push/pop/stash/unstash values that were flushed.

# Proposed Framework

The following subsections defines the framework being proposed.

Before going into the details, it is important to get an overall understanding
of what is being presented. Filter sections are a sequence of
s-expressions. These s-expressions define how to filter a bitstream. The first
element of each s-expression is a symbol. The symbol defines what algorithm to
apply to its other arguments.

Therefore, filter algorithms are nothing more than s-expressions. To keep track
of filter algorithms, state is introduced. We call this state the
environment. All algorithms operate on the environment, and update as
appropriate.

Part of the environment is a mapping from section names (or names in general),
to a corresponding list of filter algorithms (s-expression) to apply. When
filtering a section in a WASM module, the first filter algorithm in the list
corresponding filter algorithms (i.e. stored s-expression via define) is used.

If no s-expression is defined for a section, the default section algorithm is
used. The default section algorithm has the same map for section names. However,
it appends ".default" to the section name and then looks up on that name.

The input and output streams are initially a sequence of zeros and ones (i.e. a
bitstream). If subfilters are defined, these input/output streams can be
replaced by either:

1. A sequence of integers.
2. Two stacks of integers, instructions, and ast nodes. One stack is the _main_
  stack and can be read and/or written to. The other stack is only available to
  the write stream, and is called the _stash_ stack. The stash stack allows tree
  editing of the asts being built.

S-expressions simultaneously define three things:

1. Control flow
2. How to read the input.
3. How to write the output.

This allows the same code to be used to both read an input stream, and
simultaneously write to the output stream. This is intentional in that it
reduces the amount of code within filter algorithms.

The following subsections introduce a formalization of the set of legal
s-expressions, and the semantics behind these s-expressions.

## Types

This section introduces the type model used by this filter framework. It defines
the set of all possible types (integers, bitstreams, integer sequences, vectors,
and stacks). In general, we assume that values on stacks are runtime-typed, so
that the structure of the ASTs are explicit. Other values can be run-time typed,
but it is not necessary.

The following are primitive types used when evaluating filter algorithms. For
simplicity, we assume that all integers (and floating point values) can be
represented as an unsigned 64-bit value. Hence, integers, when internal will be
stored in that form. The other forms of integer are only used when encoding
to/decoding from bitstream.

Primitive types:

- void - a single, zer0-bit undefined value.
- u1 - a bit in the set {0, 1}.
- u8 - an 8-bit unsigned integer.
- u32 - a 32-bit unsigned integer.
- u64 - a 64-bit unsigned integer.
- i32 - a 32-bit signed integer.
- i64 - a 64-bit signed integer.

Structured types:

- int = u1 | u8 | u32 | u64 | i32 | i64
- astValue = int | symbol | node | void
- methods = vector<value>
- opcodeMap = symbol -> int
- stack = vector<value> 
- node = postfix:u1 x values:vector<value>
- stream = type:symbol x stream:streamValue
- streamOf<bit> = vector<u1>
- streamOff<int> = vector<int>
- streamOff<ast> = main:stack x stash:stack
- streamValue = streamOf<bit> | streamOf<int> | streamOf<ast>
- symbol = vector<u8>
- symMap = symbol -> methods
- value = type:symbol x value: astValue

Values are (runtime) typed with a symbol. The legal symbols are:

- 'int' - Value is an integer in type int
- 'sym' - Value is a symbol
- 'node' - Value is an node
- 'void' - Value is the void value

In general, we will not include conversions between instances of type value and
type ast_value. Rather, we assume appropriate conversions are implicitly added,
if needed.

Streams are similarly typed with a symbol that identifies the type of the
stream. Legal symbols are:

- 'ast' - The stream is of type streamOf<ast>. Note: Currently, one of the
  streams (either input or output) must not be an ast stream if the other stream
  is an ast stream. When an ast stream appears as input, it will automatically
  be converted to a sequence of integers by the formatting instructions.
- 'bit' - The stream is of type streamOf<bit> for which the the extract size is
  in bits.
- 'byte' - The stream is of type streamOf<bit> for which the extraction size is
  in bytes.
- 'int' - The stream is of type streamOf<int>

Like values, we will not include conversions between instances of type stream
and type streamVvalue. Rather, we assume appropriate conversions are implicitly
added, if needed.  Values are pair consisting (X, Y) where Y is the untyped
value, and X is the name of the corresponding untyped value. Values are denoted
as the pair _Y:X_.

For readability the value _undefined:void_ will simply be denoted as
_void_.

An node is prefixed with a bit that defines if the tree should be flattened
using a preorder (0) or a postorder (1) walk. Notationally, we will use the
following:

- preorder(Values) = node(0, Values)
- postorder(Values) = node(1, Values)

The opcode_map is a mapping from filter opcode names, to the corresponding
opcode value. It is used to verify that the correct opcode is used for an
instruction name.

S-expressions are just (ast) values. For readability, _Exp_ will be used for
expressions and _Stmt_ for statements.

## Translation Functions

This section defines the set of translation functions that define the semantics
of the decompressor. The implementation of these functions are described using
denotational semantics. This section focuses on the signature of translation
functions, and their intended goal. Succeeding sections define the semantics for
individual s-expressions.

```
Parse: stream -> stack -> opcode_map -> (stream x stack)
```

Function _Parse_ converts the bits in a filter section to its corresponding
s-expressions. Once the s-expressions have been decoded, the generated list of
s-expressions are processed to install the corresponding filters.

```
state = in:stream x out:stream x def:sym_map x methods:methods
```

Type _state_ defines the environment in which s-expressions are evaluated. It
contains an input stream to parse, an output stream to generate, a set of symbol
definitions (for eval s-expressions), and a set of available methods (for call
s-expressions).

Function _Run_ takes an s-expression (i.e. either a statement or an expression)
and a state, and describes how the state is updated as the s-expression if
processed.

```
Run: value -> state -> state
```

For all expressions, the definition of _Run_ is:

```
Run [[ E ]] State =
V, State.In = Read [[ E ]] State.In;
State.Out = Write [[ E ]] V State.Out;
```

Function _Read_ takes an expression and an input stream, and returns both the
value read and the updated input stream. Note: Reading (and writing) are modeled
using a side-effect free model to simplify the semantics.

```
Read: Exp -> stream -> (value x stream)
```

If _Read_ is not defined explicitly for an expression, the following is assumed:

```
Read [[ E ]] In = error
```

Function _Write_ takes an expression and an output stream, and returns an
updated output stream where the value as added to the end of the output stream.

```
Write: Exp -> value -> stream -> stream
```

If _Write_ is not defined explicitly for an expression, the following is assumed:

```
Write [[ E ]] V Out = error
```

Function _Matches_ tests if a _case_ statement matches the corresponding select
key of the select it appears in.

```
Matches: Stmt -> u1
```

If _Matches_ is not defined explicitly for a statement, the following is assumed:

```
Matches [[ S ]] = error()
```

Function _OutStream_ generates an empty output stream, as defined by the given
statement.

```
OutStream: Stmt -> stream
```

If _OutStream_ is not defined explicitly for a statement, the following is assumed:

```
OutStream [[ S ]] = error
```

## Expressions

This section defines the set of expressions that can appear in filter secctions.

### Constants

Constant instructions define constants that can be used as an argument to other
instructions. The do not consume input when reading, and do not generate any
output when writing.

#### (void)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op.value == Opcode['void']
  Values = vector<value>()
  Values.append('void')
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (void) ]] In = void, In
```

```
Write [[ (void) ]] V Out = Out
```


#### (i32.const N)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['i32.const']
  Values = vector<value>()
  Values.append('i32.const')
  N, In = readVarint32(In)
  Values.append(N)
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (i32.const N) ]] In = N, In
```

```
Write [[ (i32.const N) ]] V Out = Out
```

#### (u32.const N)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['u32.const']
  Values = vector<value>()
  Values.append('u32.const')
  N, In = readVaruint32(In)
  Values.append(N)
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (u32.const N) ]] In = N, In
```

```
Write [[ (u32.const N) ]] V Out = Out
```

#### (i64.const N)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  Assert Op == Opcode['i64.const']
  Values = vector<value>()
  Values.append('i64.const')
  N, In = readVarint64(In)
  Values.append(N)
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (i64.const N) ]] In = N, In
```

```
Write [[ (i64.const N) ]] V Out = Out
```

#### (u64.const N)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['u64.const']
  Values = vector<value>();
  Values.append('u64.const')
  N, In = readVaruint64(In)
  Values.append(N)
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (u64.const N) ]] In = N, In
```

```
Write [[ (u64.const N) ]] V Out = Out
```

#### (f32.const N)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['f32.const']
  Values = vector<value>()
  Values.append('f32.const')
  N, In = readUint32(In)
  Values.append(N)
  Stack.append(preorder(values))
  return In, Stack
```

```
Read [[ (u64.const N) ]] In = N, In
```

```
Write [[ (u64.const N) ]] V Out = Out
```

### Formatting

Formatting expressions convert integers to sequences of bits when writing, and
from sequences of bits to integers when reading. Reading or writing an integer
on an int stream corresponds to reading/writing an integer from/to the
corresponding stream.

Ast streams are slightly anomalous. Writing to an ast stream always appends
(i.e. pushes) the corresponding ast value. Reading, on the other hand, is
dependent on the type of the corresponding output stream.  Reading from an ast
stream automatically flatten ast nodes into a sequence of integers, and the
formatting instructions use the next (flattened) integer.

#### (value)

The value formatting instruction is the default formatting instruction.

```
Parse In Stack Opcode:
  Op, In = readUuint8(In)
  assert Op == Opcode['value']
  Values = vector<value>()
  Values.append('value')
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (value) ]] In = Read [[ (ivbr 6) ]] In
```

```
Write [[ (value) ]] V Out = Write [[ (ivbr 6) ]] V Out
```

#### (uint8)

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode['uint8']
  Values = vector<value>()
  Values.append('uint8');
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (uint8) ]] In:
  switch In.type:
    case 'bit', 'byte':
      V, In = readUint8(In)
    case 'int':
      V, In = readInt(In)
    case 'ast':
      V, In = readAst(In)
  return V, In
```

```
Write [[ (uint8) ]] V Out:
  switch In.type:
    case 'bit', 'byte':
      Out = writeUint8(V, Out)
    case 'int':
      Out = writeInt(V, Out)
    case 'ast':
      Out = writeAst(V, Out)
  return Out
```

#### (uint32)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['uint32']
  Values = vector<value>()
  Values.append('uint32')
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (uint32) ]] In =
  switch In.type:
    case 'bit', 'byte':
      V, In = readUint32(In)
    case 'int':
      V, In = readIint(In)
    case 'ast':
      V, In = readAst(In)
      return V, In
```

```
Write [[ (uint32) ]] V Out =
  switch out.type:
    case 'bit', 'byte:
      Out = writeUint32(V, Out);
    case 'int':
      Out = writeInt(V, Out)
    case 'ast':
      Out = write_ast(V, Out)
    return Out
```
#### (uint64)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['uint64']
  Values = vector<value>()
  Values.append('uint64')
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (uint64) ]] In =
  switch In.type:
    case 'bit', 'byte':
      V, In = readUint64(In)
    case 'int':
      V, In = readInt(In)
    case 'ast':
      V. In = readAst(In)
  return V, In
```

```
Write [[ (uint64) ]] V Out =
  Switch Out.type:
    case 'bit', 'byte':
      Out = writeUint64(V, Out)
    case 'int':
      Out = writeInt(V, Out)
    case 'ast':
      Out = writeAst(V, Out)
  return Out
```

#### (varuint1)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['varuint1']
  Values = vector<value>()
  Values.append('varuint1')
  Stack.append(preorder(values))
  return In, Stack
```

```
Read [[ (varuint1) ]] In =
  switch In.type:
    case 'bit', 'byte':
      V, In = readVaruint1(In)
    case 'int':
      V, In = readInt(In)
    case 'ast':
      V, In = readAst(In)
  return V, In
```

```
Write [[ (varuint1) ]] V Out =
  switch Out.type:
    case 'bit', 'byte':
      Out = writeVaruint1(V, Out)
    case 'int':
      Out = writeInt(V, Out);
    case 'ast':
      Out = write_ast(V, Out)
  return Out
```

#### (varuint7)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['varuint7']
  Values = vector<value>()
  Values.append('varuint7')
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (varuint7) ]] In =
  switch In.type:
    case 'bit', 'byte':
      V, In = readVaruint7(In)
    case 'int':
      V, In = readInt(In)
    case 'ast':
      V, In = readAst(In)
    return V, In
```

```
Write [[ (varuint7) ]] V Out =
  switch Out.type:
    case 'bit', 'byte':
      Out = writeVaruint7(V, Out)
    case 'int':
      Out = writeInt(V, Out)
    case 'ast':
      Out = writeAst(V, Out)
  return Out
```

#### (varint32)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['varint32']
  Values = vector<value>()
  Values.append('varint32')
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (varint32) ]] In =
  switch if In.type:
    case 'bit', 'byte':
      V, In = readVarint32(In)
    case 'int':
      V, In = readInt(In)
    case 'ast':
      V, In = readAst(In)
  return V, In
```

```
Write [[ (varint32 ]] V Out =
  switch Out.type:
    case 'bit', 'byte':
      Out = writeVarint32(V, Out)
    case 'int':
      Out = writeInt(V, Out)
    case 'ast':
      Out = writeAst(V, Out)
  return Out
```

#### (varuint32)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['varuint32']
  Values = vector<value>()
  Values.append('varuint32')
  Stack.append(preorder(Values))
  return In, Stack 
```

```
Read [[ (varuint32) ]] In =
  switch In.type:
    case 'bit', ''byte':
      V, In = readVaruint32(In)
    case 'int':
      V, In = readInt(In)
    case 'ast':
      V, In readAst(In)
  return V, In
```

```
Write [[ (varuint32 ]] V Out =
  switch Out.type: 
    case 'bit', 'byte':
      Out = writeVaruint32(V, Out)
    case 'int':
      Out = writeInt(V, Out);
    case 'ast':
      Out = writeAst(V, Out)
  return Out
```

#### (varint64)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['varint64']
  Values = vector<value>()
  Values.append('varuint64')
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (varint64) ]] In =
  switch In.type:
    case 'bit' , 'byte':
      V, In = readVarint64(In)
    case 'int':
      V, In = readInt(In)
    case 'ast':
      V, In = readAst(In)
  return V, In
```

```
Write [[ (varint64 ]] V Out =
  switch Out.type:
    case in 'bit', 'byte':
      Out = writeWarint64(V, Out)
    case 'int':
      Out = writeInt(V, Out)
    case 'ast':
      Out = writeAst(V, Out)
  return Out
```

#### (varuint64)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['varuint64']
  Values = vector<value>()
  Values.append('varuint64')
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (varuint64) ]] In =
  switch In.type:
    case 'bit', 'byte':
      V, In = readVaruint64(In)
    case 'int':
      V, In = readInt(In)
    case 'ast':
      V, In = readAst(In)
  return V, In
```

```
Write [[ (varuint64 ]] V Out =
  switch Out.type:
    case 'bit', 'byte':
      Out = writeVaruint64(V, Out)
    case 'int':
      Out = writeInt(V, Out)
    case 'ast':
      Out = writeAst(V, Out)
  return Out
```

#### (fixed N)

On bit streams, prints out the integer using the given number of bits. On byte
streams, rounds up to the nearest fixed-width byte format.

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['fixed']
  Values = vector<value>()
  Values.append('fixed')
  N, In = readUuint8(In)
  assert N <= 64
  Values.append(N)
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (fixed N) ]] In =
  switch In.type:
    case 'bit':
      V, In = readFixed(N, In)
    case 'byte':
      if N <= 8:
        return Read [[ (uint8) ]] In
      if N <= 32:
        return Read [[ (uint32) ]] In
      return Read [[ (uint64) ]] In
    case 'int':
      V, In = readInt(In)
    case 'ast':
      V, In = readAst(In)
  return V, In
```

```
Write [[ (fixed N) ]] V Out =
  switch Out.type:
    case 'bit':
      Out = writeFixed(N, V, Out)
    case 'byte':
      if N <= 8:
        return Write [[ (uint8) ]] V Out
      if N <= 32:
        return Write [[ (uint32) ]] V Out
      return Write [[ (uint64) ]] V Out
    case 'int':
      Out = writeInt(V, Out)
    case 'ast':
      Out = writeAst(V, Out)
  return Out
```

#### (vbr N)

On bit streams, formats integers using LEB128, and chunk size of N (rather than
8). On byte streams, converts to LEB128 with chunk size 8.

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['vbr']
  Values = vector<value>()
  Values.append('vbr')
  N, In = readUint8(In)
  assert N <= 64
  Values.append(N)
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (vbr N) ]] In =
  switch In.type:
    case 'bit':
      V, In = readVbr(N, In)
    case 'byte':
      if N <= 32:
        return Read [[ (varuint32) ]] In
      return Read [[ (varuint64) ]] In
    case 'int':
      V, In = readInt(In)
    case 'ast':
      V, In = readAst(In)
  return V, In
```

```
Write [[ (vbr N) ]] V Out =
  switch Out.type:
    case 'bit'
      Out = writeVbr(N, V, Out)
    case 'byte':
      if N <= 32:
        return Write [[ (varuint32) ]] In
      return Write [[ (varuint64) ]] In
    case 'int':
      Out = writeInt(V, Out)
    case 'ast':
      Out = writeAst(V, Out)
  return Out
```

#### (ivbr N)

On bit streams, formats integers using Signed LEB128, and chunk size of N
(rather than 8). On byte streams, converts to Signed LEB128 with chunk size 8.

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['ivbr']
  Values = vector<value>()
  Values.append('ivbr')
  N, In = readUint8(In)
  assert N <= 64
  Values.append(N)
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (vbr N) ]] In =
  switch In.type:
    case 'bit':
      V, In = readIvbr(N, In)
    case 'byte':
      if N <= 32:
        return Read [[ (varint32) ]] In
      return Read [[ (varint64) ]] In
    case 'int':
      V, In = readInt(In)
    case 'ast':
      V, In = readAst(In)
  return V, In
```

```
Write [[ (vbr N) ]] V Out =
  switch Out.type:
    case 'bit'
      Out = writeVbr(N, V, Out)
    case 'byte':
      if N <= 32:
        return Write [[ (varint32) ]] In
      return Write [[ (varint64) ]] In
    case 'int':
      Out = writeInt(V, Out)
    case 'ast':
      Out = writeAst(V, Out)
  return Out
```

### I/O control

This section introduces the operations for controlling when subexpressions are
read and written using different formatting I/O constructs.

#### (map E1 E2)

The _map_ expression uses the first argument for reading, and the second
argument for writing.

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['map']
  Values = vector<value>()
  for i in {1, 2}:
    Values.prepend(Stack.popLast())
  Values.prepend('map')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Read [[ (map E1 E2) ]] In = Read [[ E1 ]] In 
```

```
Write [[ (map E1 E2) ]] V Out = Write [[ E2 ]] V Out
```

#### (read E)

The _read_ expresssion reads its argument when reading, and is ignored when
writing.

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['read']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('read')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Read [[ (read E) ]] In = Read [[ E ]] In
```

```
Write [[ (read E) ]] V Out = Out
```

#### (lit N)

The _lit_ expression returns _N_ as the value read. When writing, the value _N_
is written using the default (_value_) format.

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['lit']
  Values = vector<value>()
  Values.append('lit')
  N, In = readVarint64(In)
  Values.append(N)
  Stack.append(preorder(Values))
  return In, Stack
```

```
Read [[ (lit N) ]] In = N, In
```

```
Write [[ (lit N) ]] V Out:
  assert V == N
  return Write [[ (value) ]] V Out
```

#### (write N E)

The _write_ expression returns _N_ when reading. When writing, _E_ is used to
format the value _N_.

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['write']
  Values = vector<value>()
  Values.append('write')
  N, In = readVarint64(In)
  Values.append(N)
  Values.append(Stack.popLast())
  Stack.append(preorder(NodeValues))
  return In, Stack
```

```
Read [[ (write N E) ]] In = N, In
```

```
Write [[ (write N E) ]] V Out =
  assert N == V
  return Write [[ E ]] V Out
```

#### (peek E)

When reading, the _peek_ expression reads E, and then resets the input stream
back to its original position. When writing, _peek_ is ignored.

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['peek']
  Values = vector<sexp_value>()
  Values.prepend(Stack.popLast())
  Values.prepend('peek')
  Stack.append(postorder(Values))
  return In Stack
```

```
Read [[ (peek E) ]] In = 
  V, Ignore = Read [[ E ]] In
  return V, In
```

```
Write [[ (peek E) ]] V Out = Out
```

### Trees

Note: Currently, we only have ways of constructing trees, and flattening them
back out to a sequence of integers. If (or when) additional operators can be
justified, they will be added.

#### (preorder N)

Creates an ast node of _N_ values (including the root) where the values are on
the output stream stack in preorder. Note: If the output stream is not an ast
stream, this operation is a nop.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode['preorder']
  N, In = readVaruint32(In)
  assert N > 0
  Values = vector<value>()
  Values.append('preorder')
  Values.append(N)
  Stack.append(preorder(Values))
  return In, Stack
```

```
Run [[ (preorder N) ]] Env:
  If Env.out.type != 'ast':
    return Env
  Values = vector<value>()
  for i = 1; i <= N; ++i:
    Values.prepend(Env.out.stream.main.popLast())
  Env.out.stream.main.append(preorder(Values))
  return Env
```

#### (postorder N)

Creates an ast node of _N_ values (including the root) where the values are on
the output stream stack in postorder. Note: If the output stream is not an ast
stream, this operation is a nop.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode['postorder']
  N, In = readVaruint32(In)
  assert N > 0
  Values = vector<value>()
  Values.append('postorder')
  Values.append(N)
  Stack.append(prefix(Values))
  return In, Stack
```

```
Run [[ (postorder N) ]] Env:
  If  Env.out.type != 'ast':
    return Env
  Values = vector<value>()
  Root = Env.out.stream.main.popLast()
  for i = 2; i <= N; ++i:
    Values.prepend(Env.out.stream.popLast())
  Values.prepend(Root)
  Env.out.stream.main.append(postorder(Values))
  return Env
```

#### (stash N)

Pops _N_ values off the output stream's _main_ stack, and pushes them onto the
corresponding _stash_ stack. Note: If the output stream is not an ast stream,
the operation is a nop.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode['stash']
  N, In = readVaruint32(In)
  assert N > 0
  Values = vector<value>()
  Values.append('stash')
  Values.append(N)
  Stack.append(preorder(Values))
  return In, Stack
```

```
Run [[ (stash N ]] Env
  if Env.out.type != 'ast':
    return Env
  for i = 1; i <= N; ++i:
    Env.in.stream.stash.append(Env.out.stream.main.popLast())
  return Env
```

#### (unstash N)

Pops _N_ values off the output stream's _stash_ stack, and pushes them onto the
corresponding _main_ stack. Note: If the output stream is not an ast stream, this
operation is a nop.

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['unstash']
  N, In = readVaruint32(In)
  assert N > 0
  Values = vector<value>()
  Values.append('unstash')
  Values.append(N)
  Stack.append(preorder(Values))
  return In, Stack
```

```
Run [[ (unstash N) ]] Env:
  if Env.out.type != 'ast':
    return Env;
  for i = 1; i <= N; ++i:
    Env.out.stream.main.append(Env.out.stream.stash.popLast())
  return Env
```

## Statements

This section defines the set of statements that can appear in filter secctions.

### Control Flow

This section defines statement that control the order other statements are executed.

#### (seq S1 \.\.\. Sn)

The _seq_ statement evaluates each of its arguments, in the order found.

```
Parse In Stack Opcode:
  Op, In  = readUint8(In)
  assert Op == Opcode['seq']
  Values = vector<value>()
  N, In = readVaruint32(In)
  for i = 1; i <= N; ++i:
    Values.prepend(Stack.popLast())
  Values.prepend('seq')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (seq S1 \.\.\. Sn) ]] Env:
  for i = 1, i <= n; ++i:
    Env = Run [[ Si ]] Env
  return Env
```
    
#### (loop E S1 \.\.\. Sn)

The _loop_ statement reads (and writes) expression _E_ , defining the number of
iterations _M_. It then iterates _M_ times, running each of the expressions in
the order specified.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode['loop']
  Values = vector<value>()
  N, In = readVaruint32(In)
  assert N > 1;
  for i = 1; i <= N; ++i:
    Values.prepend(Stack.popLast())
  Values.prepend('loop')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (loop E S1 \.\.\. En) ]] Env:
  M, Env.In = Read [[ E ]] Env.In
  Env.Out = Write [[ E ]] M Env.Out
  assert M.type == 'int'
  for i = 1; i <= M.value; ++i:
    PrevSize = Env.in.stream.size()
    for j = 1; j < n; ++j:
      Env = Run [[ Sj ]] Env
    assert Env.in.stream.size() > PrevSize
  return Env
```

#### (loop.unbounded S1 \.\.\. Sn)

The _loop.unbounded_ statement evaluates the sequence of expressions while there
is more input to process.

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['loop.unbounded']
  Values = vector<value>()
  N, In = readVaruint32(In)
  assert N > 1;
  for i = 1; i <= N; ++i:
    Values.prepend(Stack.popLast())
  Values.prepend('loop.unbounded')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (loop.unbounded S1 \.\.\. Sn) ]] Env:
  while not In.stream.empty():
    PrevSize = Env.in.stream.size()
    for i = 1; i <= n; ++i:
      Env = Run [[ Si ]] Env
    assert Env.In.stream.size() > PrevSize
  return Env
```

#### (if E S1 S2)

The _if_ statement initially reads (and writes) _E_ to define a condition
_C_. If _C_ is zero, _S2_ is processed. Otherwise _S1_ is processed.

```
Parse In Stack Opcode:
  Op, In = readUuint8(In)
  assert Op == Opcode['if']
  Values = vector<value>()
  for i = 1; i <= 3; ++i:
    Values.prepend(Stack.popLast())
  Values.prepend('if')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (if E S1 S2) ]] Env:
  C, Env.in = Read [[ E1 ]] Env.in
  Env.out = Write [[ E1 ]] Env.out
  if C == 0 then
    return Run [[ S2 ]] Env
  else
    return Run [[ S1 ]] Env
```

#### (select V D C1 \.\.\. Cn)

The _select_ statement reads (and writes) the selector expression _V_. It then
looks for a corresponding case _Ci_ statement that matches value _V_. If found,
_Ci_ is processed and then control returns. If no _Ci_ applies, the default
statement D is processed.

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['select']
  Values = vector<value>()
  N, In = readVaruint32(In)
  for i = 1; i <= N + 2; ++i:
    Kids.prepend(Stack.popLast())
  Values.prepend('select')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (select V D C1 \.\.\. Cn) ]] Env:
   K, Env.in = Read [[ V ]] Env.in
   Env.out = Write [[ V ]] K Env.out
   for i = 1; i <= n; ++i:
   if Matches [[ Ci ]] K then
     return Run [[ Ci ]] Env
   return Run [[ D ]] Env
```

#### (case K S1 \.\.\. Sn)

The case statement labels the sequence of instructions _S1_ through _Sn_. When
inside a select, the case if followed iff the condition of the select matches
integer _K_.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode['case']
  Values = vector<value>()
  K, In = readVaruint64(In)
  N, In = readVaruint64(In)
  for i = 1; i <= N; ++i:
    Values.prepend(Stack.popLast())
  Values.prepend(K)
  Values.prepend('case')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (case K S1 \.\.\. Sn) ]] Env =
  Run [[ (seq S1 \.\.\. Sn ]] Env
```

```
Matches [[ (case K S1 \.\.\. Sn) ]] V = (V == K)
```

#### (extract S)

The _extract_ statement reads a size value _N_ from the input stream, to define
the number of (input stream) elements that define the boundary of the construct
parsed by statement _S_. It then sets the _end of input_ marker (of the input
stream) to _N_, and runs statement _S_.

For bit streams, additional bits are read from the bit stream until the input is
byte aligned. Finally, before returning, the previous _end of input_ marker is
restored.

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['extract']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('extract')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (extract S) ]] Env:
  Size, Env.in = Read [[ (varuint32) ]] Env
  assert Size > 0
  OutCont = Env.out
  Switch Env.in.type:
    case 'bit':
      Env.in, InCont = splitBitstream(Env.in, Size)
      Env.out = streamOf<bit>():bit
    case 'byte':
      Env.in, InCont = splitBitstream(Env.in, Size * 8)
      Env.out = streamOf<bit>():byte
    case 'int':
      Env.in, InCont = splitIntstream(Env.in, Size)
      Env.out = streamOf<int>():int
    case 'ast':
      Env.in, InCont = splitAststream(Env.in, Size)
      Env.out = stream_of<ast>():ast
  Env = Run [[ S ]] Env
  Result = Env.out
  Env.In = Result
  Env.Out = OutCont
  Size = Result.stream.size()
  Env.out = Write [[ (varuint32) ]] Size Env.out
  Env = Run [[ (copy) ]] Env
  Env.in = InCont
  if Env.out.type == 'bit':
     Env.out = byteAlign(Env.out)
  return Env
```

Note that we have predefined the format for the _extract_ statement. This is
intentional. It allows the implementation greater flexibility on how the size of
the resulting output buffer is written. The semantics above implies that the
size is written after the buffer is generated, and then the output buffer is
copied back into the original output stream.

Alternatively, the implementation is allowed to reserve space for the resulting
buffer size. It can then directly output the the results of running _S_ into the
original buffer, and backpatch the size afterwards.

For byte and bit streams, this would require writting out 5 bytes initially, and
then backpatching the value using LEB128, but forcing 5 chunks to be generated,
even it that many chunks were not necessary for the size.

#### (copy)

Copies the rest of the input stream to the output stream.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode['copy']
  Values = vector<value>()
  Values.append('copy')
  Stack.append(preorder(Values))
  return In, Stack
```

```
Run [[ (copy) ]] Env:
  while not Env.in.empty() :
    V, Env.in = Read [[ (fixed 1) ]] In
    Env.out = Write [[ (fixed 1) ]] Out
  Return Env
```

### Method definitions and calls

This section defines definitions that define methods associated with a
symbol. Definitions can only appear as top-level statements, and all top-level
statements in a filter section must be a method definition.

#### (define N S1 \.\.\. Sn)


Defines methods _S1_ through _Sn_ for the symbol _N_.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode['define']
  Values = vector<value>()
  S, In = readSymbol(In)
  N, In  = readVaruint32(In)
  assert N > 0
  For i = 0; i < N; ++i:
    Values.prepend(Stack.popLast())
  Values.prepend(S)
  Values.prepend('define')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (define N S1 \.\.\. Sn) ]] Env:
  Meths = Methods()
  for i = 1; i <= n; ++i:
    Meths.append(Si);
  Env.def[N] = Methods
  return Env
```

#### (eval N)

Calls method 0 associated with symbol _N_.

Note: Should this be an expression?

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  Assert Op == Opcode['eval']
  Values = vector<value>()
  Values.prepend('eval')
  S, In = readSymbol(In)
  Values.prepend(S)
  Stack.append(preorder(Values))
  Return In, Stack
```

```
Run [[ (eval N) ]] Env:
  if not Env.def.is_defined(N):
    error
  PrevMethods = Env.methods
  Env.methods = Env.def[N]
  S = Env.methods[0]
  Env = Run S Env
  Env.methods = PrevMethods
  return Env
```

#### (call N)

Calls method _N_ associated with the current set of methods.

Note: Should this be an expression?

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode['call']
  Values = vector<value>()
  Values.append('call')
  N, In = readVaruint32(In)
  Values.append(n)
  Stack,push(preorder(values))
  return In, Stack
```

```
Run [[ (call N) ]] Env:
  assert N < Env.methods.size()
  S = Env.methods[N]
  return Run S Env
```

### Filtering

Filtering allows one to subdivide a single filter into a sequence of filters. It
also allows the ability to define the contents of each input/output stream as
they are filtered.

#### (filter S1 \.\.\. Sn)

```
Parse In Stack Opcode =
  Op, In = readUint8(In)
  assert Op == Opcode['filter']
  Values = vector<value>()
  N, In = readVarint32(In)
  assert N > 1
  for i = 1; i <= N; ++ i:
    Values.prepend(Stack.popLast())
  Values.prepend('filter')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (filter S1 \.\.\. Sn) ]] Env:
  FinalOut = Env.out
  for i = 1; i <= n; ++i:
    if i == n:
      Env.out = FinalOut
      Env = Run [[ En ]] Env
    else:
      Env.out = Outstream [[ Si ]]
      Env = Run [[ Ei ]] Env
    Env.in = Env.out
  return Env
```

#### (bit.to.bit S)

Runs statement _S_ assuming that both the input and the output streams are _bit_
streams.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode['bit.to.bit']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('bit.to.bit')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (bit.to.bit S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'bit' Env.in
  Env.out = fixStreamType 'bit' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (bit.to.bit S) ]] = return streamOf<bit>():bit
```


#### (bit.to.byte S)

Runs statement _S_ assuming that the input stream is a _bit_ stream, and the
output streams is a _byte_ stream.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode[`bit.to.byte']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('bit.to.byte')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (bit.to.byte S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'bit' Env.in
  Env.out = fixStreamType 'byte' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (bit.to.byte S) ]] = return streamOf<bit>():byte
```

#### (bit.to.int S)

Runs statement _S_ assuming that the input stream is a _bit_ stream, and the
output streams is an _int_ stream.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode[`bit.to.int']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('bit.to.int')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (bit.to.int S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'bit' Env.in
  Env.out = fixStreamType 'int' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (bit.to.int S) ]] = return streamOf<int>():int
```

#### (bit.to.ast S)

Runs statement _S_ assuming that the input stream is a _bit_ stream, and the
output streams is an _ast_ stream.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode[`bit.to.ast']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('bit.to.ast')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (bit.to.int S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'bit' Env.in
  Env.out = fixStreamType 'ast' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (bit.to.ast S) ]] = return streamOf<ast>():ast
```

#### (byte.to.bit S)

Runs statement _S_ assuming that the input stream is a _byte_ stream, and the
output streams is a _bit_ stream.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode[`byte.to.bit']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('byte.to.bit')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (byte.to.bit S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'byte' Env.in
  Env.out = fixStreamType 'bit' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (byte.to.bit S) ]] = return streamOf<bit>():bit
```

#### (byte.to.byte S)

Runs statement _S_ assuming that both the input and the output streams are _byte_
streams.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode[`byte.to.byte']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('byte.to.byte')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (byte.to.byte S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'byte' Env.in
  Env.out = fixStreamType 'byte' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (byte.to.byte S) ]] = return streamOf<bit>():byte
```

#### (byte.to.int S)

Runs statement _S_ assuming that the input stream is a _byte_ stream, and the
output streams is an _int_ stream.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode[`byte.to.int']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('byte.to.int')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (byte.to.int S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'byte' Env.in
  Env.out = fixStreamType 'int' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (byte.to.int S) ]] = return streamOf<int>():int
```

#### (byte.to.ast S)

Runs statement _S_ assuming that the input stream is a _byte_ stream, and the
output streams is an _ast_ stream.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode[`byte.to.ast']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('byte.to.ast')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (byte.to.ast S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'byte' Env.in
  Env.out = fixStreamType 'ast' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (byte.to.ast S) ]] = return streamOf<ast>():ast
```

#### (int.to.bit S)

Runs statement _S_ assuming that the input stream is an _int_ stream, and the
output streams is a _bit_ stream.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode[`int.to.bit']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('int.to.bit')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (int.to.bit S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'int' Env.in
  Env.out = fixStreamType 'bit' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (int.to.bit S) ]] = return streamOf<bit>():bit
```

#### (int.to.byte S)

Runs statement _S_ assuming that the input stream is an _int_ stream, and the
output streams is a _byte_ stream.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode[`int.to.byte']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('int.to.byte')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (int.to.bit S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'int' Env.in
  Env.out = fixStreamType 'byte' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (int.to.byte S) ]] = return streamOf<bit>():byte
```

#### (int.to.int S)

Runs statement _S_ assuming that both the input and the output streams are _int_
streams.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode[`int.to.int']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('int.to.int')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (int.to.int S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'int' Env.in
  Env.out = fixStreamType 'int' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (int.to.int S) ]] = return streamOf<int>():int
```

#### (int.to.ast S)

Runs statement _S_ assuming that the input stream is an _int_ stream, and the
output streams is an _ast_ stream.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode[`int.to.ast']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('int.to.ast')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (int.to.ast S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'int' Env.in
  Env.out = fixStreamType 'ast' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (int.to.ast S) ]] = return streamOf<ast>():ast
```

#### (ast.to.bit S)

Runs statement _S_ assuming that the input stream is an _ast_ stream, and the
output streams is a _bit_ stream.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode[`ast.to.bit']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('ast.to.bit')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (ast.to.bit S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'ast' Env.in
  Env.out = fixStreamType 'bit' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (ast.to.bit S) ]] = return streamOf<bit>():bit
```

#### (ast.to.byte S)

Runs statement _S_ assuming that the input stream is an _ast_ stream, and the
output streams is a _byte_ stream.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode[`ast.to.byte']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('ast.to.byte')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (ast.to.byte S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'ast' Env.in
  Env.out = fixStreamType 'byte' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (ast.to.byte S) ]] = return streamOf<bit>():byte
```

#### (ast.to.int S)

Runs statement _S_ assuming that the input stream is an _ast_ stream, and the
output streams is an _int_ stream.

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert Op == Opcode[`ast.to.int']
  Values = vector<value>()
  Values.prepend(Stack.popLast())
  Values.prepend('ast.to.int')
  Stack.append(postorder(Values))
  return In, Stack
```

```
Run [[ (ast.to.int S) ]] Env:
  InType = Env.in.type
  OutType = Env.out.type
  Env.in = fixStreamType 'ast' Env.in
  Env.out = fixStreamType 'int' Env.Out
  Env = Run [[ S ]] Env
  Env.in = fixStreamType InType Env.in
  Env.out = fixStreamType OutType Env.out
  return Env
```

```
OutStream [[ (ast.to.int S) ]] = return streamOf<int>():int
```

### Other

This section covers statements that don't fit into the other sections.

#### (flush)

Directive to communicate to the filter that all elements on the output stream
can be flushed at this point (i.e. won't be used later)

```
Parse In Stack Opcode:
  Op, In = readUint8(In)
  assert  Op == Opcode['flush']
  Values = vector<value>()
  Values.append('flush')
  Stack.append(preorder(Values))
  return In, Stack
```

Note that the flush instruction is a directive to the implementation that it may
flush any values already on the output stream. Semantically, this is not
modeled, since the semantics of this framework doesn't model the concept of streaming.

```
Run [[ (flush) ]] Env = Env
```

## Support functions

The following subsections define the support functions for filter constructs.

### byteAlign

Aligns a _bit_/_byte_ stream to a byte boundary. Note: The current semantics
model doesn't contain enough information to correctly model _byteAlign_, so no
definition is given.

### fixStreamType

Fixes the stream type to the given _Kind_, if _S_ isn't of that kind. Used when
streams use filter conversions, and converts back and forth between _bit_
streams and _byte_ streams.

```
fixStreamType(Kind:symbol, S:stream) -> stream:
  if S.type == Kind the
    return S
  switch S.type:
    case 'bit':
      assert Kind == 'byte'
      return byteAlign(S):byte
    case 'byte':
      assert Kind == 'bit'
      return S.stream:bit
```

### readAst

Incrementally flattens the ast into a sequence of integers, and then returns the
next integer on the input stream.

```
readAst(In:stream) -> value x stream = 
  assert In.type == 'ast'
  V = In.stream.main.popFirst()
  switch V.type:
    case 'int':
      return V, In
    case 'sym':
      for i = V.stream.main.size(); i > 0; --i:
        In.stream.main.prepend(V.value[i-1])
      return V.stream.main.size():
    case 'node':
      if V.postorder:
        In.stream.main.prepend(V.value[0])
        for i = V.stream.main.size(); i > 1; --i:
          In.stream.main.prepend(V.value[i-1])
      else: 
        for i = V.stream.main..size(); i > 0; --i:
          In.main.main.prepend(V.value[i-1])
        return readAst(In)
    case 'void':
      return readAst(In)
```

### readFixed

Reads the next N bits as an (unsigned) integer.

```
readFixed(N: u32, In:stream) -> value x stream:
  assert N >= 1
  assert N <= 64
  V = 0
  for i = 0; i < N; ++i:
    V = (V << 1) & In.stream.popFirst()
  return V:int, In
```

### readInt

Reads an integer from an integer input stream.

```
readInt(In:stream) -> value x stream:
  assert In.type == 'int'
  V = In.stream.popFirst()
  return V, In
```

### readLEB128

Reads an unsigned integer using
[LEB128](https://en.wikipedia.org/wiki/LEB128). _N_ defines the chunk size to
use (LEB128 is defined with _N=8_).

```
readLEB128(N:u32, In:stream) -> value x stream:
  assert N >= 2
  assert N <= 64
  V = 0
  Shift = 0
  while true:
    Chunk, In = readFixed(N, In)
    Data = Chunk & ~(cast<u64>(1) << (N - 1))
    V |= Data << Shift;
    if Chunk >> (N - 1) == 0:
      return V
    Shift += N - 1
```

### readSignedLEB128

Reads a signed integer using
[Signed LEB128](https://en.wikipedia.org/wiki/LEB128). _N_ defines the chunk
size to use (SIgned LEB128 is defined with _N=8_). VSize is the number of bits
used to model the corresponding integer type (_32_ for _i32_ and _u32_, _64_ for
_i64_ and _u64_).

```
readSignedLEB128(N:u32, VSize:u32, In:stream) -> i64 x stream:
  assert N >= 2
  assert N <= 64
  V = 0
  Shift = 0
  while true:
    Chunk, In = readFixed(N, In)
    Data = Chunk & ~(cast<u64>(1) << (N - 1))
    V |= Data << Shift
    Shift += N - 1
    if Chunk >> (N - 1) == 0:
      if Shift < VSize && Chunk >> (N - 2) == 1:
        V |= - (cast<u64>(1) << Shift)
      return cast<u64>(V):int, In
```

### readSymbol

Reads a symbol from the input stream. Returns the read symbol and the updated
input stream.

```
readSymbol(In: stream) -> value x stream:
  S = symbol()
  N, In = Read [[ (varuint32) ]] In
  for i = 1; i <= N; ++i:
    C, In = Read [[ (uint8) ]] In
    S.append(C)
  return S:sym, In
```

### readUnit8

Reads one byte from a bitstream, and returns the read
(unsigned) integer and the updated input stream.

```
readUint8(In:stream) -> value x stream:
  return readFixed(8, In)
```

### readUint32

Reads a four-byte little endian unsigned integer from a bitstream. Returns the
read integer and the updated input stream.

```
readUint32(In:stream) -> value x stream:
  return readFixed(32, In)
```

### readUint64

Reads an eight-byte little endian unsigned integer from a bitstream. Returns the
read integer and the updated input stream.

```
readUint64(In: stream) -> value x stream:
  return readFixed(64, In)
```

###readVarint32

Reads a [Signed LEB128](https://en.wikipedia.org/wiki/LEB128) variable-length
_i32_ value.

```
readVarint32(In:stream) -> value x stream:
  return readSignedLEB128(8, 32, In)
```

### readVarint64

Reads a [Signed LEB128](https://en.wikipedia.org/wiki/LEB128) variable-length
_i64_ value.

```
readVarint64(In:stream) -> value x stream:
  return readSignedLEB128(8, 64, In)
```

### readVaruint1

Reads a [LEB128](https://en.wikipedia.org/wiki/LEB128) unsigned integer, limited
to the range [0, 1].

```
readVaruint1(In:stream) -> value x stream:
  V, In = readLEB128(8, In)
  assert cast<u64>(V.value) <= 1
  return V, In
```

### readVaruint7

Reads a [LEB128](https://en.wikipedia.org/wiki/LEB128) unsigned integer, limited
to the range [0, 127].

```
readVaruint7(In:stream) -> value x stream:
  V, In = readLEB128(8, In)
  assert cast<u64>(V.Value) <= 127
  return V, In
```

### readVaruint32

Reads a [LEB128](https://en.wikipedia.org/wiki/LEB128) _u32_ value.

```
readVaruint32(In:stream) -> value x stream:
  V, In = readLEB128(8, In)
  assert V.value >> 32 == 0
  return V, In
```

### readVaruint64

Reads a [LEB128](https://en.wikipedia.org/wiki/LEB128) _u64_ value.

```
readVaruin64(In:stream) -> value x stream:
  V, In = readLEB128(8, In)
  assert V.value >> 64 == 0
  return V, In
```

### readVbr

Reads a [LEB128](https://en.wikipedia.org/wiki/LEB128) unsigned integer, except
that the chunk size is defined by _N_.

```
readVbr(N:u32, In:stream) -> value x stream:
  return readLEB128(N, In)
```

### readIvbr

Reads a [Signed LEB128](https://en.wikipedia.org/wiki/LEB128) integer, except
that the chunk size is defined by _N_.

```
readIivbr(N:u32, In:stream) -> value x stream:
  return readSignedLEB128(N, 64, In)
```

### splitAststream

Splits the given _ast_ stream (i.e. vector of values) into two _ast_
streams. The first stream contains the first _N_ values. The second stream has
all remaining values after the first _N_ values are removed.

```
splitAststream(In: stream, N:u64) -> stream x stream:
  Split = streamOf<ast>
  for i = 1; i < N; ++i:
    V = In.stream.main.popFirst()
    Split.main.append(V)
  Return Split:In.type, In
```
### splitBitstream

Splits the given _bit_ stream into two _bit_ streams. The first stream contains
the first _N_ bits of the given stream. The second has all remaining bits after
the first _N_ bits are removed.

```
splitBitstream(In:stream, N:u64) -> stream x stream:
  Split = streamOf<bit>
  for i = 1; i < N; ++i:
    B = In.stream.popFirst()
    Split.append(B)
  return Split:In.type, In
```

### splitIntstream

Splits the given _int_ stream into two _int_ streams. The first stream contains
the first _N_ integers of the given stream. The second has all remaining
integers after the first _N_ integers are removed.

```
splitIntstream(In:stream, N:u64) -> stream x stream:
  split = streamOf<int>()
  for i = 1; i < N; ++i:
     V = In.stream.popfirst()
     Split.append(V)
  return split:int, In
```

### writeAst

Writes a (possibly ast) value onto an ast stream.

```
writeAst(V:value, Out:stream) =
  assert Out.type == 'ast'
  Out.stream.main.append(V)
  return Out
```

### writeFixed

Writes out an unsigned integer as a sequence of _N_ fixed bits.

```
writeFixed(N:u32, V:value, Out:stream) - > stream:
  assert N > 0
  assert N <= 64
  V = cast<u64>(V.value)
  for i = N; i > 0; --i:
    Out.append((V >> (i - 1)) & 1)
  return Out
```

### writeIint

Writes an integer to an int ast stream. Returns the updated output stream.

```
writeIint(V:value, Out:stream) -> stream:
  assert V.type == 'int'
  assert Out.type == 'int'
  Out.stream.append(V.value)
  return Out
```

### writeLEB128

Writes an unsigned integer using
[LEB128](https://en.wikipedia.org/wiki/LEB128). _N_ defines the chunk size to
use (LEB128 is defined with _N=8_).

```
writeLEB128(N: u32, V:value, Out:stream) -> stream:
  assert N >= 2
  assert N <= 64
  V = cast<u64>(V.value)
  Mask = (1 << (N - 1)) - 1
  while true:
    Chunk = V & Mask
    V >>= N - 1
    if V == 0:
      return writeFixed(N, Chunk, Out)
    Chunk |= cast<u64>(1) << (N - 1)
    writeFixed(N, Chunk, Out)
```

### writeSignedLEB128

Writes a signed integer using
[Signed LEB128](https://en.wikipedia.org/wiki/LEB128). _N_ defines the chunk
size to use (SIgned LEB128 is defined with _N=8_). VSize is the number of bits
used to model the corresponding integer type (_32_ for _i32_ and _u32_, _64_ for
_i64_ and _u64_).

```
writeSignedLEB128(N: u32, V:Value, VSize:u32, Out:stream) -> stream:
  assert N >= 2
  assert N <= 64
  assert abs(cast<i64>(V)) >> VSize == 0
  V = cast<i64>(V.value)
  Negative = V < 0
  Mask = (1 << (N - 1)) - 1
  while true:
    Chunk = V & Mask
    V >>= N - 1
    If Negative:
      Value |= - (cast<u64>(1) << (VSize - (N - 1)))
    If V == 0 && Chunk >> (N - 2) == 0:
      return Out
    else if V == -1 && Chunk >> (N - 2) == 1:
      return Out
    else:
      Chunk |= cast<u64>(1) << (N - 1)
      Out = writeFixed(N, C, Out)
```

### writeUint8

Writes one byte from a bitstream. Returns the updated output stream.

```
writeUuint8(V: Value, Out:stream) -> stream:
  return writeFixed(8, V, Out)
```

### writeUint32

Writes a four-byte little endian unsigned integer from a bitstream. Returns the
updated output stream.

```
writeUuint32(V: value, Out:stream) -> stream:
  return writeFixed(32, V, Out)
```

### writeUint64

Writes an eight-byte little endian unsigned integer from a bitstream. Returns
the updated output stream.

```
writeUint64(V: value, Out:stream) -> Stream:
  return writeFixed(64, V, Out)
```

### writeVarint32

Writes a [Signed LEB128](https://en.wikipedia.org/wiki/LEB128) variable-length
_i32_ value.

```
writeVarint32(V:value, Out:stream) -> stream:
  return writeSignedLEB128(8, V 32, Out)
```

### writeVarint64

Writes a [Signed LEB128](https://en.wikipedia.org/wiki/LEB128) variable-length
_i64_ value.

```
writeVarint64(V:value, Out:stream) -> stream:
  return writeSignedLEB128(8, V, 64, Out)
```

### writeVaruint1

Writes a [LEB128](https://en.wikipedia.org/wiki/LEB128) unsigned integer, limited
to the range [0, 1].

```
writeVaruint1(V:value, Out:stream) -> stream:
  assert cast<u64>(V.value) <= 1
  return writeLEB128(8, V, Out)
```

### writeVaruint7

Writes a [LEB128](https://en.wikipedia.org/wiki/LEB128) unsigned integer, limited
to the range [0, 127].

```
writeVaruint7(V:value, Out:stream) -> stream:
  assert cast<u64>(V.Value) <= 127
  return writeLEB128(8, V, Out)
```

### writeVaruint32

Writes a [LEB128](https://en.wikipedia.org/wiki/LEB128) _u32_ value.

```
writeVaruint32(V:value, Out:stream) -> stream:
  assert cast<u64>(V.value) >> 32 == 0
  return writeLEB128(8, V, In)
```

### writeVaruint64

Writes a [LEB128](https://en.wikipedia.org/wiki/LEB128) _u64_ value.

```
writeVaruin64(V:value, Out:stream) -> Stream
  assert cast<u64>(V.value) >> 64 == 0
  return writLEB128(8, V, In)
```

### writeVbr

Writes a [Signed LEB128](https://en.wikipedia.org/wiki/LEB128) integer, except
that the chunk size is defined by _N_.

```
writeVbr(N:u32, V:value, Out:stream) -> stream:
  return writeLEB128(N, V, Out)
```

### writeIvbr

Writes a [Signed LEB128](https://en.wikipedia.org/wiki/LEB128) integer, except
that the chunk size is defined by _N_.

```
writeIvbr(N: u32, V:value, Out:stream) -> stream:
  return writeSignedLEB128(N, V, 64, Out)
```

## Defaults

This section presents the initial (default) definitions assumed to already be
preloaded by the decompressor.

TODO: Fill this in.

## Top-level driver

At the top, level, the decompressor only understands sections in a compressed
WASM module. It also assumes that this top-level structure is never compressed.
As a result, the top level input/output streams are _byte_ streams.

The top-level driver takes two streams, the _byte_ input stream and the _byte_
output stream. Returns the generated output stream. It also takes a copy of
the compiled in definitions preloaded into the decompressor.

```
decode(In:stream, Out:stream, Defaults: sym_ap) -> stream:
  assert In.type == 'byte'
  assert Out.type == 'byte'
  Methods =  vector<symbo>()
  Env = (In, Out, copyMap(Defaults), Methods)
  MagicNumber, In = readUint32(In)
  Version, In = readUint32(In)
  while not In.stream.empty():
    SectionName, In = readSymbol(In)
    if Env.def.is_defined(SectionName):
      Env = Run [[ (eval SectionName) ]] Env
    else:
      Env = Run [[ (copy) ]] Env
```

