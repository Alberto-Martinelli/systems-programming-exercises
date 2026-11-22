# Concurrent file sorting

Reads the three included input*.bin files, sorts each in its own thread, concatenates the arrays, sorts the combined array, and writes output.bin.

Build with make from the repository root, then run:

```sh
(cd exercises/concurrency/file-sort && ../../../build/file-sort)
```

Source code is unchanged from the original study folder.
