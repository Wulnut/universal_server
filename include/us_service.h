/********************************************************************************
 * @author: Wulnut
 * @email: carepdime@gmail.com
 * @date: 2023/11/24 17:54
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef UNIVERSAL_SERVER_US_SERVICE_H
#define UNIVERSAL_SERVER_US_SERVICE_H

#include "include/json.hpp"
#include "muduo/base/Logging.h"
#include <functional>
#include <muduo/net/TcpServer.h>
#include <mutex>
#include <unordered_map>

using json = nlohmann::json;
using MsgHandler =
    std::function<void(const muduo::net::TcpConnectionPtr& conn, json& msg, muduo::Timestamp)>;

class us_service
{
public:
    static us_service* instance();
    MsgHandler         getHandler(int msgId);
    void               clientCloseException(const muduo::net::TcpConnectionPtr& conn);
    void               rest();

private:
    us_service();

    std::unordered_map<int, MsgHandler>                   _msgHandlerMap;
    std::unordered_map<int, muduo::net::TcpConnectionPtr> _userConnMap;
    std::mutex                                            _connMutex;
};



#endif   // UNIVERSAL_SERVER_US_SERVICE_H
