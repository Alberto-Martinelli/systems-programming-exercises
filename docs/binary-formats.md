# Original binary formats

## Student records

The record converter and random-access exercise read and write the native
record_t struct directly using sizeof(record_t). It contains an int record
number, a long registration number, two char[30] name fields, and an int mark.

Record size, padding, integer sizes, and byte order depend on the compiler and
platform. Random access uses sizeof(record_t) * (student_num - 1). The original
binary inputs are included unchanged; they use the layout of the system on
which they were created.

## File sorting

Each input*.bin starts with a native int count followed by that many native int
values. output.bin uses the same representation. The original code concatenates
per-file sorted arrays and sorts the combined sequence again.

No portable serialization or format validation has been added.
