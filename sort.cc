#include <iostream>
#include <random>
#include <chrono>
#include "sort.hh"

constexpr int ELEM_NUM = 100'000;

int main(int argc, char* argv[]) try {
    std::vector<int> vec(ELEM_NUM);

    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> dis(0, ELEM_NUM);

    for (int i = 0; i < ELEM_NUM; ++i) {
        vec[i] = dis(gen);
    }

    std::vector<int> vec2 = vec;

    #ifdef dDEBUG
        for (auto& elem : vec) std::cout << elem << " ";
        std::cout << std::endl;
        std::cout << std::endl;
    #endif


    auto start = std::chrono::high_resolution_clock::now();

    parallel_sorter::merge_sort(vec, 8);

    double tm = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
    
    std::cout << "PARRALEL SORT = " << tm << " us" << std::endl << std::endl;


    start = std::chrono::high_resolution_clock::now();

    std::sort(vec2.begin(), vec2.end());

    tm = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start).count();

    std::cout << "STD::SORT SORT = " << tm << " us" << std::endl << std::endl;

    auto it = vec.begin(), it2 = vec2.begin();
    bool flag = true;

    while (it != vec.end()) {
        if (*it++ != *it2++) flag = false;
    }

    if (!flag) std::cout << "BAD SORT" << std::endl;

    return EXIT_SUCCESS;
} catch (const std::exception& exc) {
    std::cerr << "Something wrong: " << exc.what() << std::endl;
}