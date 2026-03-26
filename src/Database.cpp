#include "Database.h"
#include <iostream>
#include <thread>
#include <chrono>

std::string query_mysql(const std::string& key) {
    // 打印线程 ID，方便观察是谁击穿了缓存
    std::cout << "\n  🚨 [DB 警报] 线程 " << std::this_thread::get_id() 
              << " 击穿了缓存，正在缓慢查询 MySQL: " << key << "...\n" << std::endl;
    
    // 模拟极慢的磁盘 IO (500毫秒)
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
    
    // 假设数据库里只有 user_1 和 user_2 有数据
    if (key == "user_1") return "Alice";
    if (key == "user_2") return "Bob";
    
    return ""; // 数据库里根本没有这个数据！(模拟黑客乱查)
}