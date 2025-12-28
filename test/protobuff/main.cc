
#include <string>
#include <iostream>

#include "pbgen/test.pb.h"

using namespace test_pkg;

void test1() {
    // 序列化
    LoginRequest req;
    req.set_uid(10001);
    req.set_password("1234560");
    std::string str_req;
    if (req.SerializeToString(&str_req)) {
        std::cout << str_req << std::endl;
    }

    // 反序列化
    LoginRequest reqX;
    if (reqX.ParseFromString(str_req)) {
        std::cout << reqX.uid() << std::endl;
        std::cout << reqX.password() << std::endl;
    }

    // 登陆请求响应
    LoginResponse rsp;
    ResponseStatus* status = rsp.mutable_status();
    status->set_code(0);
    status->set_message("login_succeed.");
}

void test2() {
    // 好友列表获取响应
    GetFriendListResponse rsp;
    ResponseStatus* status = rsp.mutable_status();
    status->set_code(0);
    status->set_message("friend list succeed.");

    UserInfo* userInfo = nullptr;
    userInfo = rsp.add_friends();
    userInfo->set_id(10001);
    userInfo->set_name("luochenhao");
    userInfo->set_age(25);
    userInfo->set_sex(UserInfo::MALE);

    userInfo = rsp.add_friends();
    userInfo->set_id(10002);
    userInfo->set_name("lch");
    userInfo->set_age(27);
    userInfo->set_sex(UserInfo::MALE);
    
    std::cout << rsp.friends_size() << std::endl;
}

int main() {
    test1();
    test2();

    return 0;
}

