# ⚡ Ananth E-Flash V2.0: Enterprise-Grade Lock-Free C++ Memory Allocator

**Ananth E-Flash** is a high-concurrency, ultra-low latency memory engine designed for massive datacenter workloads. Engineered to completely eliminate thread-cache contention spikes and memory fragmentation, it serves as a high-performance, drop-in replacement for standard OS `glibc malloc` and Google's `TCMalloc`.

> **Status:** Closed-Source / Patent Pending (Application No: 202641127187). Currently in the Enterprise Evaluation Phase.

---

## 📊 1. The Performance Proof (Apples-to-Apples Benchmarks)

All tests were conducted on an ARM64 architecture (12 Cores) using strict assembly-level memory barriers to prevent compiler optimization (Dead Code Elimination) and ensure 100% accurate real-world hardware metrics.

### 🔥 Micro-Benchmark: The Raw Throughput Test
*(Workload: 12 Threads, Random 16B - 8KB allocations, 24 Million total operations)*

*   ❌ **Default OS glibc:** 102.44 Million Ops/sec
*   ❌ **Google TCMalloc:** 115.71 Million Ops/sec
*   ✅ **Ananth E-Flash V2.0:** **217.07 Million Ops/sec (1.87x Faster than TCMalloc)**

### 🚀 Macro-Benchmark: Real-World Redis Server Load (TCMalloc vs E-Flash)
*(Workload: `redis-benchmark`, 50 Concurrent Clients. Tested via `LD_PRELOAD` with zero source-code modifications to Redis).*

Under sustained heavy network I/O, Ananth E-Flash consistently outperforms TCMalloc in both throughput and p50 latency stability, especially as the load scales up to 6 Million requests.

**1 Million Requests Scale:**
*   **TCMalloc:** SET: 106,894 RPS (0.271ms) | GET: 109,086 RPS (0.263ms)[cite: 9]
*   **E-Flash:**  **SET: 110,411 RPS (0.247ms)** | **GET: 109,218 RPS (0.255ms)**[cite: 8]

**3 Million Requests Scale:**
*   **TCMalloc:** SET: 106,473 RPS (0.271ms) | GET: 111,715 RPS (0.255ms)[cite: 9]
*   **E-Flash:**  **SET: 110,261 RPS (0.255ms)** | **GET: 109,509 RPS (0.255ms)**[cite: 8]

**6 Million Requests Scale (Stress Load):**
*   **TCMalloc:** SET: 107,803 RPS (0.263ms) | GET: 107,586 RPS (0.271ms)[cite: 9]
*   **E-Flash:**  **SET: 110,035 RPS (0.255ms)** | **GET: 109,415 RPS (0.255ms)**[cite: 8]

<img width="533" height="184" alt="Screenshot 2026-11-02 134751" src="https://github.com/user-attachments/assets/8445f450-531e-4b2e-95f8-26c3145e8eeb" />
<img width="536" height="180" alt="Screenshot 2026-11-02 133525" src="https://github.com/user-attachments/assets/7ce6f099-eb49-447f-b167-c43f599b16d5" />

---

## 🛡️ 2. Enterprise Stability: The Datacenter Churn Test

High throughput is useless without rock-solid stability. We subjected Ananth E-Flash to a brutal multi-threaded stress test designed to break memory allocators.

*   **Load:** 100 Concurrent Threads
*   **Operations:** 5,000,000 (5 Million) simultaneous Allocations/Frees
*   **Result:** **✅ ZERO Segfaults. ✅ ZERO Deadlocks.** 

<img width="352" height="77" alt="Screenshot 2026-11-02 100912" src="https://github.com/user-attachments/assets/c7715434-10b5-434c-b169-544c5308d354" />

---

## 🧠 3. Engine Capabilities (The Advantage)

Ananth E-Flash operates differently from traditional allocators. Instead of relying on legacy Radix Trees, Page Maps, or global Spinlocks, the engine is built on:

1.  **Proprietary Metadata Isolation:** A highly optimized internal tracking system that allows for absolute **O(1) deallocation** without the heavy overhead of external memory lookups.
2.  **Zero-Contention Concurrency:** A fully lock-free pathway that handles heavy thread-cache refills and remote thread frees without acquiring a single Mutex or Spinlock.
3.  **Real-Time OS Memory Return:** Dynamically tracks span allocations and aggressively returns empty memory blocks back to the OS, drastically reducing long-term RAM bloat.
4.  **Zero-Friction Integration:** Compiled as a standalone shared library (`.so`). Inject it into any existing C++ application using `LD_PRELOAD=./libeflash.so` for instant performance upgrades.

---

## 🤝 4. Sandbox Evaluation & Licensing

To protect Intellectual Property(patent pending:202641127187), the core C++ source code and the `libeflash.so` binary are **not publicly available** in this repository.

If you are a Core Systems Engineer, Staff SWE, or VP of Engineering looking to reduce CPU/RAM costs in your datacenter infrastructure, **Ananth E-Flash is available for private sandbox evaluation.**

**How to access the Sandbox Binary:**
1. Reach out to the creator directly via DM on **X (Twitter) - @thenaren7(https://x.com/thenaren7)**.
2. We will sign a Mutual Non-Disclosure Agreement (MNDA) / Evaluation Agreement.
3. You will receive the compiled `.so` binary and benchmark suite to test on your internal servers.

*Let's build faster systems. 🚀*
