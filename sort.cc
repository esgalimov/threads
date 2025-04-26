#include <iostream>
#include <random>
#include <chrono>
#include "sort.hh"


int main(int argc, char* argv[]) {
    std::vector<int> vec{4, 5, 1, 2, 3, 4, 3, 1, 4, 7, 8, 9, 1, 2, 3, 6, 1};

    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> dis(0, 100000);

    for (int i = 0; i < 100000; ++i) {
        vec.push_back(dis(gen));
    }

    std::vector<int> vec2 = vec;

    std::vector<int> dest;
    dest.resize(vec.size());

    #ifdef dDEBUG
        for (auto& elem : vec) std::cout << elem << " ";
        std::cout << std::endl;
        std::cout << std::endl;
    #endif


    auto start = std::chrono::high_resolution_clock::now();

    pthread_t thr[parallel_sorter::NUM_THREADS];
    parallel_sorter::sort_pull_t sp[parallel_sorter::NUM_THREADS];
    
    for (int i = 0; i < parallel_sorter::NUM_THREADS; ++i) {
        sp[i].tid = i;

        sp[i].vec = &vec;
        sp[i].dest = &dest;
    }

    pthread_barrier_init(&parallel_sorter::barrier, NULL, parallel_sorter::NUM_THREADS);

    for (int i = 0; i < parallel_sorter::NUM_THREADS; ++i) {
        int ret_code = pthread_create(&thr[i], NULL, parallel_sorter::thread_sort, &sp[i]);

        if (ret_code) {
            std::cerr << "error: pthread_create, ret_code: " << ret_code << std::endl;
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < parallel_sorter::NUM_THREADS; ++i) {
        pthread_join(thr[i], NULL);
    }

    pthread_barrier_destroy(&parallel_sorter::barrier);

    if (parallel_sorter::my_log2(parallel_sorter::NUM_THREADS) % 2 == 0) vec.swap(dest);

    double tm = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
    
    std::cout << "PARRALEL SORT = " << tm << " us" << std::endl << std::endl;

    start = std::chrono::high_resolution_clock::now();

    std::sort(vec2.begin(), vec2.end());

    tm = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start).count();

    std::cout << "STD::SORT SORT = " << tm << " us" << std::endl << std::endl;

    auto it = dest.begin(), it2 = vec2.begin();

    while (it != dest.end()) {
        if (*it++ != *it2++) std::cout << "PROIZOSHLA PARASHA GOVHA" << std::endl;
    }


    #ifdef dDEBUG
        for (auto& elem : dest) std::cout << elem << " ";
        std::cout << std::endl;
    #endif

    return EXIT_SUCCESS;
}