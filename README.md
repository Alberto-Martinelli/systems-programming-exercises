# Systems Programming Exercises

11 C/C++ coursework exercises from Operating Systems and System and Device
Programming at Politecnico di Torino. Source files are preserved exactly as in
the original study folders, with a Makefile and topic-based organization.

| Exercise | Concepts |
| --- | --- |
| [Record conversion](exercises/io/record-converter/) | Text and binary file I/O |
| [Random record access](exercises/io/record-access/) | File descriptors, lseek, offsets |
| [Matrix multiplication](exercises/concurrency/pthread-matrix/) | Pthreads, shared memory, allocation |
| [Thread tree](exercises/concurrency/thread-tree/) | Recursive thread creation and joins |
| [Concurrent file sorting](exercises/concurrency/file-sort/) | C++ threads, binary I/O, sorting |
| [Future pipeline](exercises/concurrency/future-pipeline/) | Promises and futures |
| [Semaphore sequencing](exercises/concurrency/semaphore-sequencing/) | Synchronization and execution order |
| [Pipe communication](exercises/ipc/pipe-echo/) | fork, pipes, parent/child communication |
| [Signal synchronization](exercises/processes/signal-handshake/) | Signals, pause, process coordination |
| [Process tree](exercises/processes/process-tree/) | Process creation |
| [Template containers](exercises/cpp/template-containers/) | Templates and STL containers |

## Build

Requires Make and C11/C++20 compilers with thread and semaphore support.

```sh
make -j2
build/pthread-matrix
build/future-pipeline
```

Each exercise includes usage notes. Hardcoded inputs, warnings, and original
runtime behavior are preserved. Compilation was verified with Apple Clang 15 on
macOS; runtime correctness is not guaranteed.

See [source provenance](docs/provenance.md) and [binary formats](docs/binary-formats.md).
