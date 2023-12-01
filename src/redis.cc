/********************************************************************************
 * @author: Wulnut
 * @email: carepdime@gmail.com
 * @date: 2023/11/30 10:14
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "include/redis.h"
#include <muduo/base/Logging.h>

#include <utility>

redis::redis()
    : _set_img_context(nullptr)
{}

redis::~redis()
{
    if (_set_img_context != nullptr) redisFree(_set_img_context);
}

bool redis::connect()
{
    _set_img_context = redisConnect("127.0.0.1", 6379);

    if (_set_img_context == nullptr || _set_img_context->err) {
        if (_set_img_context)
            LOG_ERROR << "Error: " << _set_img_context->err;
        else
            LOG_ERROR << "Cannot allocate redis context";

        return false;
    }


    thread tid([&]() { observer_channel_message(); });
    tid.detach();

    LOG_INFO << "Connect redis-server successful";

    return true;
}

bool redis::set_img(const string& id, const string& message)
{
    auto* reply =
        (redisReply*)redisCommand(_set_img_context, "SET %s %s", id.c_str(), message.c_str());

    if (reply == nullptr) {
        LOG_ERROR << "redis publish failed";
        return false;
    }

    freeReplyObject(reply);

    return true;
}

void redis::observer_channel_message()
{
    redisReply* reply = nullptr;

    while (REDIS_OK == redisGetReply(_set_img_context, (void**)&reply)) {
        if (reply != nullptr && reply->element[2] != nullptr && reply->element[2]->str != nullptr) {
            // 给业务层上报通道上发生的消息
            _notify_message_handler(reply->element[1]->str, reply->element[2]->str);
        }

        freeReplyObject(reply);
    }

    LOG_DEBUG << ">>>>>>>>>>>>> observer_channel_message quit <<<<<<<<<<<<<";
}

void redis::init_nofity_handler(const function<void(string, string)>& nofity_handler)
{
    _notify_message_handler = nofity_handler;
}
