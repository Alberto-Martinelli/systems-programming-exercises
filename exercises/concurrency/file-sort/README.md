# Concurrent binary-file sorting and merging

Use one C++ worker per input file to read and sort its integers; join workers and merge the sorted runs.

Concepts: C++20 std::thread with scope-bound joins; worker exception propagation; per-worker ownership; binary I/O; RAII; priority-queue merging.

Build from the repository root with make, then run:

~~~sh
python3 examples/make_sort_inputs.py build/sort-inputs
build/file-sort build/sort-inputs/input1.bin build/sort-inputs/input2.bin build/sort-inputs/input3.bin build/sorted.bin
~~~

Each input contains a little-endian uint32 count followed by signed 32-bit integers. Empty runs, duplicates, and negative values are supported. Limit: 32 input files and one million values per file. The merge uses O(N log K) time for K runs, after worker sorting; it keeps input and output values in memory.

Changes from the coursework draft: Replaced raw owning arrays with vectors, hardcoded paths with CLI arguments, concatenate-and-resort with a k-way merge, and unchecked reads with format validation. Implements the wait-for-all variant.

Validation: make check includes behavioral checks for this exercise. Original source mapping is in docs/provenance.md.
