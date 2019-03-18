# Text Format

## Note: This document is no longer being updated. Please see the [normative documentation](http://webassembly.github.io/spec/core/text/index.html).

WebAssembly will define a standardized text format
that encodes a WebAssembly module with all its contained definitions
in a way that is equivalent to the [binary format](BinaryEncoding.md).
This format will use [S-expressions][] (avoiding syntax bikeshed discussions) to express modules and definitions while allowing a [linear representation](#linear-instructions) for the code in function bodies.
This format is understood by tools and used in browsers when [debugging](#debug-symbol-integration)
modules.

  [S-expressions]: https://en.wikipedia.org/wiki/S-expression

The format will be close to [this grammar][],
which provides a raw syntax in direct correspondence with the binary format
as well as some syntactic sugar on top.
Examples can be found in the WebAssembly [test suite][].

  [this grammar]: https://github.com/WebAssembly/spec/tree/master/interpreter/#s-expression-syntax
  [test suite]: https://github.com/WebAssembly/spec/tree/master/test/core/

The following tools currently understand this format:

* [specification interpreter][] consumes an s-expression syntax.
* [wabt][] consumes compatible s-expressions.
* [binaryen][] can consume compatible s-expressions.
* [LLVM backend][] (the `CHECK:` parts of these tests) emits compatible s-expressions.
* [WAVM backend][] consumes compatible s-expressions.

  [specification interpreter]: https://github.com/WebAssembly/spec/tree/master/interpreter/
  [wabt]: https://github.com/WebAssembly/wabt
  [binaryen]: https://github.com/WebAssembly/binaryen
  [LLVM backend]: https://github.com/llvm/llvm-project/tree/master/test/CodeGen/WebAssembly
  [WAVM backend]: https://github.com/AndrewScheidecker/WAVM/tree/master/Test

The recommended file extension for WebAssembly code in textual format is `.wat`.

**Note:** The `.wast` format understood by some of the listed tools is a superset of the `.wat` format that is intended for writing test scripts.
Besides the definition of modules such scripts can contain assertions and other commands as defined by a [grammar extension].
These extensions are *not* part of the official text format, which may only contain a single module.

  [grammar extension]: https://github.com/WebAssembly/spec/tree/master/interpreter/#scripts


# Linear instructions

WebAssembly function bodies encode bytecode instructions which have specified canonical opcode names. A linear presentation of these sequences of instructions allows a direct human-readable order-preserving presentation of the binary format. This format is suitable for opcode by opcode inspection of a WebAssembly program and can readily be related to the [semantics](Semantics.md) of the format. 

Here is an example function illustrated in C++, binary, and text (linear
assembly bytecode):

<table>
  <tr>
    <th>C++</th>
    <th>Binary</th>
    <th>Text</th>
  </tr>
  <tr>
    <td><pre>
int factorial(int n) {
  if (n == 0)
    return 1;
  else
    return n * factorial(n-1);
}</pre></td>
    <td><pre>
20 00
42 00
51
04 7e
42 01
05
20 00
20 00
42 01
7d
10 00
7e
0b</pre></td>
    <td><pre>
get_local 0
i64.const 0
i64.eq
if i64
    i64.const 1
else
    get_local 0
    get_local 0
    i64.const 1
    i64.sub
    call 0
    i64.mul
end</pre></td>
  </tr>
</table>

## Debug symbol integration

The binary format inherently strips names from functions, locals, globals, etc,
reducing each of these to dense indices. Without help, the text format must
therefore synthesize new names. However, as part of the [tooling](Tooling.md)
story, a lightweight, optional "debug symbol" global section may be defined
which associates names with each indexed entity and, when present, these names
will be used in the text format projected from a binary WebAssembly module.

# Design considerations

The text format for WebAssembly needs to
be able to represent any well-structured module unambiguously.
In addition to function bodies and their instruction sequences, it also needs a
way of encoding declarations, function
signatures, data segments, tables, and other
[sections](BinaryEncoding.md#high-level-structure) of the binary format.

In addition, expansions or extensions to a standardized text format may offer
additional syntactic sugar, macro expansions, or other conveniences to make it
easier to write and read WebAssembly. However, since the linear sequence of
bytecodes in a binary module may have more than one higher-level
representation, such sugar or conveniences may not be possible to resolve
canonically from the binary format alone.

The design goals for a WebAssembly text format derive from the following use
cases:

* View Source on a WebAssembly module, thus fitting into the Web (where every source can be viewed) in a natural way.
* Presentation in browser development tools when source maps aren't present (which is necessarily the case with [the Minimum Viable Product (MVP)](MVP.md)).
* Writing WebAssembly code directly for reasons including pedagogical, experimental, debugging, optimization, and testing of the spec itself.

There is no requirement to use JavaScript syntax; this format is not intended to be evaluated or translated directly into JavaScript. There may also be substantive reasons to use notation that is different than JavaScript (for example, WebAssembly has a 64-bit integer type, and it should be represented in the text format, since that is the natural thing to do for WebAssembly, regardless of JavaScript not having such a type). On the other hand, when there are no substantive reasons and the options are basically bikeshedding, then it does make sense for the text format to match existing conventions on the Web (for example, curly braces, as in JavaScript and CSS).

The text format may not be uniquely representable. Multiple textual files will likely assemble to the same binary file. For example, whitespace shouldn't be significant and memory initialization can be broken out into smaller pieces in a text format.

The text format should be precise in that values that cannot be accurately represented in the binary format are considered invalid text. Floating-point numbers should therefore be represented as hexadecimal floating-point as specified by C99, C++17, and IEEE-754-2008 section 5.12.3. The textual format may be improved to also support more human-readable representations, but never at the cost of accurate representation.
