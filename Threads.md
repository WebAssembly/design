# Threads

This page describes a proposal for the post-MVP
[threads feature][future threads].

## Modifications to [Portability.md](Portability.md)

...

Only 32-bit atomic accesses are guaranteed to be lock-free. This matches the
[ECMAScript specification](https://tc39.github.io/ecma262/#sec-atomics.islockfree).

* Availability of lock-free atomic memory operators, when naturally aligned,
  for 32-bit accesses. At a minimum this must include an atomic
  compare-and-exchange operator (or equivalent load-linked/store-conditional).

...

## Modifications to [Semantics.md](Semantics.md)

...

If the effective address of a memory access is a multiple of the alignment
attribute value of the memory access, the memory access is considered
*aligned*, otherwise it is considered *misaligned*.

Misaligned atomic accesses trap. For non-atomic accesses, aligned and
misaligned accesses have the same behavior.

Alignment of non-atomic accesses affects performance as follows:

...

## Additions to [Semantics.md](Semantics.md)

### Shared Linear Memory

A Linear memory can be marked as shared, which allows it to be shared between
threads of execution. The shared memory can be imported or defined in the
module. It is a validation error to attempt to import shared linear
memory if the module's memory import doesn't specify that it allows shared
memory.

### Atomic Memory Accesses

Atomic memory accesses are separated into three categories, load/store,
read-modify-write, and compare-exchange. All atomic memory accesses require a
shared linear memory. Attempting to use atomic access operators on non-shared
linear memory is a validation error.

Currently all atomic memory accesses are [sequentially
consistent](https://en.wikipedia.org/wiki/Sequential_consistency). This
restriction may be relaxed in the future.

Atomic load/store memory accesses behave like their non-atomic counterparts,
with the exception that the ordering of accesses is sequentially consistent.

  * `i32.atomic.load8_s`: atomically load 1 byte and sign-extend i8 to i32
  * `i32.atomic.load8_u`: atomically load 1 byte and zero-extend i8 to i32
  * `i32.atomic.load16_s`: atomically load 2 bytes and sign-extend i16 to i32
  * `i32.atomic.load16_u`: atomically load 2 bytes and zero-extend i16 to i32
  * `i32.atomic.load`: atomically load 4 bytes as i32
  * `i64.atomic.load8_s`: atomically load 1 byte and sign-extend i8 to i64
  * `i64.atomic.load8_u`: atomically load 1 byte and zero-extend i8 to i64
  * `i64.atomic.load16_s`: atomically load 2 bytes and sign-extend i16 to i64
  * `i64.atomic.load16_u`: atomically load 2 bytes and zero-extend i16 to i64
  * `i64.atomic.load32_s`: atomically load 4 bytes and sign-extend i32 to i64
  * `i64.atomic.load32_u`: atomically load 4 bytes and zero-extend i32 to i64
  * `i64.atomic.load`: atomically load 8 bytes as i64
  * `f32.atomic.load`: atomically load 4 bytes as f32
  * `f64.atomic.load`: atomically load 8 bytes as f64
  * `i32.atomic.store8`: wrap i32 to i8 and atomically store 1 byte
  * `i32.atomic.store16`: wrap i32 to i16 and atomically store 2 bytes
  * `i32.atomic.store`: (no conversion) atomically store 4 bytes
  * `i64.atomic.store8`: wrap i64 to i8 and atomically store 1 byte
  * `i64.atomic.store16`: wrap i64 to i16 and atomically store 2 bytes
  * `i64.atomic.store32`: wrap i64 to i32 and atomically store 4 bytes
  * `i64.atomic.store`: (no conversion) atomically store 8 bytes
  * `f32.atomic.store`: (no conversion) atomically store 4 bytes
  * `f64.atomic.store`: (no conversion) atomically store 8 bytes

Atomic read-modify-write (RMW) operators atomically read a value from an
address , modify the value, and store the resulting value to the same address.
All RMW operators return the value read from memory before the modify operation
was performed.

The RMW operators have two operands, an address and a value used in the modify
operation.

The sign-agnostic operations are further described
[below](Semantics.md#32-bit-integer-operators).

| Name | Read (as `read`) | Modify | Write | Return `read` |
| ---- | ---- | ---- | ---- | ---- |
| `i32.atomic.rmw8_s.add` | 1 byte | 8-bit sign-agnostic addition | 1 byte | sign-extended i8 to i32 |
| `i32.atomic.rmw8_u.add` | 1 byte | 8-bit sign-agnostic addition | 1 byte | zero-extended i8 to i32 |
| `i32.atomic.rmw16_s.add` | 2 bytes | 16-bit sign-agnostic addition | 2 bytes | sign-extended i16 to i32 |
| `i32.atomic.rmw16_u.add` | 2 bytes | 16-bit sign-agnostic addition | 2 bytes | zero-extended i16 to i32 |
| `i32.atomic.rmw.add` | 4 bytes | 32-bit sign-agnostic addition | 4 bytes | as i32 |
| `i64.atomic.rmw8_s.add` | 1 byte | 8-bit sign-agnostic addition | 1 byte | sign-extended i8 to i64 |
| `i64.atomic.rmw8_u.add` | 1 byte | 8-bit sign-agnostic addition | 1 byte | zero-extended i8 to i64 |
| `i64.atomic.rmw16_s.add` | 2 bytes | 16-bit sign-agnostic addition | 2 bytes | sign-extended i16 to i64 |
| `i64.atomic.rmw16_u.add` | 2 bytes | 16-bit sign-agnostic addition | 2 bytes | zero-extended i16 to i64 |
| `i64.atomic.rmw32_s.add` | 4 bytes | 32-bit sign-agnostic addition | 4 bytes | sign-extended i32 to i64 |
| `i64.atomic.rmw32_u.add` | 4 bytes | 32-bit sign-agnostic addition | 4 bytes | zero-extended i32 to i64 |
| `i64.atomic.rmw.add` | 8 bytes | 64-bit sign-agnostic addition | 8 bytes | as i64 |
| `i32.atomic.rmw8_s.sub` | 1 byte | 8-bit sign-agnostic subtraction | 1 byte | sign-extended i8 to i32 |
| `i32.atomic.rmw8_u.sub` | 1 byte | 8-bit sign-agnostic subtraction | 1 byte | zero-extended i8 to i32 |
| `i32.atomic.rmw16_s.sub` | 2 bytes | 16-bit sign-agnostic subtraction | 2 bytes | sign-extended i16 to i32 |
| `i32.atomic.rmw16_u.sub` | 2 bytes | 16-bit sign-agnostic subtraction | 2 bytes | zero-extended i16 to i32 |
| `i32.atomic.rmw.sub` | 4 bytes | 32-bit sign-agnostic subtraction | 4 bytes | as i32 |
| `i64.atomic.rmw8_s.sub` | 1 byte | 8-bit sign-agnostic subtraction | 1 byte | sign-extended i8 to i64 |
| `i64.atomic.rmw8_u.sub` | 1 byte | 8-bit sign-agnostic subtraction | 1 byte | zero-extended i8 to i64 |
| `i64.atomic.rmw16_s.sub` | 2 bytes | 16-bit sign-agnostic subtraction | 2 bytes | sign-extended i16 to i64 |
| `i64.atomic.rmw16_u.sub` | 2 bytes | 16-bit sign-agnostic subtraction | 2 bytes | zero-extended i16 to i64 |
| `i64.atomic.rmw32_s.sub` | 4 bytes | 32-bit sign-agnostic subtraction | 4 bytes | sign-extended i32 to i64 |
| `i64.atomic.rmw32_u.sub` | 4 bytes | 32-bit sign-agnostic subtraction | 4 bytes | zero-extended i32 to i64 |
| `i64.atomic.rmw.sub` | 8 bytes | 64-bit sign-agnostic subtraction | 8 bytes | as i64 |
| `i32.atomic.rmw8_s.and` | 1 byte | 8-bit sign-agnostic bitwise and | 1 byte | sign-extended i8 to i32 |
| `i32.atomic.rmw8_u.and` | 1 byte | 8-bit sign-agnostic bitwise and | 1 byte | zero-extended i8 to i32 |
| `i32.atomic.rmw16_s.and` | 2 bytes | 16-bit sign-agnostic bitwise and | 2 bytes | sign-extended i16 to i32 |
| `i32.atomic.rmw16_u.and` | 2 bytes | 16-bit sign-agnostic bitwise and | 2 bytes | zero-extended i16 to i32 |
| `i32.atomic.rmw.and` | 4 bytes | 32-bit sign-agnostic bitwise and | 4 bytes | as i32 |
| `i64.atomic.rmw8_s.and` | 1 byte | 8-bit sign-agnostic bitwise and | 1 byte | sign-extended i8 to i64 |
| `i64.atomic.rmw8_u.and` | 1 byte | 8-bit sign-agnostic bitwise and | 1 byte | zero-extended i8 to i64 |
| `i64.atomic.rmw16_s.and` | 2 bytes | 16-bit sign-agnostic bitwise and | 2 bytes | sign-extended i16 to i64 |
| `i64.atomic.rmw16_u.and` | 2 bytes | 16-bit sign-agnostic bitwise and | 2 bytes | zero-extended i16 to i64 |
| `i64.atomic.rmw32_s.and` | 4 bytes | 32-bit sign-agnostic bitwise and | 4 bytes | sign-extended i32 to i64 |
| `i64.atomic.rmw32_u.and` | 4 bytes | 32-bit sign-agnostic bitwise and | 4 bytes | zero-extended i32 to i64 |
| `i64.atomic.rmw.and` | 8 bytes | 64-bit sign-agnostic bitwise and | 8 bytes | as i64 |
| `i32.atomic.rmw8_s.or` | 1 byte | 8-bit sign-agnostic bitwise inclusive or | 1 byte | sign-extended i8 to i32 |
| `i32.atomic.rmw8_u.or` | 1 byte | 8-bit sign-agnostic bitwise inclusive or | 1 byte | zero-extended i8 to i32 |
| `i32.atomic.rmw16_s.or` | 2 bytes | 16-bit sign-agnostic bitwise inclusive or | 2 bytes | sign-extended i16 to i32 |
| `i32.atomic.rmw16_u.or` | 2 bytes | 16-bit sign-agnostic bitwise inclusive or | 2 bytes | zero-extended i16 to i32 |
| `i32.atomic.rmw.or` | 4 bytes | 32-bit sign-agnostic bitwise inclusive or | 4 bytes | as i32 |
| `i64.atomic.rmw8_s.or` | 1 byte | 8-bit sign-agnostic bitwise inclusive or | 1 byte | sign-extended i8 to i64 |
| `i64.atomic.rmw8_u.or` | 1 byte | 8-bit sign-agnostic bitwise inclusive or | 1 byte | zero-extended i8 to i64 |
| `i64.atomic.rmw16_s.or` | 2 bytes | 16-bit sign-agnostic bitwise inclusive or | 2 bytes | sign-extended i16 to i64 |
| `i64.atomic.rmw16_u.or` | 2 bytes | 16-bit sign-agnostic bitwise inclusive or | 2 bytes | zero-extended i16 to i64 |
| `i64.atomic.rmw32_s.or` | 4 bytes | 32-bit sign-agnostic bitwise inclusive or | 4 bytes | sign-extended i32 to i64 |
| `i64.atomic.rmw32_u.or` | 4 bytes | 32-bit sign-agnostic bitwise inclusive or | 4 bytes | zero-extended i32 to i64 |
| `i64.atomic.rmw.or` | 8 bytes | 64-bit sign-agnostic bitwise inclusive or | 8 bytes | as i64 |
| `i32.atomic.rmw8_s.xor` | 1 byte | 8-bit sign-agnostic bitwise exclusive or | 1 byte | sign-extended i8 to i32 |
| `i32.atomic.rmw8_u.xor` | 1 byte | 8-bit sign-agnostic bitwise exclusive or | 1 byte | zero-extended i8 to i32 |
| `i32.atomic.rmw16_s.xor` | 2 bytes | 16-bit sign-agnostic bitwise exclusive or | 2 bytes | sign-extended i16 to i32 |
| `i32.atomic.rmw16_u.xor` | 2 bytes | 16-bit sign-agnostic bitwise exclusive or | 2 bytes | zero-extended i16 to i32 |
| `i32.atomic.rmw.xor` | 4 bytes | 32-bit sign-agnostic bitwise exclusive or | 4 bytes | as i32 |
| `i64.atomic.rmw8_s.xor` | 1 byte | 8-bit sign-agnostic bitwise exclusive or | 1 byte | sign-extended i8 to i64 |
| `i64.atomic.rmw8_u.xor` | 1 byte | 8-bit sign-agnostic bitwise exclusive or | 1 byte | zero-extended i8 to i64 |
| `i64.atomic.rmw16_s.xor` | 2 bytes | 16-bit sign-agnostic bitwise exclusive or | 2 bytes | sign-extended i16 to i64 |
| `i64.atomic.rmw16_u.xor` | 2 bytes | 16-bit sign-agnostic bitwise exclusive or | 2 bytes | zero-extended i16 to i64 |
| `i64.atomic.rmw32_s.xor` | 4 bytes | 32-bit sign-agnostic bitwise exclusive or | 4 bytes | sign-extended i32 to i64 |
| `i64.atomic.rmw32_u.xor` | 4 bytes | 32-bit sign-agnostic bitwise exclusive or | 4 bytes | zero-extended i32 to i64 |
| `i64.atomic.rmw.xor` | 8 bytes | 64-bit sign-agnostic bitwise exclusive or | 8 bytes | as i64 |
| `i32.atomic.rmw8_s.xchg` | 1 byte | nop | 1 byte | sign-extended i8 to i32 |
| `i32.atomic.rmw8_u.xchg` | 1 byte | nop | 1 byte | zero-extended i8 to i32 |
| `i32.atomic.rmw16_s.xchg` | 2 bytes | nop | 2 bytes | sign-extended i16 to i32 |
| `i32.atomic.rmw16_u.xchg` | 2 bytes | nop | 2 bytes | zero-extended i16 to i32 |
| `i32.atomic.rmw.xchg` | 4 bytes | nop | 4 bytes | as i32 |
| `i64.atomic.rmw8_s.xchg` | 1 byte | nop | 1 byte | sign-extended i8 to i64 |
| `i64.atomic.rmw8_u.xchg` | 1 byte | nop | 1 byte | zero-extended i8 to i64 |
| `i64.atomic.rmw16_s.xchg` | 2 bytes | nop | 2 bytes | sign-extended i16 to i64 |
| `i64.atomic.rmw16_u.xchg` | 2 bytes | nop | 2 bytes | zero-extended i16 to i64 |
| `i64.atomic.rmw32_s.xchg` | 4 bytes | nop | 4 bytes | sign-extended i32 to i64 |
| `i64.atomic.rmw32_u.xchg` | 4 bytes | nop | 4 bytes | zero-extended i32 to i64 |
| `i64.atomic.rmw.xchg` | 8 bytes | nop | 8 bytes | as i64 |

The atomic compare exchange operators take three operands: an address, an
`expected` value, and a `replacement` value. If the `loaded` value is equal to
the `expected` value, the `replacement` value is stored to the same memory
address. If the values are not equal, no value is stored. In either case, the
`loaded` value is returned.

| Name | Load (as `loaded`) | Compare `expected` with `loaded` | Conditionally Store `replacement` | Return `loaded` |
| ---- | ---- | ---- | ---- | ---- |
| `i32.atomic.rmw8_s.cmpxchg` | 1 byte | `expected` wrapped from i32 to i8, 8-bit compare equal | wrapped from i32 to i8, store 1 byte | sign-extended from i8 to i32 |
| `i32.atomic.rmw8_u.cmpxchg` | 1 byte | `expected` wrapped from i32 to i8, 8-bit compare equal | wrapped from i32 to i8, store 1 byte | zero-extended from i8 to i32 |
| `i32.atomic.rmw16_s.cmpxchg` | 2 bytes | `expected` wrapped from i32 to i16, 16-bit compare equal | wrapped from i32 to i16, store 2 bytes | sign-extended from i8 to i32 |
| `i32.atomic.rmw16_u.cmpxchg` | 2 bytes | `expected` wrapped from i32 to i16, 16-bit compare equal | wrapped from i32 to i16, store 2 bytes | zero-extended from i8 to i32 |
| `i32.atomic.rmw.cmpxchg` | 4 bytes | 32-bit compare equal | store 4 bytes | as i32 |
| `i64.atomic.rmw8_s.cmpxchg` | 1 byte | `expected` wrapped from i64 to i8, 8-bit compare equal | wrapped from i64 to i8, store 1 byte | sign-extended from i8 to i64 |
| `i64.atomic.rmw8_u.cmpxchg` | 1 byte | `expected` wrapped from i64 to i8, 8-bit compare equal | wrapped from i64 to i8, store 1 byte | zero-extended from i8 to i64 |
| `i64.atomic.rmw16_s.cmpxchg` | 2 bytes | `expected` wrapped from i64 to i16, 16-bit compare equal | wrapped from i64 to i16, store 2 bytes | sign-extended from i16 to i64 |
| `i64.atomic.rmw16_u.cmpxchg` | 2 bytes | `expected` wrapped from i64 to i16, 16-bit compare equal | wrapped from i64 to i16, store 2 bytes | zero-extended from i16 to i64 |
| `i64.atomic.rmw32_s.cmpxchg` | 4 bytes | `expected` wrapped from i64 to i32, 32-bit compare equal | wrapped from i64 to i32, store 4 bytes | sign-extended from i32 to i64 |
| `i64.atomic.rmw32_u.cmpxchg` | 4 bytes | `expected` wrapped from i64 to i32, 32-bit compare equal | wrapped from i64 to i32, store 4 bytes | zero-extended from i32 to i64 |
| `i64.atomic.rmw.cmpxchg` | 8 bytes | 64-bit compare equal | 8 bytes | as i64 |

### Thread operators

  * `is_lock_free`: Given an operand `N` of type `i32`, if the atomic step of
    an atomic primitive (see [Atomic Memory Accesses](#atomic-memory-accesses))
    on a datum of size `N` bytes will be performed without the [agent](agent)
    acquiring a lock outside the `N` bytes comprising the datum, then return
    `1`. Otherwise, return `0`. Once the value of `is_lock_free` for a given
    value `N` has been observed for any [agent](agent) in an
    [agent cluster](agent cluster), it cannot change.

#### Wait and Wake operators

The wake and wait operators are optimizations over busy-waiting for a value to
change. It is a validation error to use these operators on non-shared linear
memory. They are non-atomic and have sequentially consistent memory ordering.

Both wake and wait operators trap if the effective address of either operator
is misaligned or out-of-bounds.

The embedder is also permitted to suspend or wake a thread. A suspended thread
can be woken by the embedder or the wake operator, regardless of how the thread
was suspended (e.g. via the embedder or a wait operator).

#### Wait

The wait operator take three operands: an address operand, an expected value,
and a relative timeout in milliseconds as an `f64`. The return value is `0`,
`1`, or `2`, returned as an `i32`.

| `timeout` value | Behavior |
| ---- | ---- |
| `timeout` <= 0 | Expires immediately |
| 0 < `timeout` < Positive infinity | Expires after `timeout` milliseconds |
| Positive infinity | Never expires |
| NaN | Never expires |

| Return value | Description |
| ---- | ---- |
| `0` | "ok", woken by another thread or the embedder |
| `1` | "not-equal", the loaded value did not match the expected value |
| `2` | "timed-out", not woken before timeout expired |

The wait operation begins by performing an atomic load from the given address.
If the loaded value is not equal to the expected value, the operator returns 1
("not-equal"). If the values are equal, the thread is suspended. If the thread
is woken, the wait operator returns 0 ("ok"). If the timeout expires before
another thread wakes this one, this operator returns 2 ("timed-out").

  * `i32.wait`: load i32 value, compare to expected (as `i32`), and wait for wake at same address
  * `i64.wait`: load i64 value, compare to expected (as `i64`), and wait for wake at same address

## Additions to [BinaryEncoding.md](BinaryEncoding.md)

### Thread operators ([described here](Semantics.md#thread-operators))

| Name | Opcode | Immediate | Description |
| ---- | ---- | ---- | ---- |
| `is_lock_free` | `0xff00` | | |
| `wake` | `0xff01` | `memory immediate` | |
| `i32.wait` | `0xff02` | `memory immediate` | |
| `i64.wait` | `0xff03` | `memory immediate` | |

The `memory_immediate` type is encoded as follows:

| Name | Type | Description |
| ---- | ---- | ---- |
| flags | `varuint32` | a bitfield which currently must be 0 |
| offset | `varuint32` | the value of the offset |

### Atomic read-modify-write operators ([described here](Semantics.md#atomic-memory-accesses))

| Name | Opcode | Immediates | Description |
| ---- | ---- | ---- | ---- |
| `i32.atomic.rmw.add` | `0xff10` | `0` `memory immediate` | atomic add to memory |
| `i64.atomic.rmw.add` | `0xff11` | `0` `memory immediate` | atomic add to memory |
| `i32.atomic.rmw8_s.add` | `0xff12` | `0` `memory immediate` | atomic add to memory |
| `i32.atomic.rmw8_u.add` | `0xff13` | `0` `memory immediate` | atomic add to memory |
| `i32.atomic.rmw16_s.add` | `0xff14` | `0` `memory immediate` | atomic add to memory |
| `i32.atomic.rmw16_u.add` | `0xff15` | `0` `memory immediate` | atomic add to memory |
| `i64.atomic.rmw8_s.add` | `0xff16` | `0` `memory immediate` | atomic add to memory |
| `i64.atomic.rmw8_u.add` | `0xff17` | `0` `memory immediate` | atomic add to memory |
| `i64.atomic.rmw16_s.add` | `0xff18` | `0` `memory immediate` | atomic add to memory |
| `i64.atomic.rmw16_u.add` | `0xff19` | `0` `memory immediate` | atomic add to memory |
| `i64.atomic.rmw32_s.add` | `0xff1a` | `0` `memory immediate` | atomic add to memory |
| `i64.atomic.rmw32_u.add` | `0xff1b` | `0` `memory immediate` | atomic add to memory |
| `i32.atomic.rmw.sub` | `0xff10` | `1` `memory immediate` | atomic sub from memory |
| `i64.atomic.rmw.sub` | `0xff11` | `1` `memory immediate` | atomic sub from memory |
| `i32.atomic.rmw8_s.sub` | `0xff12` | `1` `memory immediate` | atomic sub from memory |
| `i32.atomic.rmw8_u.sub` | `0xff13` | `1` `memory immediate` | atomic sub from memory |
| `i32.atomic.rmw16_s.sub` | `0xff14` | `1` `memory immediate` | atomic sub from memory |
| `i32.atomic.rmw16_u.sub` | `0xff15` | `1` `memory immediate` | atomic sub from memory |
| `i64.atomic.rmw8_s.sub` | `0xff16` | `1` `memory immediate` | atomic sub from memory |
| `i64.atomic.rmw8_u.sub` | `0xff17` | `1` `memory immediate` | atomic sub from memory |
| `i64.atomic.rmw16_s.sub` | `0xff18` | `1` `memory immediate` | atomic sub from memory |
| `i64.atomic.rmw16_u.sub` | `0xff19` | `1` `memory immediate` | atomic sub from memory |
| `i64.atomic.rmw32_s.sub` | `0xff1a` | `1` `memory immediate` | atomic sub from memory |
| `i64.atomic.rmw32_u.sub` | `0xff1b` | `1` `memory immediate` | atomic sub from memory |
| `i32.atomic.rmw.and` | `0xff10` | `2` `memory immediate` | atomic and with memory |
| `i64.atomic.rmw.and` | `0xff11` | `2` `memory immediate` | atomic and with memory |
| `i32.atomic.rmw8_s.and` | `0xff12` | `2` `memory immediate` | atomic and with memory |
| `i32.atomic.rmw8_u.and` | `0xff13` | `2` `memory immediate` | atomic and with memory |
| `i32.atomic.rmw16_s.and` | `0xff14` | `2` `memory immediate` | atomic and with memory |
| `i32.atomic.rmw16_u.and` | `0xff15` | `2` `memory immediate` | atomic and with memory |
| `i64.atomic.rmw8_s.and` | `0xff16` | `2` `memory immediate` | atomic and with memory |
| `i64.atomic.rmw8_u.and` | `0xff17` | `2` `memory immediate` | atomic and with memory |
| `i64.atomic.rmw16_s.and` | `0xff18` | `2` `memory immediate` | atomic and with memory |
| `i64.atomic.rmw16_u.and` | `0xff19` | `2` `memory immediate` | atomic and with memory |
| `i64.atomic.rmw32_s.and` | `0xff1a` | `2` `memory immediate` | atomic and with memory |
| `i64.atomic.rmw32_u.and` | `0xff1b` | `2` `memory immediate` | atomic and with memory |
| `i32.atomic.rmw.or` | `0xff10` | `3` `memory immediate` | atomic or with memory |
| `i64.atomic.rmw.or` | `0xff11` | `3` `memory immediate` | atomic or with memory |
| `i32.atomic.rmw8_s.or` | `0xff12` | `3` `memory immediate` | atomic or with memory |
| `i32.atomic.rmw8_u.or` | `0xff13` | `3` `memory immediate` | atomic or with memory |
| `i32.atomic.rmw16_s.or` | `0xff14` | `3` `memory immediate` | atomic or with memory |
| `i32.atomic.rmw16_u.or` | `0xff15` | `3` `memory immediate` | atomic or with memory |
| `i64.atomic.rmw8_s.or` | `0xff16` | `3` `memory immediate` | atomic or with memory |
| `i64.atomic.rmw8_u.or` | `0xff17` | `3` `memory immediate` | atomic or with memory |
| `i64.atomic.rmw16_s.or` | `0xff18` | `3` `memory immediate` | atomic or with memory |
| `i64.atomic.rmw16_u.or` | `0xff19` | `3` `memory immediate` | atomic or with memory |
| `i64.atomic.rmw32_s.or` | `0xff1a` | `3` `memory immediate` | atomic or with memory |
| `i64.atomic.rmw32_u.or` | `0xff1b` | `3` `memory immediate` | atomic or with memory |
| `i32.atomic.rmw.xor` | `0xff10` | `4` `memory immediate` | atomic xor with memory |
| `i64.atomic.rmw.xor` | `0xff11` | `4` `memory immediate` | atomic xor with memory |
| `i32.atomic.rmw8_s.xor` | `0xff12` | `4` `memory immediate` | atomic xor with memory |
| `i32.atomic.rmw8_u.xor` | `0xff13` | `4` `memory immediate` | atomic xor with memory |
| `i32.atomic.rmw16_s.xor` | `0xff14` | `4` `memory immediate` | atomic xor with memory |
| `i32.atomic.rmw16_u.xor` | `0xff15` | `4` `memory immediate` | atomic xor with memory |
| `i64.atomic.rmw8_s.xor` | `0xff16` | `4` `memory immediate` | atomic xor with memory |
| `i64.atomic.rmw8_u.xor` | `0xff17` | `4` `memory immediate` | atomic xor with memory |
| `i64.atomic.rmw16_s.xor` | `0xff18` | `4` `memory immediate` | atomic xor with memory |
| `i64.atomic.rmw16_u.xor` | `0xff19` | `4` `memory immediate` | atomic xor with memory |
| `i64.atomic.rmw32_s.xor` | `0xff1a` | `4` `memory immediate` | atomic xor with memory |
| `i64.atomic.rmw32_u.xor` | `0xff1b` | `4` `memory immediate` | atomic xor with memory |
| `i32.atomic.rmw.xchg` | `0xff10` | `5` `memory immediate` | atomic exchange with memory |
| `i64.atomic.rmw.xchg` | `0xff11` | `5` `memory immediate` | atomic exchange with memory |
| `i32.atomic.rmw8_s.xchg` | `0xff12` | `5` `memory immediate` | atomic exchange with memory |
| `i32.atomic.rmw8_u.xchg` | `0xff13` | `5` `memory immediate` | atomic exchange with memory |
| `i32.atomic.rmw16_s.xchg` | `0xff14` | `5` `memory immediate` | atomic exchange with memory |
| `i32.atomic.rmw16_u.xchg` | `0xff15` | `5` `memory immediate` | atomic exchange with memory |
| `i64.atomic.rmw8_s.xchg` | `0xff16` | `5` `memory immediate` | atomic exchange with memory |
| `i64.atomic.rmw8_u.xchg` | `0xff17` | `5` `memory immediate` | atomic exchange with memory |
| `i64.atomic.rmw16_s.xchg` | `0xff18` | `5` `memory immediate` | atomic exchange with memory |
| `i64.atomic.rmw16_u.xchg` | `0xff19` | `5` `memory immediate` | atomic exchange with memory |
| `i64.atomic.rmw32_s.xchg` | `0xff1a` | `5` `memory immediate` | atomic exchange with memory |
| `i64.atomic.rmw32_u.xchg` | `0xff1b` | `5` `memory immediate` | atomic exchange with memory |

The `memory_immediate` type is encoded as follows:

| Name | Type | Description |
| ---- | ---- | ---- |
| flags | `varuint32` | a bitfield which currently must be 0 |
| offset | `varuint32` | the value of the offset |

### Atomic compare exchange operators ([described here](Semantics.md#atomic-memory-accesses))

| Name | Opcode | Immediate | Description |
| ---- | ---- | ---- | ---- |
| `i32.atomic.rmw.cmpxchg` | `0xff1c` | `memory immediate` | atomic compare exchange with memory |
| `i64.atomic.rmw.cmpxchg` | `0xff1d` | `memory immediate` | atomic compare exchange with memory |
| `i32.atomic.rmw8_s.cmpxchg` | `0xff1e` | `memory immediate` | atomic compare exchange with memory |
| `i32.atomic.rmw8_u.cmpxchg` | `0xff1f` | `memory immediate` | atomic compare exchange with memory |
| `i32.atomic.rmw16_s.cmpxchg` | `0xff20` | `memory immediate` | atomic compare exchange with memory |
| `i32.atomic.rmw16_u.cmpxchg` | `0xff21` | `memory immediate` | atomic compare exchange with memory |
| `i64.atomic.rmw8_s.cmpxchg` | `0xff22` | `memory immediate` | atomic compare exchange with memory |
| `i64.atomic.rmw8_u.cmpxchg` | `0xff23` | `memory immediate` | atomic compare exchange with memory |
| `i64.atomic.rmw16_s.cmpxchg` | `0xff24` | `memory immediate` | atomic compare exchange with memory |
| `i64.atomic.rmw16_u.cmpxchg` | `0xff25` | `memory immediate` | atomic compare exchange with memory |
| `i64.atomic.rmw32_s.cmpxchg` | `0xff26` | `memory immediate` | atomic compare exchange with memory |
| `i64.atomic.rmw32_u.cmpxchg` | `0xff27` | `memory immediate` | atomic compare exchange with memory |

The `memory_immediate` type is encoded as follows:

| Name | Type | Description |
| ---- | ---- | ---- |
| flags | `varuint32` | a bitfield which currently must be 0 |
| offset | `varuint32` | the value of the offset |

### Atomic load/store operators ([described here](Semantics.md#atomic-memory-accesses))

| Name | Opcode | Immediate | Description |
| ---- | ---- | ---- | ---- |
| `i32.atomic.load` | `0xff28` | `memory immediate` | atomic load from memory |
| `i64.atomic.load` | `0xff29` | `memory immediate` | atomic load from memory |
| `f32.atomic.load` | `0xff2a` | `memory immediate` | atomic load from memory |
| `f64.atomic.load` | `0xff2b` | `memory immediate` | atomic load from memory |
| `i32.atomic.load8_s` | `0xff2c` | `memory immediate` | atomic load from memory |
| `i32.atomic.load8_u` | `0xff2d` | `memory immediate` | atomic load from memory |
| `i32.atomic.load16_s` | `0xff2e` | `memory immediate` | atomic load from memory |
| `i32.atomic.load16_u` | `0xff2f` | `memory immediate` | atomic load from memory |
| `i64.atomic.load8_s` | `0xff30` | `memory immediate` | atomic load from memory |
| `i64.atomic.load8_u` | `0xff31` | `memory immediate` | atomic load from memory |
| `i64.atomic.load16_s` | `0xff32` | `memory immediate` | atomic load from memory |
| `i64.atomic.load16_u` | `0xff33` | `memory immediate` | atomic load from memory |
| `i64.atomic.load32_s` | `0xff34` | `memory immediate` | atomic load from memory |
| `i64.atomic.load32_u` | `0xff35` | `memory immediate` | atomic load from memory |
| `i32.atomic.store` | `0xff36` | `memory immediate` | atomic store to memory |
| `i64.atomic.store` | `0xff37` | `memory immediate` | atomic store to memory |
| `f32.atomic.store` | `0xff38` | `memory immediate` | atomic store to memory |
| `f64.atomic.store` | `0xff39` | `memory immediate` | atomic store to memory |
| `i32.atomic.store8` | `0xff3a` | `memory immediate` | atomic store to memory |
| `i32.atomic.store16` | `0xff3b` | `memory immediate` | atomic store to memory |
| `i64.atomic.store8` | `0xff3c` | `memory immediate` | atomic store to memory |
| `i64.atomic.store16` | `0xff3d` | `memory immediate` | atomic store to memory |
| `i64.atomic.store32` | `0xff3e` | `memory immediate` | atomic store to memory |

The `memory_immediate` type is encoded as follows:

| Name | Type | Description |
| ---- | ---- | ---- |
| flags | `varuint32` | a bitfield which currently must be 0 |
| offset | `varuint32` | the value of the offset |

[agent]: https://tc39.github.io/ecma262/#sec-agents
[agent cluster]: https://tc39.github.io/ecma262/#sec-agent-clusters
[future threads]: FutureFeatures.md#threads
