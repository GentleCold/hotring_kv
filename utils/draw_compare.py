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

    with open(f"{hash}_20.txt", "r") as file:
        hash_20 = [int(line.strip()) for line in file]
    with open(f"{hash}_40.txt", "r") as file:
        hash_40 = [int(line.strip()) for line in file]
    with open(f"{hash}_100.txt", "r") as file:
        hash_100 = [int(line.strip()) for line in file]

    time = np.arange(1, 6, 1)

    plt.figure(figsize=(12, 8))
    fig, (ax1, ax2, ax3, ax4) = plt.subplots(nrows=4, ncols=1, figsize=(10, 10))

    ax1.plot(time, hotring_20, label="HotRing")
    ax1.plot(time, hash_20, label="HashTable")
    ax1.set_title("HotRing与HashTable的比较（X=20）")
    ax1.set_xlabel("时间 (分钟)")
    ax1.set_ylabel("每分钟处理的负载数量")
    ax1.legend()
    ax1.grid(True)

    ax2.plot(time, hotring_40, label="HotRing")
    ax2.plot(time, hash_40, label="HashTable")
    ax2.set_title("HotRing与HashTable的比较（X=40）")
    ax2.set_xlabel("时间 (分钟)")
    ax2.set_ylabel("每分钟处理的负载数量")
    ax2.legend()
    ax2.grid(True)

    ax3.plot(time, hotring_100, label="HotRing")
    ax3.plot(time, hash_100, label="HashTable")
    ax3.set_title("HotRing与HashTable的比较（X=100）")
    ax3.set_xlabel("时间 (分钟)")
    ax3.set_ylabel("每分钟处理的负载数量")
    ax3.legend()
    ax3.grid(True)

    x = np.arange(3)
    width = 0.35

    ax4.bar(
        x - width / 2,
        [sum(hotring_20), sum(hotring_40), sum(hotring_100)],
        width,
        label="HotRing",
    )
    ax4.bar(
        x + width / 2,
        [sum(hash_20), sum(hash_40), sum(hash_100)],
        width,
        label="HashTable",
    )
    ax4.set_xticks(x)
    ax4.set_xticklabels(["X=20", "X=40", "X=100"])
    ax4.set_title("HotRing与HashTable的总负载比较")
    ax4.set_xlabel("比例X")
    ax4.set_ylabel("总负载数量")
    ax4.legend()

    plt.subplots_adjust(hspace=0.5)
    plt.savefig(f"{hotring}_{hash}.png")
