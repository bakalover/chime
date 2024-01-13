# Fibers

`namespace fibers`

## Synchronization:
`namespace fibers::sync`
+ Lock-free Mutex
+ Lock-free One-Shot Event
+ Lock-free WaitGroup [Link](https://gobyexample.com/waitgroups) 
(represented as certain Event)

## Scheduling:
`namespace fibers::sched`
- `Spawn` (Spawn via certain executor)
- `Yield` (Suspend with rescheduling)
- `TeleportTo` (Switch scheduler after next stop)

## TODO
+ Structured
+ Fiber's SleepFor
+ Channel
+ Cancellation
+ ~~RPC