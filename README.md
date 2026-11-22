# Systems Programming Exercises

11 C/C++ coursework exercises covering threads, processes, concurrency, binary
I/O, file access, and memory ownership. Based on Operating Systems and System
and Device Programming coursework at Politecnico di Torino.

| Exercise | Concepts |
| --- | --- |
| [Record conversion](exercises/io/record-converter/) | Text/binary I/O, serialization |
| [Random record access](exercises/io/record-access/) | File descriptors, `lseek`, offsets |
| [Matrix multiplication](exercises/concurrency/pthread-matrix/) | Pthreads, shared memory, allocation |
| [Thread tree](exercises/concurrency/thread-tree/) | Recursive thread creation and joins |
| [Concurrent file sorting](exercises/concurrency/file-sort/) | C++ threads, RAII, k-way merge |
| [Future pipeline](exercises/concurrency/future-pipeline/) | Promises, futures, exception propagation |
| [Semaphore sequencing](exercises/concurrency/semaphore-sequencing/) | Synchronization and execution order |
| [Pipe echo](exercises/ipc/pipe-echo/) | `fork`, pipes, EOF, `waitpid` |
| [Signal handshake](exercises/processes/signal-handshake/) | Signal masks, `sigwait` |
| [Process tree](exercises/processes/process-tree/) | Process creation and child reaping |
| [Template containers](exercises/cpp/template-containers/) | STL, templates, move semantics |

## Build and run

Requires Linux or macOS, Make, C11/C++20 compilers, and Python 3.9+ for tests.

```sh
make -j2
make check
build/pthread-matrix 2 2 3
build/semaphore-sequencing 2 4
```

Each exercise includes usage notes. See [binary formats](docs/binary-formats.md)
and [source provenance and revisions](docs/provenance.md) for details.
