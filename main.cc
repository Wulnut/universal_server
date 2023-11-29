/********************************************************************************
 * @author: Wulnut
 * @email: carepdime@gmail.com
 * @date: 2023/11/16 11:35
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "include/us_server.h"
#include <csignal>
#include <iostream>

using namespace std;

void reset_handler(int)
{
    // TODO service reset
    exit(0);
}

int main(int argc, char** argv)
{
    char*    ip     = nullptr;
    char*    endptr = nullptr;
    uint16_t port   = 0;

    if (argc < 3) {
        printf("Command invalid! example: ./us_server 127.0.0.1 6000\n");
        exit(-1);
    }

    ip   = argv[1];
    port = strtol(argv[2], &endptr, 10);

    if (ip == nullptr) {
        printf("ip error\n");
        exit(-1);
    }

    if (*endptr != '\0') {
        printf("Conversion error, non-convertible part: %s\n", endptr);
        exit(-1);
    }

    signal(SIGINT, reset_handler);
    Logger::setLogLevel(Logger::DEBUG);

    EventLoop   loop;
    InetAddress addr(ip, port);
    us_server   server(&loop, addr, 10, "us_server");

    server.start();
    loop.loop();

    return 0;
}