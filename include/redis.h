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

    bool publish(const string& sequence, const string& message);

    bool subscribe(const string& sequence);

    bool unsubscribe(const string& sequence);

    void observer_channel_message();

    void init_nofity_handler(const function<void(string, string)>& nofity_handler);

private:
    redisContext* _public_context;
    redisContext* _subscribe_context;

    function<void(string, string)> _notify_message_handler;
};

#endif   // UNIVERSAL_SERVER_REDIS_H
