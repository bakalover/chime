# Concurrency framework based on Roman Lipovsky's course

Ort basis for concurrency

TODO: Weave together to General Runtime

## Intrusive Executors

- Manual (for testing purposes / deterministic runtime)
- Pool (just thread pool for heavy computations)
- Strand aka Async - Mutex

## Fibers
+ Manual start (TODO: structured concurrency with Runtime)
+ Synchronization:
    + Lock-free Mutex
    + Lock-free One-Shot Event
    + Lock-free WaitGroup (certain Event)

- Scheduling:
    - SpawnVia (spawn via certain executor)
    - Yield (suspend with rescheduling)

