# My parallel merge sort (pthread lib)
Yes, it is better to use C++ API to deal with threads, but using pthread is my university task :)

My merge sort is different from classical: I use std::sort (which is an Introsort (a combination of QuickSort, Heapsort, and Insertion Sort)) on each thread and then parallel merge these sorted vectors.

## System and CPU
```
System:
  Kernel: 5.15.0-91-generic x86_64 bits: 64 compiler: gcc v: 11.4.0
    Desktop: Cinnamon 6.0.4 Distro: Linux Mint 21.3 Virginia
    base: Ubuntu 22.04 jammy

CPU:
  Info: quad core model: Intel Core i5-1035G1 bits: 64 type: MT MCP
    arch: Ice Lake rev: 5 cache: L1: 320 KiB L2: 2 MiB L3: 6 MiB
Info:
  Memory: 7.44 GiB
```
```
turboboost OFF
governor performance
fixed CPU frequency 3000 MHz
```
## Graphics
### Average time depending on the number of elements

![time](https://github.com/user-attachments/assets/ce8c9529-4cb7-4755-93c4-e3570532d96e)
![image](https://github.com/user-attachments/assets/89ba399d-cdb1-4d18-8922-98e06764fcd8)


### n*log(n) constant
![constant](https://github.com/user-attachments/assets/104dd595-4800-4f42-8d73-08fc63b0481a)
![image](https://github.com/user-attachments/assets/30638f66-0752-476c-97e7-1dc13115749f)

We can see that on my config there is almost no effect when using more than 8 threads.

## How to run
From project source dir:
```
cmake -B build
cd build
make
./sort_test
```
Then you can run *results.py* to see graphics.
