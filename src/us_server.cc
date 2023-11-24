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
        us_service::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

void us_server::on_message(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buffer,
                           muduo::Timestamp time)
{
    string buf = buffer->retrieveAllAsString();
    json   msg = json::parse(buf);

    LOG_DEBUG << msg.dump();
    // TODO handle service logic
    auto msgHandler = us_service::instance()->getHandler(msg["msgId"].get<int>());
    msgHandler(conn, msg, time);
}
