import matplotlib.pyplot as plt
import numpy as np

from matplotlib import rcParams

rcParams["font.family"] = "Maple Mono SC NF"

with open("load_data_20.txt", "r") as file:
    load_data_20 = [int(line.strip()) for line in file]
with open("load_data_40.txt", "r") as file:
    load_data_40 = [int(line.strip()) for line in file]
with open("load_data_100.txt", "r") as file:
    load_data_100 = [int(line.strip()) for line in file]

time = np.arange(1, 6, 1)

plt.figure(figsize=(12, 6))

plt.plot(time, load_data_20, label="X = 20")
for i, value in enumerate(load_data_20):
    plt.text(time[i], value, str(int(value)), ha="center", va="bottom", fontsize=8)
plt.plot(time, load_data_40, label="X = 40")
for i, value in enumerate(load_data_40):
    plt.text(time[i], value, str(int(value)), ha="center", va="bottom", fontsize=8)
plt.plot(time, load_data_100, label="X = 100")
for i, value in enumerate(load_data_100):
    plt.text(time[i], value, str(int(value)), ha="center", va="bottom", fontsize=8)

plt.xlabel("时间 (分钟)")
plt.ylabel("每分钟处理的负载数量")
plt.title("不同热点 key 比例下的系统负载")
plt.legend()

plt.grid(True)
plt.savefig("ring_without_hotmove.png")
