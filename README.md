# Embedded software architectures

**Round Robin**
Processor checks each device in turn and services them as needed.

- simplest architecture
- no interrupts
- no shared data
- latency is limited by the maximum duration of a loop cycle

**Round Robin with interrupts**
Device set interrupt if needed action. Interrupt routines set 
flags to indicate the interrupt happened. Main loop polls the 
status of the interrupt flags and does any processing
required by a set flag.

- fast response time
- shared data problem

**Functions queue**
Interrupt routines enqueue function pointers for follow-up work
onto a queue. Main loop just dequeues a pointer from the queue and
execute the function.

- fast response time
- shared data problem
- great control over priority
- reduce the worst-case response for the high-priority task

**Operating system**


