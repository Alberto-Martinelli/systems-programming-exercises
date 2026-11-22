# Binary formats

Both formats define byte order and field sizes explicitly. They do not serialize
C pointers, native long values, or struct padding. They differ deliberately from
the machine-dependent layouts in the original coursework drafts.

## Student records

A file is a sequence of 74-byte records with no header.

| Offset | Bytes | Meaning |
| --- | --- | --- |
| 0 | 4 | One-based record ID, unsigned little endian |
| 4 | 4 | Six-digit registration, unsigned little endian |
| 8 | 31 | Surname, NUL terminated, zero padded when written |
| 39 | 31 | Name, NUL terminated, zero padded when written |
| 70 | 4 | Mark from 0 through 30, unsigned little endian |

The converter requires IDs 1, 2, 3, ... in file order. Names contain at most 30
non-whitespace bytes; limits count bytes rather than Unicode characters. The text
representation is ID REGISTRATION SURNAME NAME MARK with one record per line.
Reading an incomplete record or a name without a terminator is an error.
Random-access offsets are (INDEX - 1) * 74.

## Integer sorting files

The first four bytes contain an unsigned little-endian count. Exactly that many
signed 32-bit little-endian integers follow. The complete length must equal
4 + 4 * count bytes. A zero count represents an empty sequence.

The output uses the same format, with values in ascending order. Header counts
are not payload values. examples/make_sort_inputs.py generates sample inputs.
