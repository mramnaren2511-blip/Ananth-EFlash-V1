#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <iomanip>
#include <cstdlib>
#include <sched.h>

#include "flash.h"

using namespace std;
using namespace std::chrono;

const int NUM_THREADS = 16; 
const int ALLOCATIONS_PER_THREAD = 10000000; // 10 Million ops per thread
const size_t ALLOC_SIZE = 64; 

atomic<uint64_t> total_allocations(0);

void tcmalloc_worker() {
    for (int i = 0; i < ALLOCATIONS_PER_THREAD; ++i) {
        void* ptr = malloc(ALLOC_SIZE);
        if (ptr) {
            *(volatile char*)ptr = 'A'; // Prevent compiler optimization (DCE)
            free(ptr);
        }
    }
    total_allocations.fetch_add(ALLOCATIONS_PER_THREAD, memory_order_relaxed);
}

void ananth_eflash_worker(int thread_id) {
    init_thread_worker();

    for (int i = 0; i < ALLOCATIONS_PER_THREAD; ++i) {
        void* ptr = eflash_malloc_core(ALLOC_SIZE); 
        if (ptr) {
            *(volatile char*)ptr = 'A';
            eflash_free_core(ptr);
        }
    }
    total_allocations.fetch_add(ALLOCATIONS_PER_THREAD, memory_order_relaxed);
}

template<typename Func>
void run_test(string name, Func thread_func) {
    total_allocations = 0;
    cout << ">> Benchmarking " << name << " (16 Threads)...\n";
    
    vector<thread> threads;
    auto start_time = high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(thread_func, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end_time = high_resolution_clock::now();
    duration<double> time_taken = end_time - start_time;

    double total_ops = total_allocations.load();
    double ops_per_sec = total_ops / time_taken.count();
    double latency_ns = (time_taken.count() * 1e9) / total_ops;

    cout << "   [RESULT] Throughput : " << fixed << setprecision(2) << (ops_per_sec / 1000000.0) << " Million Ops/sec\n";
    cout << "   [RESULT] L1 Latency : " << latency_ns << " ns / op\n";
    cout << "------------------------------------------------------\n";
}

int main() {
    cout << "\n======================================================\n";
    cout << "   HIGH-CONCURRENCY MEMORY ALLOCATION STRESS TEST\n";
    cout << "   Workload: 16 Threads | 160M Total Ops | 64-Byte Chunks\n";
    cout << "======================================================\n\n";

    run_test("Google TCMalloc / Baseline", [](int id) { tcmalloc_worker(); });  
    this_thread::sleep_for(seconds(2)); 
    init_eflash_boot_sequence(); 
    run_test("Ananth E-Flash V2.0 (Proprietary)", [](int id) { ananth_eflash_worker(id); });

    cout << "\n[PROCESS COMPLETED]\n\n";
    return 0;
}
