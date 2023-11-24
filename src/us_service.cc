/********************************************************************************
 * @author: Wulnut
 * @email: carepdime@gmail.com
 * @date: 2023/11/24 17:54
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "include/us_service.h"

us_service::us_service()
{
    LOG_INFO << "Hello World us_service";
}

us_service* us_service::instance()
{
    static us_service service;
    return &service;
}

MsgHandler us_service::getHandler(int msgId)
{
    auto it = _msgHandlerMap.find(msgId);

    if (it == _msgHandlerMap.end()) {
        return [=](const muduo::net::TcpConnectionPtr& conn, json& msg, muduo::Timestamp) {
            LOG_ERROR << "msId: " << msgId << " can not find handler";
        };
    }
    else {
        return _msgHandlerMap[msgId];
    }
}

void us_service::clientCloseException(const muduo::net::TcpConnectionPtr& conn)
{
    // TODO handler exception
}

void us_service::rest()
{
    // TODO reset service server
}
