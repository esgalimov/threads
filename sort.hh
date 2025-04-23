#ifndef PAR_SORT
#define PAR_SORT

#include <pthread.h>
#include <vector>
#include <algorithm>

namespace parallel_sorter {

struct sort_pull_t final {
    using vec_it_t = typename std::vector<int>::iterator;
    vec_it_t begin;
    vec_it_t end;
};


void* thread_sort(void* arg) {
    sort_pull_t* data = static_cast<sort_pull_t*>(arg);

    for (auto it = data->begin; it != data->end; ++it) {
        std::cout << *it << std::endl;
    }
    std::cout << std::endl;

    std::sort(data->begin, data->end);
}

}; // <-- namespace parallel_sorter
#endif