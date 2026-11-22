# Bounded process tree

Create a process tree with configurable height and branching factor, printing each leaf path.

Concepts: fork return values; independent address spaces; recursive process creation; _exit; waitpid.

Build from the repository root with make, then run:

~~~sh
build/process-tree 2 2
~~~

Prints 0.0, 0.1, 1.0, and 1.1 in scheduling-dependent order. Height is 0..4 and branching factor is 1..4. Every parent reaps its own children before it returns.

Changes from the coursework draft: Replaced fixed parameters and early parent exits with bounded CLI parameters, explicit tree structure, and cleanup at every level.

Validation: make check includes behavioral checks for this exercise. Original source mapping is in docs/provenance.md.
