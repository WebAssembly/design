# `Unchecked`

## Problem statement
We want to limit nondeterminism in WebAssembly. At the same time, we want
WebAssembly to be performant. The overflow behavior of arithmetic operators
(including bit-shift operators) has been previously identified as a source of
tension between these two constraints: different CPU architectures provide
different behavior for the exceptional case (i.e. out of bounds), which means
that, in order to offer consistent behavior, we need to generate boundary
conditions checks, which potentially hurts performance.

## Proposal
Provide consistent behavior by default, but give the developer opt-out control,
should their performance scenario be hurt by the extra boundary checks.

For C/C++ developers, this is achieved by marking functions with the `unchecked`
attribute (a new attribute introduced for this purpose).

This proposal does not prescribe how this is rendered in bytecode. There are at
least 2 possibilities: (1) unchecked variants of pertinent opcodes; or (2) a new
operator which gives unchecked semantics to all pertinent operators used in its
scope.
