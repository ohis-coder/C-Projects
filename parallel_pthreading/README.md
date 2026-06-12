
# Cache-Isolated Multi-Threaded Data Pipeline

A high-performance concurrent pipeline written in C that demonstrates **Mechanical Sympathy** by surgically partitioning a unified heap structure to eliminate **False Sharing** across CPU cache lines.

Rather than relying on heavy, kernel-level mutex locks that force threads to sleep and waste cycles, this engine shifts the burden of synchronization from runtime code execution to **spatial memory layout**.

---

## The Core Problem: The False Sharing Trap

In traditional multi-threaded applications, developers often partition workloads by letting separate threads write to different indices of a single, shared array. Logically, this code is perfectly thread-safe because the indices do not overlap.

Mechanically, this layout triggers a hidden hardware bottleneck. Modern CPUs fetch memory from RAM into ultra-fast, private L1/L2 caches in fixed, contiguous blocks of **64 bytes (Cache Lines)**.

Because a standard 8-element integer array is only 32 bytes long, the CPU places the entire buffer onto a single tracking line. Every time Core 0 writes to its index, the hardware cache coherency protocol (MESI) violently invalidates that entire 64-byte chunk inside Core 1's private cache. Core 1 then stalls, forces a cache flush from Core 0, updates its index, and invalidates Core 0 right back.

The threads end up serializing each other on the hardware bus, playing a high-speed game of ping-pong with a single piece of silicon.

---

## The Architectural Solution

This project solves false sharing completely at compile-time by enforcing strict hardware alignment invariants inside the application's data models.

### 1. Sub-Struct Cache Line Partitioning

The master `packet` struct leverages GNU compiler attributes to align itself to a clean **256-byte boundary**, guaranteeing that its base heap address lands perfectly at the threshold of a fresh cache block. Internally, the destination targets for the threads are forced onto completely separate **64-byte cache lines**:

```c
typedef struct __attribute__((aligned(256))) {
  int upper_newlist[4] __attribute__((aligned(64))); // Private Cache Line for Thread 1
  int lower_newlist[4] __attribute__((aligned(64))); // Private Cache Line for Thread 2
  int upper[4];                                     // Contiguous Source Buffer
  int lower[4];                                     // Contiguous Source Buffer
} packet;

```

### 2. Zero-Synchronization Parallel Processing

Because `upper_newlist` and `lower_newlist` reside on firewalled tracks of silicon, **Thread 1** and **Thread 2** operate with absolute execution freedom. They require zero mutexes, zero spinlocks, and zero atomic loops while hammering their respective destinations.

### 3. Late-Stage Stitching Pattern

The ultimate synchronization is zero synchronization. The worker threads blast data into their isolated hardware sectors at raw CPU velocity. Once the threads hit their `pthread_join` checkpoints, a managing function uses precise pointer arithmetic offsets to stitch the distinct sectors together into a flat heap layout:

```c
memcpy(newlist, p->upper_newlist, 3 * sizeof(int));
memcpy(&newlist[3], p->lower_newlist, 3 * sizeof(int)); // Advances destination pointer by exactly 12 bytes

```

---

## Project Layout

```text
├── include/
│   └── thread.h          # Hardware alignment mappings & packet structure
├── src/
│   ├── thread.c        # Isolated thread execution & late-stage merging
│   └── main.c            # Execution orchestration entry point

```

---

## Getting Started

### Prerequisites

* A C compiler (`gcc` or `clang`) with support for GNU extension attributes.
* POSIX Threads library (`pthread`).

### Compilation

Compile the project with full optimization flags to allow the compiler to aggressively optimize instruction paths:

```bash
make

```


## Key Takeaways

* **Hardware Sympathy:** Software performance is directly bound by physical hardware constraints. An algorithm that respects the 64-byte grid will routinely outperform a theoretically superior algorithm that causes cache line thrashing.
* **Spatial Optimization:** Paying a microscopic `memcpy` performance tax at the very end of an execution chain is an massive win if it completely decouples your core multi-core processing loops from thread contention.

---

## License

This project is open-source and available under the MIT License.

---
