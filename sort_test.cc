#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <random>
#include <chrono>
#include <iostream>

#include "sort.hh"


using namespace parallel_sorter;

std::ofstream out("test_res.txt");

constexpr int WARMUP_ITERS = 10;
constexpr int TEST_ITERS   = 20;

std::vector<int> gen_data(long elem_num) {
    std::vector<int> vec(elem_num);

    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> dis(0, elem_num);

    for (int i = 0; i < elem_num; ++i) {
        vec.push_back(dis(gen));
    }

    return vec;
}

void param_test_std(std::vector<int>& vec, std::ostream& os) {
    os << "THR_NUM = 1" << std::endl << "ELEM_NUM = " << vec.size() << std::endl;

    // =============== STD::SORT ==================
    for (int i = 0; i < WARMUP_ITERS; ++i) { // warmup
        std::vector<int> vec_warmup(vec);
        std::sort(vec_warmup.begin(), vec_warmup.end());
    }

    double tm = 0.0;
    for (int i = 0; i < TEST_ITERS; ++i) { // benchmark
        std::vector<int> vec_test(vec);
        auto start = std::chrono::high_resolution_clock::now();
        std::sort(vec_test.begin(), vec_test.end());
        tm += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    }
    os << "STD::SORT SORT = " << tm / TEST_ITERS  << " us" << std::endl;
    //=============================================

    // =============== STD::QSORT ==================

    auto comp = [](const void* x, const void* y) {
                      const int arg1 = *static_cast<const int*>(x);
                      const int arg2 = *static_cast<const int*>(y);
                      const auto cmp = arg1 <=> arg2;
                      if (cmp < 0)
                          return -1;
                      if (cmp > 0)
                          return 1;
                      return 0;
                  };
    
    for (int i = 0; i < WARMUP_ITERS; ++i) { // warmup
        std::vector<int> vec_warmup(vec);
        std::qsort(vec_warmup.data(), vec_warmup.size(), sizeof(decltype(vec_warmup)::value_type), comp);
    }

    tm = 0.0;
    for (int i = 0; i < TEST_ITERS; ++i) { // benchmark
        std::vector<int> vec_test(vec);
        auto start = std::chrono::high_resolution_clock::now();
        std::qsort(vec_test.data(), vec_test.size(), sizeof(decltype(vec_test)::value_type), comp);
        tm += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    }
    os << "STD::QSORT SORT = " << tm / TEST_ITERS  << " us" << std::endl << std::endl;
    //=============================================
}

bool param_test_parallel(std::vector<int>& vec, long thr_num, std::ostream& os) {
    os << "THR_NUM = " << thr_num << std::endl << "ELEM_NUM = " << vec.size() << std::endl;

    double tm = 0.0;

    // =========== PARRALEL SORT =================
    for (int i = 0; i < WARMUP_ITERS; ++i) { // warmup
        std::vector<int> vec_warmup(vec);
        parallel_sorter::merge_sort(vec_warmup, thr_num);
    }

    for (int i = 0; i < TEST_ITERS; ++i) { // benchmark
        std::vector<int> vec_test(vec);
        auto start = std::chrono::high_resolution_clock::now();
        parallel_sorter::merge_sort(vec_test, thr_num);
        tm += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    }
    os << "PARRALEL SORT = " << tm / TEST_ITERS << " us" << std::endl << std::endl;
    //=============================================

    std::vector<int> vec2(vec);
    std::sort(vec2.begin(), vec2.end());
    parallel_sorter::merge_sort(vec, thr_num);

    auto it = vec.begin(), it2 = vec2.begin();

    while (it != vec.end()) {
        if (*it++ != *it2++) return false;
    }
    return true;

}

constexpr int MAX_POW = 15;

TEST(std_sort_qsort, threads_1) {
    for (int i = 0; i <= MAX_POW; ++i) {
        std::vector<int> vec = gen_data(1000 * std::pow(2, i));
        param_test_std(vec, out);
    }
}

TEST(merge_sort, threads_2) {
    for (int i = 0; i <= MAX_POW; ++i) {
        std::vector<int> vec = gen_data(1000 * std::pow(2, i));
        ASSERT_TRUE(param_test_parallel(vec, 2, out));
    }
}

TEST(merge_sort, threads_4) {
    for (int i = 0; i <= MAX_POW; ++i) {
        std::vector<int> vec = gen_data(1000 * std::pow(2, i));
        ASSERT_TRUE(param_test_parallel(vec, 4, out));
    }
}

TEST(merge_sort, threads_8) {
    for (int i = 0; i <= MAX_POW; ++i) {
        std::vector<int> vec = gen_data(1000 * std::pow(2, i));
        ASSERT_TRUE(param_test_parallel(vec, 8, out));
    }
}

TEST(merge_sort, threads_16) {
    for (int i = 0; i <= MAX_POW; ++i) {
        std::vector<int> vec = gen_data(1000 * std::pow(2, i));
        ASSERT_TRUE(param_test_parallel(vec, 16, out));
    }
}

TEST(merge_sort, threads_32) {
    for (int i = 0; i <= MAX_POW; ++i) {
        std::vector<int> vec = gen_data(1000 * std::pow(2, i));
        ASSERT_TRUE(param_test_parallel(vec, 32, out));
    }
}

TEST(merge_sort, threads_64) {
    for (int i = 0; i <= MAX_POW; ++i) {
        std::vector<int> vec = gen_data(1000 * std::pow(2, i));
        ASSERT_TRUE(param_test_parallel(vec, 64, out));
    }
}

TEST(merge_sort, threads_128) {
    for (int i = 0; i <= MAX_POW; ++i) {
        std::vector<int> vec = gen_data(1000 * std::pow(2, i));
        ASSERT_TRUE(param_test_parallel(vec, 128, out));
    }
}

TEST(merge_sort, threads_256) {
    for (int i = 0; i <= MAX_POW; ++i) {
        std::vector<int> vec = gen_data(1000 * std::pow(2, i));
        ASSERT_TRUE(param_test_parallel(vec, 256, out));
    }
}

// TEST(threads_4, elems_100_000) {
//     ASSERT_TRUE(param_test(4, 100'000, out));
// }

// TEST(threads_8, elems_100_000) {
//     ASSERT_TRUE(param_test(8, 100'000, out));
// }

// TEST(threads_16, elems_100_000) {
//     ASSERT_TRUE(param_test(16, 100'000, out));
// }

// TEST(threads_32, elems_100_000) {
//     ASSERT_TRUE(param_test(32, 100'000, out));
// }

// TEST(threads_64, elems_100_000) {
//     ASSERT_TRUE(param_test(64, 100'000, out));
// }

// TEST(threads_2, elems_500_000) {
//     ASSERT_TRUE(param_test(2, 500'000, out));
// }

// TEST(threads_4, elems_500_000) {
//     ASSERT_TRUE(param_test(4, 500'000, out));
// }

// TEST(threads_8, elems_500_000) {
//     ASSERT_TRUE(param_test(8, 500'000, out));
// }

// TEST(threads_16, elems_500_000) {
//     ASSERT_TRUE(param_test(16, 500'000, out));
// }

// TEST(threads_32, elems_500_000) {
//     ASSERT_TRUE(param_test(32, 500'000, out));
// }

// TEST(threads_64, elems_500_000) {
//     ASSERT_TRUE(param_test(64, 500'000, out));
// }


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}