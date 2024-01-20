# Fibers

`namespace fibers`

## Synchronization:
+ Lock-free `Mutex`
+ Lock-free One-Shot `Event`
+ Lock-free `WaitGroup` [Link](https://gobyexample.com/waitgroups) 
(represented as certain Event)

## Scheduling:
- `Spawn` (Spawn via executor | Spawn in Context)
- `Yield` (Suspend with rescheduling)
- `TeleportTo` (Switch scheduler after next stop)

## TODO
+ Structured
+ Fiber's SleepFor
+ Channel
+ Cancellation
+ ~~RPC