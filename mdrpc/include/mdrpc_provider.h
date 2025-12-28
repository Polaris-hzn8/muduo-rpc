/**
* @author: luochenhao
* @email: lch2022fox@163.com
* @time: Sun 28 Dec 2025 16:57:46 CST
* @brief: 框架提供的负责发布rpc服务的网络对象类
**/

#pragma once

#include <google/protobuf/service.h>

class MdrpcProvider {
public:
    // 发布rpc服务方法
    void RegisterService(google::protobuf::Service* service);
    // 启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();
private:

};

