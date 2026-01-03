/**
* @author: luochenhao
* @email: lch2022fox@163.com
* @time: Sun 28 Dec 2025 17:05:31 CST
* @brief: 
**/

#include <string>
#include <iostream>
#include <functional>
#include "mdrpc_provider.h"
#include "mdrpc_application.h"
#include "rpcheader.pb.h"

// 注册发布rpc服务方法
void MdrpcProvider::RegisterService(google::protobuf::Service* service) {
    ServiceInfo service_info;
    service_info._service = service;
    service_info._method_map.clear();

    // 服务描述符
    const google::protobuf::ServiceDescriptor* sd = service->GetDescriptor();
    // 服务名称
    std::string service_name = sd->name();
    // 服务方法数量
    int method_count = sd->method_count();
    for (int i = 0; i < method_count; ++i) {
        // 服务方法描述符
        const google::protobuf::MethodDescriptor* md = sd->method(i);
        // 方法名称
        std::string method_name = md->name();
        // 存储服务方法名称和方法描述符的映射
        service_info._method_map.insert({method_name, md});
        std::cout << "Register method: " << method_name << " for service: " << service_name << std::endl;
    }
    _service_map.insert({service_name, service_info});
    std::cout << "Register service: " << service_name << " with " << method_count << " methods." << std::endl;
}

void MdrpcProvider::Run() {
    // RPC服务节点ip和port端口数据获取
    MdrpcConfig& config = MdrpcApplication::GetConfig();
    auto ip_opt = config.Load("rpcserver_ip");
    auto port_opt = config.Load("rpcserver_port");
    if (ip_opt == std::nullopt || port_opt == std::nullopt) {
        std::cerr << "rpcserver_ip or rpcserver_port is not configured!" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string ip = ip_opt.value();
    uint16_t port = static_cast<uint16_t>(std::stoi(port_opt.value()));
    
    muduo::net::InetAddress address(ip, port);

    // 创建tcpServer
    muduo::net::TcpServer tcp_server(&_event_loop, address, "MdrpcProvider");
    tcp_server.start();
    // 绑定连接回调和消息读写回调方法
    // 分离网络代码与业务代码
    tcp_server.setConnectionCallback(std::bind(&MdrpcProvider::OnConnection, this, std::placeholders::_1));
    tcp_server.setMessageCallback(std::bind(&MdrpcProvider::OnMessage, this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3));
    // 设置muduo库的线程数量(基于Reactor模型的服务器：EPOLL + 线程池)
    tcp_server.setThreadNum(4);
    // 启动网络服务
    tcp_server.start();
    std::cout << "MdrpcProvider start service at " << ip << ":" << port << std::endl;
    // 事件循环epoll_wait
    _event_loop.loop();
}

// 短链接请求
void MdrpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn) {
    if (!conn->connected()) {
        // 断开连接
        conn->shutdown();
    } else {
        // 新连接建立
        std::cout << "New connection established: " << conn->peerAddress().toIpPort() << std::endl;


    }
}

// 长链接请求
// 处理rpc请求的反序列化、调用服务方法、序列化响应等逻辑
// 在框架内部 RpcProvider和RpcConsumer协商好通信协议
// header_size(4) + header_data(?) + args_data(?)
// service_name_size(4) + method_name_size(4) + args_size(?)
void MdrpcProvider::OnMessage(
    const muduo::net::TcpConnectionPtr& conn,
    muduo::net::Buffer* buffer,
    muduo::Timestamp receive_time) {
    //////////////////////////////////////////////////////////////////////////////////
    // 读取客户端发送的rpc请求消息
    std::string request = buffer->retrieveAllAsString();
    // 读取前4个字节获取header_size
    uint32_t header_size = 0;
    std::memcpy(&header_size, request.data(), 4);
    // 读取header_size大小的header_data数据
    std::string header_data = request.substr(4, header_size);

    // 反序列化header_data数据
    mdrpc_pkg::RpcHeader rpc_header;
    if (!rpc_header.ParseFromString(header_data)) {
        std::cerr << "Failed to parse RpcHeader." << std::endl;
        return;
    }
    // 读取args_size大小的args_data数据
    std::string service_name = rpc_header.service_name();
    std::string method_name = rpc_header.method_name();
    uint32_t args_size = rpc_header.args_size();
    std::string args_data = request.substr(4 + header_size, args_size);
    std::cout << "Received RPC request for service: " << service_name
              << ", method: " << method_name
              << ", args_size: " << args_size << std::endl;
    
    //////////////////////////////////////////////////////////////////////////////////
    // 查找服务对象和方法描述符
    auto service_it = _service_map.find(service_name);
    if (service_it == _service_map.end()) {
        std::cerr << "Service not found: " << service_name << std::endl;
        return;
    }
    ServiceInfo& service_info = service_it->second;
    auto method_it = service_info._method_map.find(method_name);
    if (method_it == service_info._method_map.end()) {
        std::cerr << "Method not found: " << method_name << " for service: " << service_name << std::endl;
        return;
    }

    const google::protobuf::MethodDescriptor* method_descriptor = method_it->second;
    google::protobuf::Service* service = service_info._service;
    google::protobuf::Message* request_msg = service->GetRequestPrototype(method_descriptor).New();
    google::protobuf::Message* response_msg = service->GetResponsePrototype(method_descriptor).New();

    // 反序列化请求消息
    if (!request_msg->ParseFromString(args_data)) {
        std::cerr << "Failed to parse request message." << std::endl;
        return;
    }

    //////////////////////////////////////////////////////////////////////////////////
    // 调用服务方法
    service->CallMethod(method_descriptor, nullptr, request_msg, response_msg, nullptr);

    //////////////////////////////////////////////////////////////////////////////////
    // 序列化响应消息
    std::string response_data;
    if (!response_msg->SerializeToString(&response_data)) {
        std::cerr << "Failed to serialize response message." << std::endl;
        return;
    }
    conn->send(response_data);
}