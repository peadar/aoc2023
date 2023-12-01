#include <chrono>
#include <functional>
#include <iostream>

#include <signal.h>
#include <unistd.h>

void
timeit(std::function<void(void)> f) {
    std::cerr << "running payload...";
    static volatile bool done;
    done = false;
    signal(SIGALRM, [] (int) { done = true; } );
    alarm(5);
    auto start = std::chrono::high_resolution_clock::now();
    unsigned long long iters;
    for (iters = 0; !done; ++iters)
        f();
    auto end = std::chrono::high_resolution_clock::now();
    auto micros = std::chrono::duration_cast<std::chrono::microseconds>(
          end - start).count();
    double secondsfloat = double(micros) / 1000000;
    auto persec = double(iters) / double(secondsfloat);
    auto perop = double(micros) / double(iters);
    std::cout << "time taken: " << micros << "us, iterations=" << iters
       << ", operations per sec=" << (unsigned long long)(persec)
       << ", usec/operation: " << perop << "\n";
}
