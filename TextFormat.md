# Text Format

WebAssembly does not yet have a standardized text format that encodes function
bodies in addition to module structure, data segments and other program
metadata in a way that is eqivalent to the [binary format](BinaryEncoding.md).
WebAssembly does, however, have a specified
[textual representation](#linear-bytecode) of function bodies which should be displayed in browsers and other tools when [debugging](#debug-symbol-integration)
modules.

# Linear bytecode

WebAssembly function bodies encode bytecode instructions which have specified canonical opcode names. A linear presentation of these sequences of instructions allows a direct human-readable order-preserving presentation of the binary format. This format is suitable for opcode by opcode inspection of a WebAssembly program and can readily be related to the [sematics](Semantics.md) of the format. 

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
    return n * fac(n-1);
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

# Tool conventions

Most WebAssembly tools currently use [s-expressions][] to represent modules in a
textual format. Although the s-expression format is not an official text format,
it is a tooling convention because it allows for the representation of function
signatures, declarations, and other metadata and it doesn't have much of a
syntax to speak of (avoiding syntax bikeshed discussions).

  [s-expressions]: https://en.wikipedia.org/wiki/S-expression

Here are some of these prototypes. Keep in mind that these *aren't* official,
and the final standard format may look entirely different:

* [Prototype specification][] consumes an s-expression syntax.
* [WAVM backend][] consumes compatible s-expressions.
* [wabt][] consumes compatible s-expressions.
* [LLVM backend][] (the `CHECK:` parts of these tests) emits compatible s-expressions.
* [ilwasm][] emits compatible s-expressions.
* [binaryen][] can consume compatible s-expressions.

  [prototype specification]: https://github.com/WebAssembly/spec/tree/master/interpreter/test
  [LLVM backend]: https://github.com/llvm-mirror/llvm/tree/master/test/CodeGen/WebAssembly
  [WAVM backend]: https://github.com/AndrewScheidecker/WAVM/tree/master/Test
  [V8 prototype]: https://github.com/WebAssembly/v8-native-prototype
  [ilwasm]: https://github.com/WebAssembly/ilwasm
  [wabt]: https://github.com/WebAssembly/wabt
  [binaryen]: https://github.com/WebAssembly/binaryen

## Debug symbol integration

The binary format inherently strips names from functions, locals, globals, etc,
reducing each of these to dense indices. Without help, the text format must
therefore synthesize new names. However, as part of the [tooling](Tooling.md)
story, a lightweight, optional "debug symbol" global section may be defined
which associates names with each indexed entity and, when present, these names
will be used in the text format projected from a binary WebAssembly module.

# Future design
## :unicorn:

An official text format for WebAssembly needs to
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

## Additional design considerations

There is no requirement to use JavaScript syntax; this format is not intended to be evaluated or translated directly into JavaScript. There may also be substantive reasons to use notation that is different than JavaScript (for example, WebAssembly has a 64-bit integer type, and it should be represented in the text format, since that is the natural thing to do for WebAssembly, regardless of JavaScript not having such a type). On the other hand, when there are no substantive reasons and the options are basically bikeshedding, then it does make sense for the text format to match existing conventions on the Web (for example, curly braces, as in JavaScript and CSS).

The text format may not be uniquely representable. Multiple textual files will likley assemble to the same binary file. For example, whitespace shouldn't be significant and memory initialization can be broken out into smaller pieces in a text format.

The text format should be precise in that values that cannot be accurately represented in the binary format are considered invalid text. Floating-point numbers should therefore be represented as hexadecimal floating-point as specified by C99, C++17, and IEEE-754-2008 section 5.12.3. The textual format may be improved to also support more human-readable representations, but never at the cost of accurate representation.
