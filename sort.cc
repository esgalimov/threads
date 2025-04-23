#include <iostream>
#include "sort.hh"

constexpr int NUM_THREADS = 4;

static_assert(!(NUM_THREADS == 0) && !(NUM_THREADS & (NUM_THREADS - 1)), "NUM_THREADS must be power of 2 ");

int main(int argc, char* argv[]) {
    std::vector<int> vec{4, 5, 1, 2, 3, 4, 3, 1, 4};

    #ifdef DEBUG
        for (auto& elem : vec) std::cout << elem << " ";
        std::cout << std::endl;
        std::cout << std::endl;
    #endif

    parallel_sorter::sort_pull_t sp[NUM_THREADS];
    int thr_vec_size = vec.size() / NUM_THREADS;
    auto it = vec.begin();

    for (int i = 0; i < NUM_THREADS; ++i) {
        sp[i].begin = it;

        if (i != NUM_THREADS - 1) {
            it += thr_vec_size;
            sp[i].end = it;
        } else sp[i].end = vec.end();
    } 

    #ifdef DEBUG
        for (int i = 0; i < NUM_THREADS; ++i) {
            for (auto it = sp[i].begin; it != sp[i].end; ++it) {
                std::cout << *it << " ";
            }
            std::cout << std::endl;
        }
    #endif
    
    pthread_t thr[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
        int ret_code = pthread_create(&thr[i], NULL, parallel_sorter::thread_sort, &sp[i]);

        if (ret_code) {
            std::cerr << "error: pthread_create, ret_code: " << ret_code << std::endl;
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        std::cout << thr[i] << std::endl;
        pthread_join(thr[i], NULL);
    }

    #ifdef DEBUG
        for (auto& elem : vec) std::cout << elem << " ";
        std::cout << std::endl;
    #endif

    return EXIT_SUCCESS;
}