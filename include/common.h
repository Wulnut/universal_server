/********************************************************************************
 * @author: Wulnut
 * @email: carepdime@gmail.com
 * @date: 2023/11/28 17:27
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef UNIVERSAL_SERVER_COMMON_H
#define UNIVERSAL_SERVER_COMMON_H

#include <cstring>
#include <iostream>
#include <openssl/evp.h>
#include <string>

std::string base64_encode(const std::string& buffer);
std::string base64_decode(const std::string& encoded);

typedef enum us_code
{
    CODE_AUTH = 1000,
    CODE_AUTH_ACK,
    CODE_CONNECT,
    CODE_CONNECT_ACK,
    CODE_PHOTO,
    CODE_PHOTO_ACK,
} us_code_t;

typedef enum us_result
{
    RESULT_OK             = 0,
    RESULT_ID_NO_MATCH    = -1000,
    RESULT_CANNOT_EXECUTE = -2000,
} us_result_t;

#endif   // UNIVERSAL_SERVER_COMMON_H
