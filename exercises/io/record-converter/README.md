# Text and binary record conversion

Convert validated text records into an explicit fixed-size binary representation, and back.

Concepts: Open file descriptors versus FILE streams; short reads and writes; fixed-width serialization; metadata checks.

Build from the repository root with make, then run:

~~~sh
build/record-converter to-binary examples/students.txt build/students.bin
build/record-converter to-text build/students.bin build/students-copy.txt
diff examples/students.txt build/students-copy.txt
~~~

IDs must be sequential from 1. Names contain 1..30 non-whitespace bytes; registrations have six digits; marks are 0..30. Integer encoding is little endian. The shared format is documented in docs/binary-formats.md. An error can leave partial output; conversion is not a transactional file replacement.

Changes from the coursework draft: Replaced hardcoded paths, native-struct serialization, and unchecked I/O with validated CLI arguments, explicit encoding, and checked resource cleanup.

Validation: make check includes behavioral checks for this exercise. Original source mapping is in docs/provenance.md.
