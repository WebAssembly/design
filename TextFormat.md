# Text Format

The purpose of this text format is to support:

* View Source on a WebAssembly module, thus fitting into the Web (where every
  source can be viewed) in a natural way.
* Presentation in browser development tools when source maps aren't present
  (which is necessarily the case with [the Minimum Viable Product (MVP)](MVP.md)).
* Writing WebAssembly code directly for reasons including pedagogical,
  experimental, debugging, optimization, and testing of the spec itself.

The text format is equivalent and isomorphic to the [binary format](BinaryEncoding.md).

The text format will be standardized, but only for tooling purposes:

* Compilers will support this format for `.S` and inline assembly.
* Debuggers and profilers will present binary code using this textual format.
* Browsers will not parse the textual format on regular web content in order to
  implement WebAssembly semantics.

Given that the code representation is actually an
[Abstract Syntax Tree](Semantics.md), the syntax would contain nested
statements and expressions (instead of the linear list of instructions most
assembly languages have).

There is no requirement to use JavaScript syntax; this format is not intended to
be evaluated or translated directly into JavaScript. There may also be
substantive reasons to use notation that is different than JavaScript (for
example, WebAssembly has a 32-bit integer type, and it should be represented
in the text format, since that is the natural thing to do for WebAssembly,
regardless of JavaScript not having such a type). On the other hand,
when there are no substantive reasons and the options are basically
bikeshedding, then it does make sense for the text format to match existing
conventions on the Web (for example, curly braces, as in JavaScript and CSS).

The text format isn't uniquely representable. Multiple textual files can assemble
to the same binary file, for example whitespace isn't relevant and memory initialization
can be broken out into smaller pieces in the text format.

The text format is precise in that values that cannot be accurately represented in the
binary format are considered invalid text. Floating-point numbers are therefore
represented as hexadecimal floating-point as specified by the C99 standard, which
IEEE-754-2008 section 5.12.3 also specifies. The textual format may be improved to also
support more human-readable representations, but never at the cost of accurate representation.

# Official Text Format

WebAssembly currently doesn't have a final, official, text format. As detailed above the
main purpose of the text format will be for human consumption, feedback from humans on
readability will therefore factor into standardizing a text format.

There are, however, prototype syntaxes which are used to bring up WebAssembly: it's easier
to develop using a text format than it is with a binary format, even if the ultimate
WebAssembly format will be binary. Most of these prototypes use [s-expressions][] because they
can easily represent expression trees and [ASTs](Semantics.md) (as opposed to CFGs)
and don't have much of a syntax to speak of (avoiding syntax bikeshed discussions).

  [s-expressions]: https://en.wikipedia.org/wiki/S-expression

Here are some of these prototypes. Keep in mind that these *aren't* official, and the final
official format may look entirely different:

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

# Debug symbol integration

The binary format inherently strips names from functions, locals, globals, etc,
reducing each of these to dense indices. Without help, the text format must
therefore synthesize new names. However, as part of the [tooling](Tooling.md)
story, a lightweight, optional "debug symbol" global section may be defined
which associates names with each indexed entity and, when present, these names
will be used in the text format projected from a binary WebAssembly module.
