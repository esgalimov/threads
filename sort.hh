#ifndef PAR_SORT
#define PAR_SORT

#include <pthread.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <fstream>
#include <string>
#include <stdexcept>

namespace parallel_sorter {

pthread_barrier_t barrier;

using vec_it_t = typename std::vector<int>::iterator;

struct sort_pull_t final {
    std::vector<int>* vec;
    std::vector<int>* dest;

    int tid;
    int thr_num;
};

int my_log2(int x) {
    int cnt = 0;
    while (x >>= 1) cnt++;
    return cnt;
}

void* thread_sort(void* arg) {
    sort_pull_t* data = static_cast<sort_pull_t*>(arg);

    int thr_vec_size = std::distance(data->vec->begin(), data->vec->end()) / data->thr_num;

    #ifdef dDEBUG
        std::string filename = "filename" + std::to_string(data->tid) + ".txt";
        std::ofstream in(filename);
    #endif

    auto beg = data->vec->begin() + thr_vec_size * data->tid;
    auto en = data->tid != data->thr_num - 1 ? beg + thr_vec_size : data->vec->end();

    std::sort(beg, en);

    pthread_barrier_wait(&barrier);

    int merge_cnt = my_log2(data->thr_num);

    for (int i = 1; i <= merge_cnt; ++i) {
        int each = 1 << i;

        if (data->tid % each == 0) {
            auto begin = data->vec->begin() + thr_vec_size * data->tid;
            auto end = data->tid != data->thr_num - 1 ? begin + thr_vec_size * std::pow(2, i - 1) : data->vec->end();

            auto begin2 = end;
            auto end2 = (data->tid + i) != data->thr_num - 1 && i < merge_cnt ? begin2 + thr_vec_size * std::pow(2, i - 1) : data->vec->end();

            auto dest_begin = data->dest->begin() + thr_vec_size * data->tid;

            #ifdef dDEBUG
                in << "BEGIN  "  << " " << std::distance(data->vec->begin(), begin)  << std::endl;
                in << "END    "  << " " <<  std::distance(data->vec->begin(), end)    << std::endl;
                in << "BEGIN2 "  << " " <<  std::distance(data->vec->begin(), begin2) << std::endl;
                in << "END2   "  << " " <<  std::distance(data->vec->begin(), end2)   << std::endl;
                in << "DEST   "  << " " <<  std::distance(data->dest->begin(), dest_begin) << std::endl << std::endl;
            #endif

            merge(begin, end, begin2, end2, dest_begin);  
        }

        pthread_barrier_wait(&barrier);
        if (data->tid == 0) { 
            #ifdef dDEBUG
                std::cout << "STEP " << i << std::endl;
                for (auto it = data->dest->begin(); it != data->dest->end(); ++it) {
                    std::cout << *it << " ";
                }
                std::cout << std::endl;
            #endif
            data->vec->swap(*data->dest); 
        }
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

int merge_sort(std::vector<int>& vec, int thr_num) {
    if ((thr_num == 0) || (thr_num & (thr_num - 1))) 
        throw std::runtime_error("thr_num must be power of 2");

    std::vector<int> dest(vec.size());

    std::vector<pthread_t> thr(thr_num);
    std::vector<sort_pull_t> sp(thr_num);
    
    for (int i = 0; i < thr_num; ++i) {
        sp[i].tid = i;
        sp[i].thr_num = thr_num;

        sp[i].vec = &vec;
        sp[i].dest = &dest;
    }

    pthread_barrier_init(&parallel_sorter::barrier, NULL, thr_num);

    for (int i = 0; i < thr_num; ++i) {
        int ret_code = pthread_create(&thr[i], NULL, parallel_sorter::thread_sort, &sp[i]);

        if (ret_code) {
            std::cerr << "error: pthread_create, ret_code: " << ret_code << std::endl;
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < thr_num; ++i) {
        pthread_join(thr[i], NULL);
    }

    pthread_barrier_destroy(&parallel_sorter::barrier);

    #ifdef dDEBUG
        for (auto& elem : vec) std::cout << elem << " ";
        std::cout << std::endl;
    #endif

    return EXIT_SUCCESS;
}

}; // <-- namespace parallel_sorter
#endif