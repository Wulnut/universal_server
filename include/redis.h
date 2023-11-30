/********************************************************************************
 * @author: Wulnut
 * @email: carepdime@gmail.com
 * @date: 2023/11/30 10:14
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef UNIVERSAL_SERVER_REDIS_H
#define UNIVERSAL_SERVER_REDIS_H

#include <functional>
#include <hiredis/hiredis.h>
#include <thread>

using namespace std;

class redis
{
public:
    redis();
    ~redis();

    bool connect();

    bool publish(int sequence, const string& message);

    bool subscribe(int sequence);

    bool unsubscribe(int sequence);

    void observer_channel_message();

    void init_nofity_handler(function<void(int, string)> nofity_handler);

private:
    redisContext* _public_context;
    redisContext* _subscribe_context;

    function<void(int, string)> _notify_message_handler;
};

#endif   // UNIVERSAL_SERVER_REDIS_H
