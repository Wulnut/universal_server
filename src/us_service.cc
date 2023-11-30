/********************************************************************************
 * @author: Wulnut
 * @email: carepdime@gmail.com
 * @date: 2023/11/24 17:54
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "include/us_service.h"
#include "include/common.h"

using namespace muduo;
using namespace muduo::net;
using namespace std;
using namespace std::placeholders;

us_service::us_service()
{
    _us_handlers.insert({CODE_CONNECT, bind(&us_service::handler_connect_ack, this, _1, _2, _3)});
    _us_handlers.insert({CODE_PHOTO, bind(&us_service::handler_photo_ack, this, _1, _2, _3)});

    if (_redis.connect())
        _redis.init_nofity_handler(bind(&us_service::redis_subscribe_msg_handler, this, _1, _2));
}

us_service* us_service::instance()
{
    static us_service service;
    return &service;
}

us_handle_t us_service::handler_us_msg(int code)
{
    auto it = _us_handlers.find(code);

    if (it == _us_handlers.end()) {
        return [=](json& msg, us_bundle_t& bundle, Timestamp) {
            LOG_ERROR << "code: " << code << " can not find handler";
        };
    }
    else {
        return _us_handlers[code];
    }
}

void us_service::handler_photo_ack(json& msg, us_bundle_t& bundle, muduo::Timestamp)
{
    // TODO handler photo
    int    code = 0;
    string tmp;
    string data;
    string sequence;

    try {
        code = msg["code"].get<int>();
    }
    catch (out_of_range& e) {
        LOG_ERROR << e.what();
    }

    try {
        sequence = msg["sequence"].get<string>();
    }
    catch (out_of_range& e) {
        LOG_ERROR << e.what();
    }

    try {
        tmp = msg["data"].get<string>();
    }
    catch (out_of_range& e) {
        LOG_ERROR << e.what();
    }

    data = base64_decode(tmp);

    _redis.publish(sequence, data);
}

void us_service::handler_connect_ack(json& msg, us_bundle_t& bundle, muduo::Timestamp)
{
    int    code = 0;
    string data;

    try {
        code = msg["code"].get<int>();
    }
    catch (out_of_range& e) {
        LOG_ERROR << e.what();
    }

    try {
        data = msg["data"].get<string>();
    }
    catch (out_of_range& e) {
        LOG_ERROR << e.what();
    }

    bundle.code        = code + 1;
    bundle.msg["code"] = code + 1;
    bundle.msg["data"] = data;
}

void us_service::client_close_exception(const TcpConnectionPtr& conn)
{
    // TODO handler exception
}

void us_service::rest()
{
    // TODO reset service server
}

void us_service::redis_subscribe_msg_handler(const string&, const string&)
{
    // TODO
}
