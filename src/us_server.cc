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
#include <sys/sysinfo.h>

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

    LOG_DEBUG << "System enable num is " << get_nprocs();
    _server.setThreadNum(get_nprocs() * 2 - 1);   // No more than twice the CPU core
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
        LOG_DEBUG << "connection:" << conn->name();
    }
}

void us_server::on_message(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buffer,
                           muduo::Timestamp time)
{
    string      buf    = buffer->retrieveAllAsString();
    us_bundle_t bundle = {nullptr, "", {{"code", 0}}};
    _manager.update_timeout(conn);
    bundle.conn = conn;

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

        auto us_handler = us_service::instance()->handler_us_msg(msg["code"].get<int>());

        if (us_handler == nullptr) {
            LOG_ERROR << "Unsupported code: " << RESULT_CANNOT_EXECUTE;
            bundle.msg["code"] = RESULT_CANNOT_EXECUTE;
        }
        else {
            us_handler(msg, bundle, time);
        }

        if (!bundle.msg.empty() && bundle.conn != nullptr) {
            LOG_INFO << "(US) "
                     << "[" << pthread_self() << "]"
                     << " -> " << bundle.msg.dump();
            conn->send(bundle.msg.dump());
        }
        else {
            if (bundle.conn == nullptr) LOG_ERROR << "(US) send error conn is NULL";
            if (bundle.msg.empty()) LOG_ERROR << "(US) send error msg is NULL";
            conn->shutdown();
        }
    }
}
