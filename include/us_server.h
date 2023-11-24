/********************************************************************************
 * @author: Wulnut
 * @email: carepdime@gmail.com
 * @date: 2023/11/24 15:49
 * @version: 1.0
 * @description: define tcp connection
 ********************************************************************************/
#ifndef UNIVERSAL_SERVER_US_SERVER_H
#define UNIVERSAL_SERVER_US_SERVER_H

#include "include/us_service.h"
#include <muduo/base/LogFile.h>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

using namespace muduo;
using namespace muduo::net;

class us_server
{
public:
    us_server(EventLoop* loop, const InetAddress& listenAddr, const string& nameArg);

    void start();

private:
    void on_connection(const TcpConnectionPtr&);
    void on_message(const TcpConnectionPtr&, Buffer*, Timestamp);

private:
    TcpServer  _server;
    EventLoop* _loop;
};

#endif   // UNIVERSAL_SERVER_US_SERVER_H
