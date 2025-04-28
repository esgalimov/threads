import numpy as np
import matplotlib.pyplot as plt
import pathlib

filename = pathlib.Path(__file__).parent / "build" / "test_res.txt"

lines = []

with open(str(filename), "r") as file:
    lines = file.readlines()



std_sort = []
qsort = []
merge_2 = []
merge_4 = []
merge_8 = []
merge_16 = []
merge_32 = []
merge_64 = []
merge_128 = []
merge_256 = []

elems = [2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, 512000, 1024000, 2048000, 4096000, 8192000, 16384000, 32768000, 65536000]

for i in range(len(lines)):
    if "THR_NUM" in lines[i] and lines[i].split()[2] == "1":
        if "STD::SORT" in lines[i + 2]:
            std_sort.append(float(lines[i + 2].split()[3]))
            if "STD::QSORT" in lines[i + 3]:
                qsort.append(float(lines[i + 3].split()[3]))

    if "THR_NUM" in lines[i] and lines[i].split()[2] == "2":
        if "PARRALEL" in lines[i + 2]:
            merge_2.append(float(lines[i + 2].split()[3]))

    if "THR_NUM" in lines[i] and lines[i].split()[2] == "4":
        if "PARRALEL" in lines[i + 2]:
            merge_4.append(float(lines[i + 2].split()[3]))

    if "THR_NUM" in lines[i] and lines[i].split()[2] == "8":
        if "PARRALEL" in lines[i + 2]:
            merge_8.append(float(lines[i + 2].split()[3]))

    if "THR_NUM" in lines[i] and lines[i].split()[2] == "16":
        if "PARRALEL" in lines[i + 2]:
            merge_16.append(float(lines[i + 2].split()[3]))

    if "THR_NUM" in lines[i] and lines[i].split()[2] == "32":
        if "PARRALEL" in lines[i + 2]:
            merge_32.append(float(lines[i + 2].split()[3]))

    if "THR_NUM" in lines[i] and lines[i].split()[2] == "64":
        if "PARRALEL" in lines[i + 2]:
            merge_64.append(float(lines[i + 2].split()[3]))

    if "THR_NUM" in lines[i] and lines[i].split()[2] == "128":
        if "PARRALEL" in lines[i + 2]:
            merge_128.append(float(lines[i + 2].split()[3]))

    if "THR_NUM" in lines[i] and lines[i].split()[2] == "256":
        if "PARRALEL" in lines[i + 2]:
            merge_256.append(float(lines[i + 2].split()[3]))
       

std_sort = np.array(std_sort)
qsort = np.array(qsort)
merge_2 = np.array(merge_2)
merge_4 = np.array(merge_4)
merge_8 = np.array(merge_8)
merge_16 = np.array(merge_16)
merge_32 = np.array(merge_32)
merge_64 = np.array(merge_64)
merge_128 = np.array(merge_128)
merge_256 = np.array(merge_256)




fig, ax = plt.subplots(figsize=(8,10))

ax.plot(elems, std_sort, markersize=4, marker="o", linewidth=1, label="std::sort", )
ax.plot(elems, qsort, markersize=4, marker=".", linewidth=1, label="qsort")

ax.plot(elems, merge_2, markersize=4, marker="v", linewidth=1, label="merge sort thr 2")
ax.plot(elems, merge_4, markersize=4, marker="x", linewidth=1, label="merge sort thr 4")
ax.plot(elems, merge_8, markersize=4, marker="^", linewidth=1, label="merge sort thr 8")
ax.plot(elems, merge_16, markersize=4, marker="p", linewidth=1, label="merge sort thr 16")
ax.plot(elems, merge_32, markersize=4, marker="*", linewidth=1, label="merge sort thr 32")
ax.plot(elems, merge_64, markersize=4, marker="<", linewidth=1, label="merge sort thr 64")
ax.plot(elems, merge_128, markersize=4, marker=">", linewidth=1, label="merge sort thr 128")
ax.plot(elems, merge_256, markersize=4, marker="s", linewidth=1, label="merge sort thr 256")

ax.grid(which='major')
ax.minorticks_on()
ax.grid(which='minor', linestyle=':')
ax.set_title("Время сортировки в зависимости от кол-ва элементов")
ax.set(ylabel="Время, мкс", xlabel="Кол-во элементов")
ax.legend(loc="upper left")

plt.savefig("time.png")


fig2, ax2 = plt.subplots(figsize=(8,10))

ax2.plot(elems, std_sort / elems * np.log2(elems), markersize=4, marker="o", linewidth=1, label="std::sort", )
ax2.plot(elems, qsort / elems * np.log2(elems), markersize=4, marker=".", linewidth=1, label="qsort")

ax2.plot(elems, merge_2 / elems * np.log2(elems), markersize=4, marker="v", linewidth=1, label="merge sort thr 2")
ax2.plot(elems, merge_4 / elems * np.log2(elems), markersize=4, marker="x", linewidth=1, label="merge sort thr 4")
ax2.plot(elems, merge_8 / elems * np.log2(elems), markersize=4, marker="^", linewidth=1, label="merge sort thr 8")
ax2.plot(elems, merge_16 / elems * np.log2(elems), markersize=4, marker="p", linewidth=1, label="merge sort thr 16")
ax2.plot(elems, merge_32 / elems * np.log2(elems), markersize=4, marker="*", linewidth=1, label="merge sort thr 32")
ax2.plot(elems, merge_64 / elems * np.log2(elems), markersize=4, marker="<", linewidth=1, label="merge sort thr 64")
ax2.plot(elems, merge_128 / elems * np.log2(elems), markersize=4, marker=">", linewidth=1, label="merge sort thr 128")
ax2.plot(elems, merge_256 / elems * np.log2(elems), markersize=4, marker="s", linewidth=1, label="merge sort thr 256")

ax2.grid(which='major')
ax2.minorticks_on()
ax2.grid(which='minor', linestyle=':')
ax2.set_title("Константа перед n*log(n)")
ax2.set(ylabel="Константа", xlabel="Кол-во элементов")
ax2.legend(loc="upper right")

plt.savefig("constant.png")

plt.show()