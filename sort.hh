#ifndef PAR_SORT
#define PAR_SORT

#include <pthread.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>

namespace parallel_sorter {

constexpr int NUM_THREADS = 4;

static_assert(!(NUM_THREADS == 0) && !(NUM_THREADS & (NUM_THREADS - 1)), "NUM_THREADS must be power of 2 ");

pthread_barrier_t barrier;

using vec_it_t = typename std::vector<int>::iterator;

struct sort_pull_t final {
    std::vector<int>* vec;
    std::vector<int>* dest;

    int tid;
};

int my_log2(int x) {
    int cnt = 0;
    while (x >>= 1) cnt++;
    return cnt;
}

void* thread_sort(void* arg) {
    sort_pull_t* data = static_cast<sort_pull_t*>(arg);

    int thr_vec_size = std::distance(data->vec->begin(), data->vec->end()) / NUM_THREADS;

    auto begin = data->vec->begin() + thr_vec_size * data->tid;
    auto end = data->tid != NUM_THREADS - 1 ? begin + thr_vec_size : data->vec->end();

    std::sort(begin, end);

    pthread_barrier_wait(&barrier);

    int merge_cnt = my_log2(NUM_THREADS);

    for (int i = 1; i <= merge_cnt; ++i) {
        int each = 1 << i;

        if (data->tid % each == 0) {
            auto begin = data->vec->begin() + thr_vec_size * data->tid;
            auto end = data->tid != NUM_THREADS - 1 ? begin + thr_vec_size * i : data->vec->end();

            auto begin2 = data->vec->begin() + thr_vec_size * (data->tid + i);
            auto end2 = (data->tid + i) != NUM_THREADS - 1 && i < merge_cnt ? begin2 + thr_vec_size * i : data->vec->end();

            auto dest_begin = data->dest->begin() + thr_vec_size * data->tid;

            merge(begin, end, begin2, end2, dest_begin);  
        }

        pthread_barrier_wait(&barrier);
        if (data->tid == 0) data->vec->swap(*data->dest);
        pthread_barrier_wait(&barrier); 
    }

    pthread_exit(NULL);
}

void merge(vec_it_t head1, vec_it_t tail1, vec_it_t head2, vec_it_t tail2, vec_it_t dest_begin) {
    while (head1 != tail1 && head2 != tail2) {
        if (*head1 < *head2) *dest_begin++ = *head1++;
        else                 *dest_begin++ = *head2++;
    }
    while (head1 != tail1) *dest_begin++ = *head1++;
    while (head2 != tail2) *dest_begin++ = *head2++;
}

}; // <-- namespace parallel_sorter
#endif