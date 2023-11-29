/********************************************************************************
 * @author: Wulnut
 * @email: carepdime@gmail.com
 * @date: 2023/11/29 15:08
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef UNIVERSAL_SERVER_INACTIVE_CONN_MGR_H
#define UNIVERSAL_SERVER_INACTIVE_CONN_MGR_H

#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <unordered_map>

using namespace muduo;
using namespace muduo::net;

class inactive_conn_mgr
{
public:
    inactive_conn_mgr(EventLoop* loop, int timeout);
    void update_timeout(const TcpConnectionPtr& conn);
    void remove_timeout(const TcpConnectionPtr& conn);

private:
    void check_connections();

    int                                             _timeout;
    EventLoop*                                      _loop;
    MutexLock                                       _mutex;
    std::unordered_map<TcpConnectionPtr, Timestamp> _active_connections;
};



#endif   // UNIVERSAL_SERVER_INACTIVE_CONN_MGR_H
