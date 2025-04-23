#ifndef PAR_SORT
#define PAR_SORT

#include <pthread.h>
#include <vector>
#include <algorithm>

namespace parallel_sorter {

using vec_it_t = typename std::vector<int>::iterator;

struct sort_pull_t final {
    vec_it_t begin;
    vec_it_t end;
};

struct merge_pull_t final {
    vec_it_t src_begin1;
    vec_it_t src_end1;

    vec_it_t src_begin2;
    vec_it_t src_end2;

    vec_it_t dst_begin;
    vec_it_t dst_end;
};


void* thread_sort(void* arg) {
    sort_pull_t* data = static_cast<sort_pull_t*>(arg);

    // for (auto it = data->begin; it != data->end; ++it) {
    //     std::cout << *it << std::endl;
    // }
    // std::cout << std::endl;

    std::sort(data->begin, data->end);
    pthread_exit(NULL);
}

void* thread_merge(void* arg) {
    pthread_exit(NULL);
}

}; // <-- namespace parallel_sorter
#endif