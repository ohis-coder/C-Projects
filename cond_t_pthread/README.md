# C Concurrency: Thread Coordination & State Machines

A collection of high-performance C programs demonstrating advanced thread orchestration using POSIX Condition Variables (`pthread_cond_t`) and Mutexes.

## Programs Overview

### 1. Bitwise Turn-Taking (`num & 1`)
Demonstrates **Logical Concurrency**. Two threads increment a shared counter in a strict "Ping-Pong" fashion by monitoring the Least Significant Bit (LSB).
- **Thread A**: Operates only when `num` is Even.
- **Thread B**: Operates only when `num` is Odd.
- **Key Concept**: Using data state as a flow-control mechanism.

### 2. Status-Code State Machine (`whistle`)
Implements a **Producer-Consumer** pattern. Threads communicate via a "Whistle" (Condition Variable) to signal when a specific system status has been reached.
- **Thread B (Resetter)**: Sets the system status to `0` and signals the waiter.
- **Thread A (Logger)**: Waits for the `0` status, processes the event, and resets the state to `99`.
- **Key Concept**: Preventing "Busy Waiting" by putting threads to sleep until specific hardware or software conditions are met.

## Systems Architecture Notes
- **Spurious Wakeup Protection**: All condition waits are wrapped in `while` loops to ensure the predicate is re-verified upon wakeup.
- **Atomicity**: Guarantees that state transitions (`0 -> 99`) are indivisible.


## Compilation
```bash
gcc -O3 main.c -o thread_sync -pthread
