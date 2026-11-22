# Random access to binary records

Read, update, or append a record by its one-based position using lseek.

Concepts: Descriptor offsets; file size and inode metadata; in-place updates; record bounds.

Build from the repository root with make, then run:

~~~sh
build/record-converter to-binary examples/students.txt build/students.bin
build/record-access read build/students.bin 2
build/record-access write build/students.bin 2 100002 Example Bob 28
build/record-access read build/students.bin 2
~~~

Use the converter format. Writes may replace an existing record or append exactly one record; gaps are rejected. Updates are intended for one process at a time and do not implement locking or crash recovery.

Changes from the coursework draft: Replaced the interactive draft with explicit subcommands, bounded arguments, checked offsets, shared serialization, and corruption checks.

Validation: make check includes behavioral checks for this exercise. Original source mapping is in docs/provenance.md.
