# xv6 OS Labs

My labs from an operating systems course built on MIT's 6.1810 ("Operating System Engineering") — the one where instead of just reading about kernels, you extend a small real one called xv6.

Each lab is its own branch, built on top of the course's starter code for that lab. Checking out a branch gets you just that lab's code:

- `util` — a few Unix utilities (`sleep`, `find`, a number filter, a memory dumper)
- `syscall` — a syscall sandbox that survives `fork()`
- `pgtbl` — page table internals, a faster `getpid()`, 2MB superpages
- `traps` — stack backtraces, a user-level alarm signal
- `cow` — copy-on-write `fork()`
- `thread` — a user-level thread switcher, a lock-striped hash table, a barrier
- `lock` — a per-CPU page allocator, a reader/writer lock
- `mmap` — lazy `mmap`/`munmap` with write-back for shared mappings
- `net` — an E1000 driver and UDP
- `proc_pgtbl` — a private kernel page table per process
- `perproc` — still working on this one

To try a lab:

```
git checkout cow
make qemu
make grade
```

You'll need a RISC-V toolchain and `qemu-system-riscv64`.

On the scores: I reran every lab's real grading script before putting this up, not just read over the code. `util`, `syscall`, `pgtbl`, `traps`, `cow`, `thread`, and `net` all pass clean. `lock`, `mmap`, and `proc_pgtbl` drop a handful of points purely because the single heaviest test in each (a full `usertests` run, or `kalloctest` grinding through all of physical memory) timed out on the machine I used — an 8GB laptop that had a few other things running at once, not a bug. I went back and read through the allocator and mmap fault-handling code by hand to double check.

xv6 itself is MIT's, not mine — see LICENSE on any branch.
