/********************************************************************************
 * @author: Wulnut
 * @email: carepdime@gmail.com
 * @date: 2023/11/29 15:08
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "include/inactive_conn_mgr.h"

inactive_conn_mgr::inactive_conn_mgr(muduo::net::EventLoop* loop, int timeout)
    : _loop(loop)
    , _timeout(timeout)
{
    _loop->runEvery(_timeout, std::bind(&inactive_conn_mgr::check_connections, this));
}

void inactive_conn_mgr::update_timeout(const muduo::net::TcpConnectionPtr& conn)
{
    MutexLockGuard lock(_mutex);
    _active_connections[conn] = _loop->pollReturnTime();
}

void inactive_conn_mgr::remove_timeout(const muduo::net::TcpConnectionPtr& conn)
{
    MutexLockGuard lock(_mutex);
    _active_connections.erase(conn);
}

void inactive_conn_mgr::check_connections()
{
    MutexLockGuard lock(_mutex);
    Timestamp      now = Timestamp::now();

    for (auto it = _active_connections.begin(); it != _active_connections.end();) {
        double time = timeDifference(now, it->second);

        if (time > _timeout) {
            it->first->shutdown();
            it = _active_connections.erase(it);
        }
        else {
            ++it;
        }
    }
}