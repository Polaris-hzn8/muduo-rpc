/**
* @author: luochenhao
* @email: lch2022fox@163.com
* @time: Sun 28 Dec 2025 18:28:36 CST
* @brief: 
**/

#include <fstream>
#include <stdexcept>
#include <cstring>
#include <cstdio>
#include <optional>
#include <iostream>

#include "mdrpc_config.h"

bool MdrpcConfig::LoadConfigFile(const char* config_file) {
    FILE *pf = fopen(config_file, "r");
    if (pf == nullptr) {
        throw std::runtime_error("Config file not found");
        return false;
    }
    char line[512] = {0};
    while (fgets(line, sizeof(line), pf) != nullptr) {
        std::string str_line(line);
        size_t pos = str_line.find('=');
        if (pos == std::string::npos)
            continue; // 无效行
        std::string key = str_line.substr(0, pos);
        std::string value = str_line.substr(pos + 1);
        // 去除可能的换行符
        if (!value.empty() && value.back() == '\n')
            value.pop_back();
        _config_map.insert({key, value});
    }
    fclose(pf);
    return true;
}

std::optional<std::string> MdrpcConfig::Load(const std::string& key) {
    auto it = _config_map.find(key);
    if (it != _config_map.end()) {
        return it->second;
    }
    return std::nullopt;
}
