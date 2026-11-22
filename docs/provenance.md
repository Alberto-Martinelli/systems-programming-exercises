# Coursework provenance and revisions

The selected drafts were copied from the local study folders listed below, then revised inside this repository. The source files were not edited. Hashes identify the exact original bytes used, rather than the revised files.

Course context: Operating Systems and System and Device Programming, Politecnico di Torino. Source paths are relative to the original Uni directory.

Only selected drafts under my_exercises and laboratory/specifications were used. The laboratory/solutions directory, handouts, exam PDFs, binaries, IDE files, and generated build files were excluded. A folder name alone does not establish authorship; this map records origins without assigning authorship to supplied snippets. Check the coursework origins before choosing a public license.

## exercises/io/record-converter/main.c

Original: 5 Fifth year/System and Device Programming/Stefano-Quer/laboratory/specifications/lab01/ex01v2/main.c

SHA-256: f87cc931c69d79767245d6f3caf1a09b6a957339b2e821aa042865a0373cfa68

Revisions: Explicit portable record encoding, validated arguments and records, complete I/O, resource cleanup.

## exercises/io/record-access/main.c

Original: 5 Fifth year/System and Device Programming/Stefano-Quer/laboratory/specifications/lab01/ex02/main.c

SHA-256: 20af828e70625d2838608e4e122ee13e156df15592d08303086c4676db9889c5

Revisions: CLI reads/writes, checked offsets and file sizes, bounded append, shared record format.

## exercises/concurrency/pthread-matrix/main.c

Original: 5 Fifth year/System and Device Programming/Stefano-Quer/laboratory/specifications/lab02/ex2/main.c

SHA-256: ca8c565d0c4efeaf2cd36b69e46031cf5df8904cb08d5babb55a5e8836b7f4a4

Revisions: Configurable dimensions, contiguous allocations, deterministic inputs, ownership and cleanup.

## exercises/concurrency/thread-tree/main.c

Original: 5 Fifth year/System and Device Programming/Stefano-Quer/laboratory/specifications/lab02/ex1/main.c

SHA-256: a9226d99c3c8ae10c7f72454977a9398ad79708f9594d9dae508d1fbf7894a8a

Revisions: Logical paths, bounded depth, distinct child arguments, error propagation and joins.

## exercises/concurrency/file-sort/main.cpp

Original: 5 Fifth year/System and Device Programming/Stefano-Quer/laboratory/specifications/lab04/ex4/main.cpp

SHA-256: ceba06baabddb104dcde1cecc13ba6a863a81cc193a1af62e6c25e2308fdacee

Revisions: RAII workers and storage, checked binary format, propagated exceptions, true k-way merge.

## exercises/concurrency/future-pipeline/main.cpp

Original: 5 Fifth year/System and Device Programming/Stefano-Quer/my_exercises/u06s09e02/main.cpp

SHA-256: ce9390e8dd907126d39c4651d5c0eebd58185ae840bf576dd83130353c7f92f5

Revisions: Validated integers, overflow-safe primality, exceptions passed between stages, RAII threads.

## exercises/concurrency/semaphore-sequencing/main.cpp

Original: 5 Fifth year/System and Device Programming/Stefano-Quer/my_exercises/u06s05e02/main.cpp

SHA-256: 3e81830073414016404f0568d0c71ac0336ab5bca9f2d3dfa21ae898d0465ac3

Revisions: Finite cycles, configurable width, startup gates, semaphore-only ordering.

## exercises/ipc/pipe-echo/main.c

Original: 3 Third year/Operating systems/my_exercises/quiz/quiz_u04/ex2/main.c

SHA-256: d2f0a18980cb4fa1631f6745b31d5cfcbb072dd953009d6a9510a933d389801d

Revisions: Arbitrary byte streaming, complete writes, EOF handling, descriptor cleanup, child reaping.

## exercises/processes/signal-handshake/main.c

Original: 3 Third year/Operating systems/my_exercises/Labs/Lab04/lab04e03/lab04e03.c

SHA-256: 413079cacb1f36be98891f46709a3aa5f6027c767306b080bfe194b92751acc7

Revisions: Blocked signals plus sigwait, finite handshake, child reaping.

## exercises/processes/process-tree/main.c

Original: 5 Fifth year/System and Device Programming/Stefano-Quer/laboratory/specifications/lab01/ex06/main.c

SHA-256: 366e804079fb77b3c0ac328ee049df8d8d30fa72898b6763241fc9c4f4bcca2b

Revisions: Bounded inputs, logical leaf paths, recursive child reaping.

## exercises/cpp/template-containers/main.cpp

Original: 5 Fifth year/System and Device Programming/Stefano-Quer/laboratory/specifications/lab03/ex1/main.cpp

SHA-256: b1bd60985b476708e7d3dd08be08ae9407a255d16574723ecc6d2be7fc494345

Revisions: Empty-removal exceptions, const queries, size_t, move-only element support.

## exercises/cpp/template-containers/stack.h

Original: 5 Fifth year/System and Device Programming/Stefano-Quer/laboratory/specifications/lab03/ex1/stack.h

SHA-256: 4b0cd94c7b0a2e17e4f62e9f2af550e105696029acca33bc7c22ed5f71b15cda

Revisions: Empty-removal exceptions, const queries, size_t, move-only element support.

## exercises/cpp/template-containers/queue.h

Original: 5 Fifth year/System and Device Programming/Stefano-Quer/laboratory/specifications/lab03/ex1/queue.h

SHA-256: f8a3222c923efe25639d022182aca5a853701d6f93e4f4d38287d8d0bea7bdd8

Revisions: Empty-removal exceptions, const queries, size_t, move-only element support.

## New supporting material

The Makefile, shared headers in include/, behavioral checks, sample-data generator, synthetic student fixture, and repository documentation were added during this reorganization. Existing exercise implementations were substantially revised; this is a curated, repaired edition of study material, rather than an untouched snapshot of the coursework.
