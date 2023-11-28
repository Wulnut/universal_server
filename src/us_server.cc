/********************************************************************************
 * @author: Wulnut
 * @email: carepdime@gmail.com
 * @date: 2023/11/24 15:49
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "include/us_server.h"
#include "include/json.hpp"
#include <iostream>

using namespace std;
using namespace placeholders;
using json = nlohmann::json;

us_server::us_server(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr,
                     const std::string& nameArg)
    : _server(loop, listenAddr, nameArg)
    , _loop(loop)
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
        conn->shutdown();
    }
}

void us_server::on_message(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buffer,
                           muduo::Timestamp time)
{
    string buf = buffer->retrieveAllAsString();

    if (!buf.empty()) {
        json msg = json::parse(buf.c_str(), nullptr, false);

        if (msg.is_discarded()) {
            LOG_ERROR << "msg is not json, connection close";
            goto err;
        }
        LOG_DEBUG << msg.dump();
        // TODO handle service logic
        auto us_handler = us_service::instance()->handler_us_msg(msg["msgId"].get<int>());
        us_handler(msg, time);
        // TODO handle send uniformly
    }

    LOG_ERROR << "Receive message is NULL, connection close";
err:
    conn->shutdown();
}
