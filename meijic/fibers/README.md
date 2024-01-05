# Fibers
## Synchronization:
`namespace fibers::sync`
+ Lock-free Mutex
+ Lock-free One-Shot Event
+ Lock-free WaitGroup (certain Event)

## Scheduling:
`namespace fibers::sched`
- `SpawnVia` (spawn via certain executor)
- `Yield` (suspend with rescheduling)
- `TeleportTo` (switch schedulers after next stop)

## TODO
+ Structured
+ Fiber's SleepFor
+ Channel
+ Cancellation
+ ~~RPC