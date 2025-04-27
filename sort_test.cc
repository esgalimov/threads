#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <random>
#include <chrono>
#include <iostream>

#include "sort.hh"


using namespace parallel_sorter;

std::ofstream out("test_res.txt");

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

bool param_test(long thr_num, long elem_num, std::ostream& os) {
    std::vector<int> vec = gen_data(elem_num);

    std::vector<int> vec2(vec);


    auto start = std::chrono::high_resolution_clock::now();
    parallel_sorter::merge_sort(vec, thr_num);
    double tm = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    
    os << "PARRALEL SORT = " << tm << " us" << std::endl;


    start = std::chrono::high_resolution_clock::now();
    std::sort(vec2.begin(), vec2.end());
    tm = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();

    os << "STD::SORT SORT = " << tm << " us" << std::endl << std::endl;


    auto it = vec.begin(), it2 = vec2.begin();

    while (it != vec.end()) {
        if (*it++ != *it2++) return false;
    }
    return true;

}

TEST(threads_2, elems_100_000) {
    ASSERT_TRUE(param_test(2, 100'000, out));
}

TEST(threads_4, elems_100_000) {
    ASSERT_TRUE(param_test(4, 100'000, out));
}

TEST(threads_8, elems_100_000) {
    ASSERT_TRUE(param_test(8, 100'000, out));
}

TEST(threads_16, elems_100_000) {
    ASSERT_TRUE(param_test(16, 100'000, out));
}

TEST(threads_32, elems_100_000) {
    ASSERT_TRUE(param_test(32, 100'000, out));
}

TEST(threads_64, elems_100_000) {
    ASSERT_TRUE(param_test(64, 100'000, out));
}

TEST(threads_2, elems_500_000) {
    ASSERT_TRUE(param_test(2, 500'000, out));
}

TEST(threads_4, elems_500_000) {
    ASSERT_TRUE(param_test(4, 500'000, out));
}

TEST(threads_8, elems_500_000) {
    ASSERT_TRUE(param_test(8, 500'000, out));
}

TEST(threads_16, elems_500_000) {
    ASSERT_TRUE(param_test(16, 500'000, out));
}

TEST(threads_32, elems_500_000) {
    ASSERT_TRUE(param_test(32, 500'000, out));
}

TEST(threads_64, elems_500_000) {
    ASSERT_TRUE(param_test(64, 500'000, out));
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}