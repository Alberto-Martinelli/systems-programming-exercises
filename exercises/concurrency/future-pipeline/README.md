# Promise and future pipeline

Pass an integer from a producer to a primality worker, then pass the boolean result to a consumer.

Concepts: Three threads; single-result communication; blocking future.get; exception transport; joining and visibility.

Build from the repository root with make, then run:

~~~sh
build/future-pipeline 17
build/future-pipeline 2147483647
~~~

Accepts a signed 32-bit integer. Invalid input travels through the promise/future chain as an exception. Primality checking uses division to avoid overflow in i*i.

Changes from the coursework draft: Replaced unchecked interactive input with validated arguments, added exception propagation and RAII joins, and fixed the integer-overflow boundary.

Validation: make check includes behavioral checks for this exercise. Original source mapping is in docs/provenance.md.
