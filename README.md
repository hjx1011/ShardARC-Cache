# ShardARC-Cache

**ShardARC-Cache** 是一个基于现代 C++14 开发的高性能、高可用分布式内存缓存系统。它不仅实现了复杂的 **ARC (Adaptive Replacement Cache)** 自适应淘汰算法，还通过**分段锁架构**解决了高并发下的性能瓶颈。

---

## ✨ 核心特性

- **🧠 自适应淘汰算法 (ARC)**: 核心采用 ARC 算法，通过动态调整近期访问（T1）与高频访问（T2）的配额，有效抵御缓存污染，命中率优于传统 LRU。
- **🔒 16 分片分段锁 (Sharded Locking)**: 将全局锁拆分为 16 个独立分片锁，极大降低了多线程高并发下的锁竞争，吞吐量提升显著。
- **🛡️ 生产级高可用防御**:
  - **防击穿**: 采用 **Singleflight (Loader Lock)** 与 **双重检查 (DCL)**，确保热点 Key 仅由 1 个线程查询数据库。
  - **防穿透**: 引入 **空对象缓存 (Cache Null)**，有效拦截无效 Key 的恶意攻击。
  - **防雪崩**: 实现 **TTL 随机抖动 (Jitter)**，打散过期时间，保护数据库平稳运行。
- **⌛ 生命周期管理**: 支持 **TTL (Time-To-Live)** 惰性删除机制以及 **驱逐回调 (Eviction Callback)**，方便业务层解耦。
- **🌐 RESTful 接口**: 封装基于 `cpp-httplib` 的 HTTP 服务层，支持跨进程、跨语言的分布式调用。
- **🏗️ 工程化架构**: 严格的头文件分离、泛型支持、RAII 内存管理及 CMake 构建。

---

## 📂 项目结构

```text
.
├── CMakeLists.txt        # 构建配置文件
├── include/              # 头文件 (ARC, LRU, LFU, ShardedCache, ICache, CacheManager)
├── src/                  # 核心实现 (CacheServer, Database, CacheManager, main)
├── test/                 # 并发压测模块
├── third_party/          # 第三方库 (httplib.h)
└── LICENSE               # MIT 许可证
🚀 快速开始
编译构建
code
Bash
mkdir build && cd build
cmake ..
make
运行服务
code
Bashgit status
./kamacache_server
网络接口调用
code
Bash
# 写入数据 (10s 过期)
curl -X POST "http://localhost:8080/put?key=name&val=Kama&ttl=10"

# 读取数据
curl "http://localhost:8080/get?key=name"
📈 性能测试
在 10 线程并发压测环境下，单全局锁版本与分段锁版本的耗时对比：
架构类型	10w 次读写耗时	并发表现
单全局锁	~1200ms	锁争用严重，多核利用率低
ShardARC (16分片)	~350ms	锁竞争极小，高并发吞吐能力极强
