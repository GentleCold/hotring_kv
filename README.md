# 基于负载热点感知的键值存储

项目使用了名叫HotRing的数据结构，是一种针对热点数据的内存键值存储系统设计方案，参考论文为：[HotRing: A Hotspot-Aware In-Memory Key-Value Store](https://www.usenix.org/system/files/fast20-chen_jiqiang.pdf)

项目由cmake构建，使用了GoogleTest进行单元测试。

运行命令`sh build.sh`将编译二进制文件并运行单元测试代码。

项目利用socket构建了简单的客户端-服务端模型，其中random_load将不断生成负载，运行命令`sh analyse.sh`将在`./utils/`目录下生成分析图表。
