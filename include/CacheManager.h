#pragma once
#include <string>
#include <mutex>
#include <unordered_map>
#include <memory>
#include "ShardedCache.h"

class HighAvailableCacheManager {
private:
    ShardedCache<std::string, std::string> cache_;
    std::mutex loader_mtx_;
    std::unordered_map<std::string, std::shared_ptr<std::mutex>> key_locks_;
    int get_random_jitter();

public:
    HighAvailableCacheManager();
    std::string get_data(const std::string& key);
    
    // 【新增】：手动写入缓存接口，方便 HTTP Server 调用
    void put_data(const std::string& key, const std::string& value, int ttl_seconds = 0);
};