# Design Rationale

WebAssembly was designed incrementally, and desisions were made with a solid
rationale which may turn out to be incorrect in the future. Sometimes, decisions
were made without the luxury of data, and the designers would like to revisit
certain decisions once real end-to-end usecases are met and Science can be
performed.

You, the reader, may also be interested in understanding why
[Abstract Syntax Tree (AST)](AstSemantics.md) semantics are the way they are,
and digging through github's issues and pull requests becomes difficult.

This rationale document tries to list how decisions were made, and where
tradeoffs were made for the sake of language ergonomics, portability,
performance, security, and Getting Things Done.

[Limited local nondeterminism](Nondeterminism.md) is detailed in its own
document, its rationale is also explained here.


## Why AST?

Why not a stack-, register- or SSA-based bytecode?
* Trees allow a smaller binary encoding: [JSZap][], [Slim Binaries][].
* [Polyfill prototype][] shows simple and efficient translation to asm.js.

  [JSZap]: https://research.microsoft.com/en-us/projects/jszap/
  [Slim Binaries]: https://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.108.1711
  [Polyfill prototype]: https://github.com/WebAssembly/polyfill-prototype-1


## Basic Types Only

WebAssembly only represents [a few types](AstSemantics.md#Types).
* More complex types can be formed from these basic types. It's up to the source
  language compiler to express its own types in terms of the basic machine
  types. This allows WebAssembly to present itself as a virtual ISA, and lets
  compilers target it as they would any other ISA.
* These types are efficiently executed by all modern architectures.
* Smaller types (such as `i8` and `i16`) are no more efficient and are only
  semantically meaningful for memory accesses.
* Other types (such as `f16`, `f80`, `i128`) aren't widely supported by existing
  hardware and can be supported by runtime libraries if developers wish to use
  them. They can be added to WebAssembly later without compromising MVP.
* More complex object types aren't semantically useful for MVP. They may become
  useful to support other languages, especially when considering
  [garbage collection](GC.md).


## Load/Store Addressing

Load/store instructions include an immediate offset used for
[addressing](AstSemantics.md#Addressing). This is intended to simplify folding
of offsets into complex address modes in hardware, and to simplify bounds
checking optimizations. It offloads some of the optimization work to the
compiler that targets WebAssembly, executing on the developer's machine, instead
of performing that work in the WebAssembly compiler on the user's machine.


## Alignment Hints

Load/store instructions contain
[alignment hints](AstSemantics.md#Alignment). This makes it easier to generate
efficient code on certain hardware architectures.

Either tooling or an explicit opt-in "debug mode" in the spec could allow
execution of a module in a mode that threw exceptions on misaligned access.
This mode would incur some runtime cost for branching on most platforms which is
why it isn't the specified default.


## Out of Bounds

The ideal semantics is for
[out-of-bounds accesses](AstSemantics.md#Out-of-Bounds) to trap, but the
implications are not yet fully clear.

There are several possible variations on this design being discussed and
experimented with. More measurement is required to understand the associated
tradeoffs.

  * After an out-of-bounds access, the instance can no longer execute code and
    any outstanding JavaScript [ArrayBuffer][] aliasing the linear memory are
    detached.
    * This would primarily allow hoisting bounds checks above effectful
      operations.
    * This can be viewed as a mild security measure under the assumption that
      while the sandbox is still ensuring safety, the instance's internal state
      is incoherent and further execution could lead to Bad Things (e.g., XSS
      attacks).
  * To allow for potentially more-efficient memory sandboxing, the semantics
    could allow for a nondeterministic choice between one of the following when
    an out-of-bounds access occurred.
    * The ideal trap semantics.
    * Loads return an unspecified value.
    * Stores are either ignored or store to an unspecified location in the
      linear memory.
    * Either tooling or an explicit opt-in "debug mode" in the spec should allow
      execution of a module in a mode that threw exceptions on out-of-bounds
      access.

  [ArrayBuffer]: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/ArrayBuffer


## Resizing

Implementations provide a `page_size` operation which allows them to efficiently
map the underlying OS's capabilities to the WebAssembly application, as well as
to communicate their own implementation details in a useful manner to the
developer.


## Control Flow

See [#299](https://github.com/WebAssembly/design/pull/299).


## Locals

Address-taken variables in the call stack are expected to be in linear memory,
since they cannot be represented as locals. This prevents WebAssembly from
performing clever optimizations on the stack and liveness of such variables, but
this loss isn't expected to be consequential.

Conversely, non-address taken values which are usually on the stack are instead
represented as locals inside functions. This effectively means that WebAssembly
has an infinite set of registers, and can choose to spill values as it sees fit
in a manner unobservable to the hosted code. This implies that there's a shadow
stack, which is also used to spill return values. This allows strong security
properties to be enforced, but does mean that two stacks are maintained (one by
the VM, the other by the compiler which targets WebAssembly) which can lead to
some inefficiencies.

Local values can also be pre-colored, meaning that multiple incoming SSA values
which have separate liveness can "share" the storage represented by a
local. This offloads some of the optimization work from the WebAssembly VM.


## Variable-Length Argument Lists

C and C++ compilers are expected to implement variable-length argument lists by
storing arguments in a buffer in linear memory and passing a pointer to the
buffer. This greatly simplifies WebAssembly VM implementations by punting this
ABI consideration to the front-end compiler. It does negatively impact
performance, but variable-length calls are already somewhat slow.


## Multiple Return Values

TODO


## Indirect Calls

The exact semantics of indirect function calls, function pointers, and what
happens when calling the wrong function, are still being discussed.

Fundamentally linear memory is a simple collection of bytes, which means that
some integral representation of function pointers must exist. It's desirable to
hide the actual address of generated code from untrusted code because that would
be an unfortunate information leak which could have negative security
implications. Indirection is therefore desired.

One extra concern is that existing C++ code sometimes stores data inside of what
is usually a function pointer. This is expected to keep working.

Dynamic linking further complicates this: WebAssembly cannot simply standardize
on fixed-size function tables since dynamically linked code can add new
functions, as well as remove them.


## Expressions with Control Flow

Expression trees offer significant size reduction by avoiding the need for
`set_local`/`get_local` pairs in the common case of an expression with only one,
immediate use. The following primitives provide AST nodes that express control
flow and thus allow more opportunities to build bigger expression trees and
further reduce `set_local`/`get_local` usage (which constitute 30-40% of total
bytes in the
[polyfill prototype](https://github.com/WebAssembly/polyfill-prototype-1)).
Additionally, these primitives are useful building blocks for
WebAssembly-generators (including the JavaScript polyfill prototype).


## Limited Local Nondeterminism

The current specification is fairly strict when it comes to
[limited local nondeterminism](Nondeterminism.md) of operations: it tries to
specify all possible corner cases.

As WebAssembly gets implemented and tested with multiple languages on multiple
architectures there may be a need to revisit some of the decisions:

* When all relevant hardware implement features the same way then there's no
  need to get creative.
* When different languages have different expectations then it's unfortunate if
  WebAssembly measurably penalizes one's performance by enforcing determinism
  which that language doesn't care about, but which another language may want.
