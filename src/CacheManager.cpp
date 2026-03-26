#include "CacheManager.h"
#include "Database.h"
#include <iostream>
#include <random>
#include <thread>
#include <memory>
#include <mutex>
// 定义一个全局回调：当底层 ARC 淘汰数据时，打印一条日志
auto global_eviction_callback = [](const std::string& k, const std::string& v) {
    std::cout << "  🗑️ [回调警报] 缓存满了/过期了！数据被驱逐 -> Key: " << k << ", Val: " << v << std::endl;
};

// 初始化：16 分片，总容量 10，传入驱逐回调（容量设小点，方便测试驱逐）
HighAvailableCacheManager::HighAvailableCacheManager() 
    : cache_(16, 10, global_eviction_callback) {}

int HighAvailableCacheManager::get_random_jitter() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 60);
    return dis(gen);
}

void HighAvailableCacheManager::put_data(const std::string& key, const std::string& value, int ttl_seconds) {
    cache_.put(key, value, ttl_seconds);
}

std::string HighAvailableCacheManager::get_data(const std::string& key) {
    std::string val;
    if (cache_.get(key, val)) {
        if (val == "[EMPTY_DATA]") {
            std::cout << "[Cache] 拦截到黑客请求 " << key << "，直接返回空！" << std::endl;
            return ""; 
        }
        std::cout << "[Cache] 命中真实数据: " << key << " -> " << val << std::endl;
        return val; 
    }

    std::shared_ptr<std::mutex> key_lock;
    {
        std::lock_guard<std::mutex> global_lock(loader_mtx_);
        if (key_locks_.find(key) == key_locks_.end()) {
            key_locks_[key] = std::make_shared<std::mutex>();
        }
        key_lock = key_locks_[key];
    }

    std::lock_guard<std::mutex> lock(*key_lock);

    if (cache_.get(key, val)) {
        if (val == "[EMPTY_DATA]") return "";
        std::cout << "[Cache Double Check] 线程 " << std::this_thread::get_id() 
                  << " 醒来发现别人已经查好放入缓存了，直接捡漏: " << val << std::endl;
        return val;
    }

    std::string db_result = query_mysql(key); 

    if (db_result.empty()) {
        std::cout << "[Cache] 数据库没数据，存入空对象防穿透: " << key << std::endl;
        cache_.put(key, "[EMPTY_DATA]", 30);
        return "";
    } else {
        int base_ttl = 3600;
        int safe_ttl = base_ttl + get_random_jitter();
        std::cout << "[Cache] 成功从 DB 拿到数据，写入缓存并加随机抖动 TTL=" << safe_ttl << "秒" << std::endl;
        cache_.put(key, db_result, safe_ttl);
        return db_result;
    }
}