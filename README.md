# Embedded software architectures

All examples are implemented for Raspberry Pi Zero.


**Loop**

Processor checks each device in turn and services them as needed.

- No interrupts
- No shared data
- Simplest architecture
- Latency is limited by the maximum duration of a loop cycle

**Events**

Device set interrupt if needed action. Interrupt routines set
flags to indicate the interrupt happened. Main loop polls the
status of the interrupt flags and does any processing
required by a set flag.

- Shared data problem
- Greater control over the priority
- Fast response time for interrupt routines

**Functions queue**

Interrupt routines enqueue function pointers for follow-up work
onto a queue. Main loop just dequeues a pointer from the queue and
execute the function.

- Fast response time
- Shared data problem
- Great control over priority
- Reduce the worst-case response for the high-priority task
- Response time has good stability in the event of changes to the code

**Operating system**

User creates tasks. RTOS decide what task run next.
The RTOS can suspend one task subroutine in the middle
of processing in order to run another.

- Multitasking
- Task scheduler
- Every function can be interrupted
- Very short response time for high priority tasks
- Task synchronization and communication
- Stable against system modifications
