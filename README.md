# Concurrency framework 
(Gratitude to Roman Lipovsky)

Doing concurrency for fun (or not...)

## Intrusive Executors

- Manual (for testing purposes / deterministic runtime)
- Pool (just thread pool for heavy computations)
- Strand aka Async - Mutex (currently bad for lifetimes)

## Fibers
+ Manual start (TODO: structured concurrency with Runtime)
+ Synchronization:
    + Lock-free Mutex
    + Lock-free One-Shot Event
    + Lock-free WaitGroup (certain Event)

- Scheduling:
    - SpawnVia (spawn via certain executor)
    - Yield (suspend with rescheduling)

## TODO Queue
+ Lazy futures
+ Fiber's SleepFor via futures 
+ Schedulers
+ Name for project))
+ Channel
+ Cancellation
+ Structured Runtime
+ RPC communication between actors
+ Oh my, what will happen here...

