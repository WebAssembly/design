# Text Format

The purpose of this text format is to support:
* View Source on a WebAssembly module, thus fitting into the Web (where every
  source can be viewed) in a natural way.
* Presentation in browser development tools when source maps aren't present
  (which is necessarily the case with the MVP).
* Writing WebAssembly code directly for reasons including pedagogical,
  experimental, debugging, optimization, and testing of the spec itself.

The text format is equivalent and isomorphic to the [binary format](BinaryEncoding.md).

The text format will be standardized, but only for tooling purposes:
* Compilers will support this format for `.S` and inline assembly.
* Debuggers and profilers will present binary code using this textual format.
* Browsers will not parse the textual format on regular web content in order to
  implement WebAssembly semantics.

Given that the code representation is actually an
[Abstract Syntax Tree](ASTSemantics.md), the syntax would contain nested
statements and expressions (instead of the linear list of instructions most
assembly languages have).

There is no requirement to use JavaScript syntax; this format is not intended to
be evaluated or translated directly into JavaScript.
