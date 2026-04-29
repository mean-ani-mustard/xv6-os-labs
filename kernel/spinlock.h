// Mutual exclusion lock.
struct spinlock {
  uint locked;

  char *name;
  struct cpu *cpu;

#ifdef LAB_LOCK
  int nts;
  int n;
#endif
};

#ifdef LAB_LOCK
// Reader-writer lock.
struct rwspinlock {
  struct spinlock lock;
  int readers;
  int writer;
  int waiting_writers;
};
#endif