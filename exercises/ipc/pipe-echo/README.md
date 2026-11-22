# Parent/child pipe communication

A child reads strings interactively and writes them to a pipe; its parent prints what it reads. Both loops run indefinitely and preserve the original EOF behavior.

Build with make from the repository root, then run:

```sh
build/pipe-echo
```

Source code is unchanged from the original study folder.
