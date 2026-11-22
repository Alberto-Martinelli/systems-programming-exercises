# Parent and child signal handshake

Alternate parent and child messages using SIGUSR1 and SIGUSR2 as a request/acknowledgment protocol.

Concepts: Blocked signal masks inherited through fork; synchronous sigwait; signal delivery; child reaping.

Build from the repository root with make, then run:

~~~sh
build/signal-handshake 3
~~~

Prints PARENT 1, CHILD 1, and so on in strict order. Signals are blocked before fork, so an early notification remains pending until sigwait. One acknowledgment per request avoids relying on standard-signal queuing. This teaching example assumes both participants remain alive during the handshake.

Changes from the coursework draft: Replaced signal-plus-pause and sleeps with blocked signals and sigwait; added finite rounds and waitpid. No I/O runs in an asynchronous signal handler.

Validation: make check includes behavioral checks for this exercise. Original source mapping is in docs/provenance.md.
