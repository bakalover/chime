# Futures

`namespace futures`

## Creating
- `Contract`\
Represents contract between Producer as `Promise` and Consumer as `EagerFuture`
- `Value`\
Immediate value
- `Submit`\
Lazy submittion in executor
- `Failure`\
Immediate error
- `Just aka Value<Unit>`

## Parallel composition
+ `Both` -\> Tuple / First error
+ `First` -\> First result / Last Error

## Sequential composition
- `AndThen`\
Future\<T\> -> (T -> Result\<U\>) -> Future\<U\>
- `OrElse`\
Future\<T\> -> (Error -> Result\<T\>) -> Future\<T\>
- `Map`\
Future\<T\> -> (T -> U) -> Future\<U\>
- `Flatten`\
Future\<Future\<T\>\> -> Future\<T\>
- `FlatMap`\
Future\<T\> -> (T -> Future\<U\>) -> Future\<U\>
- `Via`\
Future\<T\> -> Executor -> Future\<T\>
- `Start`\
Future\<T\> -> EagerFuture\<T\>
- `Box`\
Future\<T\> -> BoxedFuture\<T\>
## Chain running:
+ `Get`\
Blocking on getting result
+ `Detach`\
Consume future with running aka non-returning `Get`


## TODO
+ Timer Future
+ Variadic  First and  Both <-> All
+ Cancellation