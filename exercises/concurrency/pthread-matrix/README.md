# Matrix multiplication with pthreads

Create one worker per output cell of a matrix product, then join all workers before printing.

Concepts: Shared read-only matrices; exclusive ownership of output cells; argument lifetime; heap allocation and cleanup.

Build from the repository root with make, then run:

~~~sh
build/pthread-matrix 2 2 3
~~~

A and B contain consecutive positive integers in row-major order. The sample prints rows "9 12 15" and "19 26 33". Dimensions are bounded to 1..32 and output cells to 128. This intentionally demonstrates decomposition, without claiming that a thread per cell is efficient.

Changes from the coursework draft: Added configurable dimensions, contiguous matrix allocations, complete cleanup, deterministic data, and cleanup after partial thread creation.

Validation: make check includes behavioral checks for this exercise. Original source mapping is in docs/provenance.md.
