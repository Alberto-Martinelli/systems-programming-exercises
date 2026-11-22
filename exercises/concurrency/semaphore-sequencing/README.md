# Semaphore-controlled output sequencing

Coordinate three symbol workers and a newline worker to print ordered groups of plus, minus, and star characters.

Concepts: C++20 binary semaphores; token handoff; acknowledgment; finite termination; safe startup.

Build from the repository root with make, then run:

~~~sh
build/semaphore-sequencing 2 4
~~~

Prints "++++", "----", and "****" on separate lines, repeating twice. Both cycle count and width range from 1 to 1000. Ordering follows semaphore handoffs and requires no sleeps.

Changes from the coursework draft: Added finite cycles, configurable width, a startup gate for thread-creation failure, and RAII joins. Removed timing-dependent delays.

Validation: make check includes behavioral checks for this exercise. Original source mapping is in docs/provenance.md.
