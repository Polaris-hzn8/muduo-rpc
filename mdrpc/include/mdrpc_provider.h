/**
* @author: luochenhao
* @email: lch2022fox@163.com
* @time: Sun 28 Dec 2025 16:57:46 CST
* @brief: 框架提供的负责发布rpc服务的网络对象类
**/

#pragma once

#include <memory>
#include <unordered_map>
#include <muduo/net/TcpServer.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <google/protobuf/service.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

// 保存服务对象和服务方法的映射表
struct ServiceInfo {
    // 保存服务对象
    google::protobuf::Service* _service;
    // 保存服务方法名称和服务方法描述符的映射表
    std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> _method_map;
};

class MdrpcProvider {
public:
    // 发布rpc服务方法
    void RegisterService(google::protobuf::Service* service);
    // 启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();
private:
    // 新连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr& conn);
    // 消息读写回调
    void OnMessage(const muduo::net::TcpConnectionPtr& conn,
                   muduo::net::Buffer* buffer,
                   muduo::Timestamp receive_time);
private:
    // 存储注册成功的服务对象和服务方法
    std::unordered_map<std::string, ServiceInfo> _service_map;
    // eventLoop对象(muduo库提供)
    muduo::net::EventLoop _event_loop;
};

