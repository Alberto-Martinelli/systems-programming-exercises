# Template stack and queue

Implement LIFO and FIFO wrappers over STL containers, including move-only element ownership.

Concepts: Templates; vector and list; move semantics; const-correct queries; exceptions for invalid operations.

Build from the repository root with make, then run:

~~~sh
build/template-containers
~~~

Stack prints third, second, first; queue prints 1, 2, 3. Empty removal throws std::out_of_range. These containers are sequential and do not synchronize concurrent access.

Changes from the coursework draft: Added defined empty-removal behavior, size_t sizes, const queries, move-only support, and behavioral checks.

Validation: make check includes behavioral checks for this exercise. Original source mapping is in docs/provenance.md.
