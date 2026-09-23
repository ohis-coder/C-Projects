# High-Performance Concurrent Rate Limiters in C

A production-grade, thread-safe implementation of the **Token Bucket** (burst-tolerant rate limiting) and **Leaky Bucket** (smooth traffic shaping) algorithms written in pure C.

## The Architecture: O(1) Lazy Evaluation
Most naive implementations of rate limiters spawn background daemon threads to constantly tick a clock and update bucket volumes. If a system scales to 500,000 active users, running 500,000 background threads triggers context-switching thrashing and exhausts system memory.

This implementation uses **Lazy Evaluation**:
* **Zero Background Threads:** Buckets remain cold and silent in memory when traffic stops.
* **On-Demand Math:** When an incoming request thread executes `_check()`, it dynamically calculates the elapsed time delta (Δ t), derives the mathematical state update instantly, and commits it.
* **O(1) Time & Space Complexity:** Maximizes CPU efficiency and scales gracefully across millions of concurrent users.

## Thread Safety & Memory Integrity
* **Pthread Mutex Protection:** Every bucket structure encapsulates its own `pthread_mutex_t` guard to isolate memory access across parallel CPU cores.
* **Local State Normalization:** To eliminate race conditions and middle-state memory corruption, all mathematical accumulation, leakage, and capacity clamping are performed entirely within thread-local stack variables (CPU registers) before being committed to the shared structure.

## 🛠️ How to Compile and Run

To compile the implementation with multi-threading support and compiler optimizations enabled, execute:

```bash
gcc -O2 token_bucket.c leaky_bucket.c -pthread -o rate_limiter
```
