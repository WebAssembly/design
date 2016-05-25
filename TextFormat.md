# Text Format

The purpose of this text format is to support:
* View Source on a WebAssembly module, thus fitting into the Web (where every
  source can be viewed) in a natural way.
* Presentation in browser development tools when source maps aren't present
  (which is necessarily the case with [the Minimum Viable Product (MVP)](MVP.md)).
* Working with WebAssembly code directly for reasons including pedagogical,
  experimental, debugging, profiling, optimization, and testing of the spec
  itself.

The text format is equivalent and isomorphic to the [binary format](BinaryEncoding.md).

The text format will be standardized, but only for tooling purposes; browsers
will not parse the textual format on regular web content in order to implement
WebAssembly semantics.

The text format is neither a subset or superset of JavaScript syntax, and it
cannot be directly evaluated as JavaScript. Some parts of the syntax resemble
JavaScript so that it's familiar and readable, however there are also
substantive reasons to use notation that is different than JavaScript (for
example, WebAssembly has a 32-bit integer type, and it is represented
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

## Philosophy:

 - Use JS-style sensibilities when there aren't reasons otherwise.
 - It's a compiler target, not a programming language, but readability still counts.


## High-level summary:

 - Curly braces for function bodies, blocks, etc., `/* */`-style and `//`-style
   comments, and whitespace is not significant.
   Nested `/* */`-style comments follow JavaScript's rules.

 - Semicolons separate expressions inside blocks, loops, and so on. The
   last statement in the sequence may omit its trailing semicolon to indicate
   that its result is being used as the result value of the parent construct.

 - `get_local` looks like a simple reference; `set_local` looks like an
   assignment. Constants use a simple literal syntax. This makes wasm's most
   frequent opcodes very concise.

 - Infix syntax for arithmetic, with simple overloading. Explicit grouping via
   parentheses. Concise and familiar with JS and others.

 - Prefix syntax with comma-separated operands for all other operators. For less
   frequent opcodes, prefer just presenting operator names, so that they're easy
   to identify.

 - Typescript-style `name: type` declarations.

 - Parentheses around call arguments, eg. `call $functionname(arg, arg, arg)`,
   and `if` conditions, eg. `if ($condition) { call $then() } else { call $else() }`,
   because they're familiar to many people and not too intrusive.

 - Put labels "where they go".


## Examples:

### Basics

```
  function $fac-opt ($a:i64) : (i64) {
    var $x:i64;
    $x = 1;
    br_if ($a <s 2) $end;
    loop $loop {
      $x = $x * $a;
      $a = $a + -1;
      br_if ($a >s 1) $loop;
    }
  $end:
    $x
  }
```

(from [fac.wast](https://github.com/WebAssembly/spec/blob/master/ml-proto/test/fac.wast))

The function return type has parentheses for symmetry with the parameter types,
anticipating adding multiple return values to wasm in the future.

The curly braces around the function body are not a `block` node; they are part
of the function syntax, reflecting how function bodies in wasm are block-like.

The last expression of the function body here acts as its return value. This
works in all block-like constructs (`block`, function body, `if`, etc.)

`>s` means *signed* greater-than. explicit unsigned or signed operators will be
suffixed with 'u' or 's', respectively.

The `$` sigil on user names cleanly ensures that they never collide with wasm
keywords, present or future.

The last statement has no trailing semicolon; this indicates that its result
value is being used as the block result value.


### Linear memory addresses

Addresses are printed as `[base,+offset,align=val]`. The offset can be omitted
when it is zero, and the alignment can be omitted for natural alignment.

Addresses are prefixed by an identifier giving the type of a load, or the
type of the value to store, optionally followed by a colon and a number of
bits of memory to access (for truncating stores or extending loads).
For loads, the number of bits is followed by `s` or `u` for signed and
unsigned.

Examples:

```
  function $loads_and_stores ($ptr : i32) : (i32) {
    var $t : i32;

    // Simple cases.
    $t = i32[$ptr];           // plain i32.load from address $ptr
    i32[$ptr] = $t;           // plain i32.store of $t to address $ptr

    // Various modifiers.
    f32[$ptr,+4] = -0x1.8p0;  // f32.store to address $ptr plus 4
    i64:8s[$ptr,+17,align=1]; // i64.load8_s from address $ptr plus 17 with alignment 1
  }
```

### A slightly larger example:

Here's some C code:

```
  float Q_rsqrt(float number)
  {
      long i;
      float x2, y;
      const float threehalfs = 1.5F;

      x2 = number * 0.5F;
      y  = number;
      i  = *(long *) &y;
      i  = 0x5f3759df - (i >> 1);
      y  = *(float *) &i;
      y  = y * (threehalfs - (x2 * y * y));
      y  = y * (threehalfs - (x2 * y * y));

      return y;
  }
```

Here's the corresponding LLVM wasm backend output + binaryen + slight tweaks:

```
  (func $Q_rsqrt (param $0 f32) (result f32)
    (local $1 f32)
    (set_local $1
      (f32.reinterpret/i32
        (i32.sub
          (i32.const 1597463007)
          (i32.shr_s
            (i32.reinterpret/f32
              (get_local $0))
            (i32.const 1)))))
    (set_local $1
      (f32.mul
        (get_local $1)
        (f32.sub
          (f32.const 0x1.8p+0)
          (f32.mul
            (get_local $1)
            (f32.mul
              (get_local $1)
              (set_local $0
                (f32.mul
                  (get_local $0)
                  (f32.const 0x1p-1))))))))
    (f32.mul
      (get_local $1)
      (f32.sub
        (f32.const 0x1.8p+0)
        (f32.mul
          (get_local $1)
          (f32.mul
            (get_local $0)
            (get_local $1)))))
   )
```

And here's the proposed text syntax:

```
   function $Q_rsqrt ($0:f32) : (f32) {
     var $1:f32;
     $1 = f32.reinterpret/i32(1597463007 - ((i32.reinterpret/f32($0)) >> 1));
     $1 = $1 * (0x1.8p0 - $1 * ($0 = $0 * 0x1p-1) * $1);
     $1 * (0x1.8p0 - $1 * $0 * $1)
   }
```

This shows off the compactness of infix operators with overloading. In the
s-expression syntax, these expressions are quite awkward to read, and this
isn't even a very big example. But the text syntax here is very short.

### Labels

Excerpt from labels.wast:

```
  (func $loop3 (result i32)
    (local $i i32)
    (set_local $i (i32.const 0))
    (loop $exit $cont
      (set_local $i (i32.add (get_local $i) (i32.const 1)))
      (if (i32.eq (get_local $i) (i32.const 5))
        (br $exit (get_local $i))
      )
      (get_local $i)
    )
  )
```

Corresponding proposed text syntax:

```
  function $loop3 () : (i32) {
    var $i:i32;
    $i = 0;
    loop $cont {
      $i = $i + 1;
      if ($i == 5) {
        br ($i) $exit;
      }
    $exit:
    }
  }
```

Note that the curly braces are part of the `if`, rather than introducing a
block. This reflects how `if` essentially provides `block`-like capabilities
in the wasm binary format.

### Nested blocks

Label definitions, like the `$exit:` above, introduce additional blocks nested
within the nearest `{`, without requiring their own `{`. This allows the deep
nesting of `br_table` to be printed in a relatively flat manner:

```
  {
    br_table ($index) [$red, $orange, $yellow, $green], $default;
  $red:
      // ...
  $orange:
      // ...
  $yellow:
      // ...
  $green:
      // ...
  $default:
  }
```

representing the following in nested form:

```
  (block $default
    (block $green
      (block $yellow
        (block $orange
          (block $red
            (br_table (get_local $index), [$red, $orange, $yellow, $green] $default)
          )
          // ...
        )
        // ...
      )
      // ...
    )
    // ...
  )
```

`br_table`s can have large numbers of labels, so this feature allows us to
avoid very deep nesting in many cases.

Note that when a label appears just before the closing `}`, it doesn't introduce
a new block; it just provides a name for the enclosing block's label.

## Operators with special syntax

The default syntax for operators is their opcode name followed by a
comma-separated list of operands. However, several operators have
special syntax.


### Control flow operators ([described here](https://github.com/WebAssembly/design/blob/master/AstSemantics.md))

| Name | Syntax | Examples
| ---- | ---- | ---- |
| `block` | `{` … *label*: `}` | `{ br $a; a: }`
| `loop` | `loop` *label* `{` … `}` | `loop $a { br $a; }`
| `if` | `if` `(` *condition* `)` `{` … `}` | `if (0) { 1 }`
| `if` | `if` `(` *condition* `) `{` … `} else `{` … `}` | `if (0) { 1 } else { 2 }`
| `br` | `br` *label* | `br $where`
| `br` | `br` `(` *expr* `)` *label* | `br ($v) $where`
| `br_if` | `br_if` `(` *expr* `)` *label* | `br_if ($x < $y) $where`
| `br_if` | `br_if` `(` *expr* `,` *condition* `)` *label* | `br_if ($v, $x < $y) $where`
| `br_table` | `br_table` `(` *index-expr* `)` `[` *label* `,` … `]` `,` *default-label* | `br_table ($i) [$somewhere, $or_other], $default`
| `br_table` | `br_table` `(` *expr* `,` *index-expr* `)` `[` *label* `,` … `]` `,` *default-label* | `br_table ($v, $i) [$somewhere, $or_other], $default`
| `return` | `return` | `return`
| `return` | `return` *expr* | `return $x`
| `unreachable` | `unreachable` | `unreachable`


### Basic operators ([described here](https://github.com/WebAssembly/design/blob/master/AstSemantics.md#constants))

| Name | Syntax | Example
| ---- | ---- | ---- |
| `i32.const` | … | `234`, `0xfff7`
| `i64.const` | … | `234`, `0xfff7`
| `f64.const` | … | `0.1p2`, `infinity`, `nan:0x789`
| `f32.const` | … | `0.1p2`, `infinity`, `nan:0x789`
| `get_local` | *name* | `$x + 1`
| `set_local` | *name* `=` *expr* | `$x = 1`
| `select` | `select` *expr*, *expr*, *expr* | `select 1, 2, $x < $y`
| `call` | `call` *name* `(`*expr* `,` … `)` | `call $min(0, 2)`
| `call_import` | `call_import` *name* `(`*expr* `,` … `)` | `call_import $max(0, 2)`
| `call_indirect` | `call_indirect` *signature-name* `[` *expr* `] (`*expr* `,` … `)` | `call_indirect $foo [1] (0, 2)`

### Arithmetic operators ([described here](AstSemantics#32-bit-integer-operators))

Arithmetic operators use C/JS-like infix and prefix notation.

Add, sub, mul, div, rem, and, or, xor, shl, and shr operators use
`+`, `-`, `*`, `/`, `%`, `&`, `|`, `^`, `<<`, and `>>`, respectively.

`eq`, `ne`, `lt`, `le`, `gt`, `ge`, operators use
`==`, `!=`, `<`, `<=`, `>`, `>=`, respectively.

`eqz` and `neg` operators use prefix unary `!` and `-`, respectively.

Signed and unsigned operators are suffixed with `s` and `u`, respectively,
so `i32.div_s` looks like `/s`, and so on.

These operators use C/JS-style operator precedence rules, except that in order
to correct [an old mistake](http://www.lysator.liu.se/c/dmr-on-or.html)),
it is not valid to rely on precedence alone between a `&`, `|`, or `^` operator
and another operator of similar precedence. Parentheses are required in such
contexts in order to ensure that the code reads as intended.

All other arithmetic operators use their actual name in a prefix notation,
such as `f32.sqrt(…)`, with comma-seperated operands.

## Module-level syntax

Type declarations:

`type $ternary of function (i32,i32,i32) : (i32)`

Imports:

`import "print" as $print_int from "global.PrintingStuff" typeof function (f64) : ()`

Function table:

`table [$func0,$func1,$func2,…]`

Exports:

`export memory as "memory"`

`export $func0 as "ThisIsFunc0"`

Function definitions and local variables:

```
function $func0($x : i32, $y : i32, $y : i32) : (f64) {
    var $a : i32, $b : i64, $c : f32
```

# Debug symbol integration

## Debug symbol section

The binary format inherently strips names from functions, locals, globals, etc,
reducing each of these to dense indices. Without help, the text format must
therefore synthesize new names. However, as part of the [tooling](Tooling.md)
story, a lightweight, optional "debug symbol" global section may be defined
which associates names with each indexed entity and, when present, these names
will be used in the text format projected from a binary WebAssembly module.

## Code references

In the binary format, references to locations in the code are done through
byte offsets. In the text format, these references are represented symbolically
with a special label syntax, prefixed by an `@` sign. These labels have no
semantic effect, and merely serve to identify a location in the code that can
be referred to from other sections.
