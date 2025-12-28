/**
* @author: luochenhao
* @email: lch2022fox@163.com
* @time: Sun 28 Dec 2025 16:57:24 CST
* @brief: 主应用程序类
**/

#pragma once

#include "mdrpc_config.h"

class MdrpcApplication {
public:
    static MdrpcApplication& GetInstance();
    static void Init(int argc, char** argv);

private:
    MdrpcApplication() = default;
    MdrpcApplication(const MdrpcApplication&) = delete;
    MdrpcApplication(MdrpcApplication&&) = delete;
    MdrpcApplication& operator=(const MdrpcApplication&) = delete;
};
