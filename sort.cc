#include <iostream>
#include "sort.hh"

constexpr int NUM_THREADS = 4;

int main(int argc, char* argv[]) {
    std::cout << "HELLO" << std::endl;

    std::vector<int> vec{4, 5, 1, 2, 3};

    parallel_sorter::sort_pull_t sp;

    sp.begin = vec.begin();
    sp.end = vec.end();

    parallel_sorter::thread_sort(&sp);

    for (auto& elem : vec) std::cout << elem << std::endl;



    // pthread_t thr[NUM_THREADS]; int i, rc;
    // thread_data_t thr_data[NUM_THREADS];

    // for(i = 0; i< NUM_THREADS; ++i) {
    //     thr_data[i].tid = i;
    //     if((rc = pthread_create(&thr[i], NULL, thr_func, &thr_data[i]))) {
    //         fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
    //         return EXIT_FAILURE;
    //     }
    // }
    //     for(i = 0; i< NUM_THREADS; ++i) {
    //     pthread_join(thr[i], NULL);
    // }
    // return EXIT_SUCCESS;



    return 0;
}