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
#include "include/redis.h"
#include <functional>
#include <muduo/base/Logging.h>
#include <muduo/net/TcpServer.h>
#include <mutex>
#include <unordered_map>

using json = nlohmann::json;

typedef struct us_bundle
{
    int  code;
    json msg;
} us_bundle_t;

using us_handle_t = std::function<void(json&, us_bundle_t&, muduo::Timestamp)>;

class us_service
{
public:
    static us_service* instance();
    us_handle_t        handler_us_msg(int code);

    void handler_photo_ack(json& msg, us_bundle_t& bundle, muduo::Timestamp);
    void handler_connect_ack(json& msg, us_bundle_t& bundle, muduo::Timestamp);

    void client_close_exception(const muduo::net::TcpConnectionPtr& conn);
    void rest();

    void redis_subscribe_msg_handler(const string&, const string&);

private:
    us_service();

    std::unordered_map<int, us_handle_t>                  _us_handlers;
    std::unordered_map<int, muduo::net::TcpConnectionPtr> _us_users_conn_handlers;
    std::mutex                                            _conn_mutex;
    redis                                                 _redis;
};



#endif   // UNIVERSAL_SERVER_US_SERVICE_H
