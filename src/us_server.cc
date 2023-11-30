/********************************************************************************
 * @author: Wulnut
 * @email: carepdime@gmail.com
 * @date: 2023/11/24 15:49
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "include/us_server.h"
#include "include/common.h"
#include "include/json.hpp"
#include "include/us_service.h"
#include <iostream>

using namespace std;
using namespace placeholders;
using json = nlohmann::json;

us_server::us_server(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr,
                     int timeout, const std::string& nameArg)
    : _server(loop, listenAddr, nameArg)
    , _manager(loop, timeout)
{
    _server.setConnectionCallback(bind(&us_server::on_connection, this, _1));
    _server.setMessageCallback(bind(&us_server::on_message, this, _1, _2, _3));
    _server.setThreadNum(5);
}

void us_server::start()
{
    _server.start();
}

void us_server::on_connection(const muduo::net::TcpConnectionPtr& conn)
{
    if (!conn->connected()) {
        // TODO clean up service logic
        us_service::instance()->client_close_exception(conn);
        _manager.remove_timeout(conn);
        conn->shutdown();
    }
    else {
        _manager.update_timeout(conn);
    }
}

// -> {code:10006, sequence:"uuid", devId:"xxx-xxx-xxx", time:11234, model:"xxx", game:"xxx",
// msg:[{name:"xxx", data:"xxx"}]}
// <- {code:10007, sequence:"uuid", result:0, devId:"xxx-xxx-xxx", time:1234, game:"xxx",
// msg:[{name:"xxx", data:{index:["x", "y"]}}]}
void us_server::on_message(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buffer,
                           muduo::Timestamp time)
{
    string      buf    = buffer->retrieveAllAsString();
    us_bundle_t bundle = {0, {{"code", 0}}};

    if (buf.empty()) {
        LOG_ERROR << "Receive message is NULL, connection close";
        conn->shutdown();
    }
    else {
        json msg = json::parse(buf.c_str(), nullptr, false);

        if (msg.is_discarded()) {
            LOG_ERROR << "msg is not json, connection close";
            conn->shutdown();
            return;
        }

        LOG_INFO << "(US) "
                 << "[" << pthread_self() << "]"
                 << " <- " << msg.dump();

        _manager.update_timeout(conn);

        auto us_handler = us_service::instance()->handler_us_msg(msg["code"].get<int>());

        if (us_handler == nullptr) {
            LOG_ERROR << "Unsupported code: " << RESULT_CANNOT_EXECUTE;
            bundle.code        = RESULT_CANNOT_EXECUTE;
            bundle.msg["code"] = RESULT_CANNOT_EXECUTE;
        }
        else {
            us_handler(msg, bundle, time);
        }

        if (!bundle.msg.empty() && bundle.code != 0) {
            LOG_INFO << "(US) "
                     << "[ " << pthread_self() << " ]"
                     << " -> " << bundle.msg.dump();
            conn->send(bundle.msg.dump());
        }
    }
}
