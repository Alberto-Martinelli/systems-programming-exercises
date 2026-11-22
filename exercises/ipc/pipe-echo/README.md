# Streaming between parent and child through a pipe

A child copies standard input into a pipe; its parent drains the pipe to standard output.

Concepts: fork; descriptor ownership; EOF; short I/O; EINTR; waitpid; byte-stream semantics.

Build from the repository root with make, then run:

~~~sh
printf "hello\nworld\n" | build/pipe-echo
~~~

The transport preserves arbitrary bytes, including NUL and streams larger than the pipe buffer. Unused pipe ends are closed so EOF propagates. This is a byte stream, with no assumptions about message boundaries.

Changes from the coursework draft: Replaced unbounded scanf and string-based pipe reads with streaming byte buffers, complete writes, EOF termination, and child reaping.

Validation: make check includes behavioral checks for this exercise. Original source mapping is in docs/provenance.md.
