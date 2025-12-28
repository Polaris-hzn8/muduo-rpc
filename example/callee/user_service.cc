
/**
* @author: luochenhao
* @email: lch2022fox@163.com
* @time: Sun 28 Dec 2025 16:56:38 CST
* @brief: rpc服务发布端
**/

#include <string>
#include <iostream>

#include "user.pb.h"
#include "mdrpc_provider.h"
#include "mdrpc_application.h"

// RPC服务发布端
class UserService : public user_pkg::UserServiceRpc {
public:
    bool Login(const std::string& uid, const std::string& password) {
        // 远程方法执行的具体逻辑
        // 1. 获取数据库连接
        // 2. 查询用户名和密码
        // 3. 如果用户名和密码正确，返回true
        // 4. 如果用户名和密码不正确，返回false
        std::cout << "callee do service rpc function: login" << std::endl;
        std::cout << "uid: " << uid << std::endl;
        std::cout << "password: " << password << std::endl;
        return true;
    }
    // 重写基类虚函数（rpc框架调用）
    void Login(
        ::google::protobuf::RpcController* controller,
        const ::user_pkg::LoginRequest* request,
        ::user_pkg::LoginResponse* response,
        ::google::protobuf::Closure* done) {
        // 框架给业务上报了请求参数LoginRequest，业务获取相应数据做本地业务
        // Protobuff直接从字节流中取出数据 省去协议解析的过程
        // 1. 获取请求参数
        std::string uid = std::to_string(request->uid());
        std::string password = request->password();
        // 2. 调用本地业务方法
        bool login_result = Login(uid, password);
        // 3. 填充响应
        user_pkg::ResponseStatus* status = response->mutable_status();
        if (login_result) {
            status->set_code(0);
            status->set_message("login succeed!");
        } else {
            status->set_code(1);
            status->set_message("login failed!");
        }
        // 4. 执行回调操作
        // 执行响应对象数据的序列化和网络发送（都是由框架完成）
        done->Run();
    }
};

int main(int argc, char** argv) {
    // 框架初始化
    MdrpcApplication::Init(argc, argv);

    // 将UserService对象发布到rpc服务节点上
    // 注册UserService服务对象
    MdrpcProvider server;
    server.RegisterService(new UserService());

    // 启动rpc服务节点
    // 进程进入阻塞状态等待远程调用
    server.Run();

    return 0;
}

