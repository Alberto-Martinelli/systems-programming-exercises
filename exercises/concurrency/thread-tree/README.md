# Recursive pthread tree

Create a binary thread tree and print the logical path of every leaf.

Concepts: Recursive concurrency; per-child argument storage; stack lifetime through joins; nondeterministic output order.

Build from the repository root with make, then run:

~~~sh
build/thread-tree 3
~~~

Depth 3 prints all eight paths from LLL through RRR, in scheduling-dependent order. Depth 0 prints root. Depth is limited to 6; logical paths avoid nonportable assumptions about pthread_t.

Changes from the coursework draft: Replaced fixed-size concatenated native thread IDs with bounded logical paths and per-child status propagation.

Validation: make check includes behavioral checks for this exercise. Original source mapping is in docs/provenance.md.
