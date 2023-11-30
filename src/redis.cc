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
    : _public_context(nullptr)
    , _subscribe_context(nullptr)
{}

redis::~redis()
{
    if (_public_context != nullptr) redisFree(_public_context);
    if (_subscribe_context != nullptr) redisFree(_subscribe_context);
}

bool redis::connect()
{
    _public_context = redisConnect("127.0.0.1", 6379);

    if (_public_context == nullptr || _public_context->err) {
        if (_public_context)
            LOG_ERROR << "Error: " << _public_context->err;
        else
            LOG_ERROR << "Cannot allocate redis context";

        return false;
    }

    _subscribe_context = redisConnect("127.0.0.1", 6397);

    if (_subscribe_context == nullptr || _subscribe_context->err) {
        if (_subscribe_context)
            LOG_ERROR << "Error: " << _subscribe_context->err;
        else
            LOG_ERROR << "Cannot allocate redis context";

        return false;
    }

    thread tid([&]() { observer_channel_message(); });
    tid.detach();

    LOG_INFO << "Connect redis-server successful";

    return true;
}

bool redis::publish(int sequence, const std::string& message)
{
    auto* reply =
        (redisReply*)redisCommand(_public_context, "Publish %d %s", sequence, message.c_str());

    if (reply == nullptr) {
        LOG_ERROR << "redis publish failed";
        return false;
    }

    freeReplyObject(reply);

    return true;
}

bool redis::subscribe(int sequence)
{
    int done = 0;

    if (REDIS_ERR == redisAppendCommand(_subscribe_context, "Subscribe %d", sequence)) {
        LOG_ERROR << "redis subscribe failed";
        return false;
    }

    while (!done) {
        if (REDIS_ERR == redisBufferWrite(_subscribe_context, &done)) {
            LOG_ERROR << "redis subscribe failed";
            return false;
        }
    }

    return true;
}

bool redis::unsubscribe(int sequence)
{
    int done = 0;

    if (REDIS_ERR == redisAppendCommand(_subscribe_context, "Unsubscribe %d", sequence)) {
        LOG_ERROR << "redis unsubscribe failed";
        return false;
    }

    while (!done) {
        if (REDIS_ERR == redisBufferWrite(_subscribe_context, &done)) {
            LOG_ERROR << "redis subscribe failed";
            return false;
        }
    }

    return true;
}

void redis::observer_channel_message()
{
    redisReply* reply  = nullptr;
    char*       endptr = nullptr;

    while (REDIS_OK == redisGetReply(_subscribe_context, (void**)&reply)) {
        // TODO
        if (reply != nullptr && reply->element[2] != nullptr && reply->element[2]->str != nullptr) {
            // 给业务层上报通道上发生的消息
            _notify_message_handler((int)strtol(reply->element[1]->str, &endptr, 10),
                                    reply->element[2]->str);

            if (*endptr != '\0') {
                printf("Conversion error, non-convertible part: %s\n", endptr);
                return;
            }
        }

        freeReplyObject(reply);
    }

    LOG_DEBUG << ">>>>>>>>>>>>> observer_channel_message quit <<<<<<<<<<<<<";
}

void redis::init_nofity_handler(function<void(int, std::string)> nofity_handler)
{
    _notify_message_handler = std::move(nofity_handler);
}
