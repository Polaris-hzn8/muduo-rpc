/**
* @author: luochenhao
* @email: lch2022fox@163.com
* @time: Sun 28 Dec 2025 18:28:52 CST
* @brief: 配置加载
**/

#pragma once

#include <string>
#include <unordered_map>

class MdrpcConfig {
public:
    // 解析加载配置文件
    bool LoadConfigFile(const char* config_file);
    // 查询配置项信息
    std::optional<std::string> Load(const std::string& key);
private:
    std::unordered_map<std::string, std::string> _config_map;
};
