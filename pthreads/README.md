# Multi-Threaded Atomic Counter in C

A lightweight concurrent C program demonstrating thread coordination and race-condition prevention using the POSIX Threads (`pthreads`) API.

## Architecture & Design
This program spawns two independent concurrent worker threads (`Thread A` and `Thread B`) executing over a shared memory resource (`num`). 

To eliminate data races and prevent CPU register collision during the read-modify-write sequence (`num++`), a **Mutual Exclusion Lock (Mutex)** is implemented. This guarantees that only one thread can modify the critical section at any single microsecond.

## How to Compile and Run

### Prerequisites
You need a Unix-like environment (Linux or macOS) with `gcc` or `clang` installed.

### Compilation
Compile the program via the terminal. You **must** pass the `-pthread` flag to link the POSIX threads system library:

```bash
gcc -O3 main.c -o atomic_counter -pthread
```

### Execution
Run the compiled binary:

```bash
./atomic_counter
```

### Expected Output
```text
Before calling worker threads to work on NUM its value is: 0
Ready to Increase 0
Done increasing Thread A
Done increasing Thread B
The Final Value of NUM after both worker threads incrementing is: 20
```
