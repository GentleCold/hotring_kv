import sys
import matplotlib.pyplot as plt
import numpy as np

from matplotlib import rcParams

rcParams["font.family"] = "Maple Mono SC NF"


if __name__ == "__main__":
    hotring = sys.argv[1]
    hash = sys.argv[2]

    with open(f"{hotring}_20.txt", "r") as file:
        hotring_20 = [int(line.strip()) for line in file]
    with open(f"{hotring}_40.txt", "r") as file:
        hotring_40 = [int(line.strip()) for line in file]
    with open(f"{hotring}_100.txt", "r") as file:
        hotring_100 = [int(line.strip()) for line in file]

    with open(f"{hotring}_20.txt", "r") as file:
        hash_20 = [int(line.strip()) for line in file]
    with open(f"{hotring}_40.txt", "r") as file:
        hash_40 = [int(line.strip()) for line in file]
    with open(f"{hotring}_100.txt", "r") as file:
        hash_100 = [int(line.strip()) for line in file]

    time = np.arange(1, 6, 1)

    plt.figure(figsize=(12, 6))
    fig, (ax1, ax2, ax3) = plt.subplots(nrows=3, ncols=1, figsize=(10, 8))

    ax1.plot(time, hotring_20, label="HotRing")
    ax1.plot(time, hash_20, label="HashTable")
    ax1.title("HotRing与HashTable的比较（X=20）")

    ax2.plot(time, hotring_40, label="HotRing")
    ax2.plot(time, hash_40, label="HashTable")
    ax2.title("HotRing与HashTable的比较（X=40）")

    ax3.plot(time, hotring_100, label="HotRing")
    ax3.plot(time, hash_100, label="HashTable")
    ax3.title("HotRing与HashTable的比较（X=100）")

    plt.xlabel("时间 (分钟)")
    plt.ylabel("每分钟处理的负载数量")

    plt.legend()
    plt.subplots_adjust(hspace=0.5)
    plt.grid(True)

    plt.savefig(f"{hotring}_{hash}.png")
