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
    char*    ip   = NULL;
    uint16_t port = 0;

    if (argc < 3) {
        printf("Command invalid! example: ./us_server 127.0.0.1 6000\n");
        exit(-1);
    }

    ip   = argv[1];
    port = atoi(argv[2]);

    signal(SIGINT, reset_handler);
    Logger::setLogLevel(Logger::DEBUG);

    EventLoop   loop;
    InetAddress addr(ip, port);
    us_server   server(&loop, addr, "us_server");

    server.start();
    loop.loop();

    return 0;
}