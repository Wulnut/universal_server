/********************************************************************************
 * @author: Wulnut
 * @email: carepdime@gmail.com
 * @date: 2023/11/30 18:08
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "include/common.h"

std::string base64_encode(const std::string& buffer)
{
    // 创建一个内存 BIO，用于结果的存储
    BIO* bio      = BIO_new(BIO_s_mem());
    BIO* b64      = BIO_new(BIO_f_base64());
    bio           = BIO_push(b64, bio);
    char* encoded = nullptr;
    long  len     = 0;

    // 确保不自动换行
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    // 将数据写入 BIO
    BIO_write(bio, buffer.c_str(), (int)buffer.length());
    BIO_flush(bio);

    // 从内存 BIO 读取编码后的数据
    len = BIO_get_mem_data(bio, &encoded);
    std::string result(encoded, len);

    // 释放 BIO
    BIO_free_all(bio);

    return result;
}

std::string base64_decode(const std::string& encoded)
{
    BIO*          bio            = nullptr;
    BIO*          b64            = nullptr;
    int           decoded_length = 0;
    unsigned char buffer[encoded.length()];
    memset(buffer, 0, sizeof(buffer));

    bio = BIO_new_mem_buf(encoded.c_str(), -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    decoded_length = BIO_read(bio, buffer, (int)encoded.length());
    std::string decoded((char*)buffer, decoded_length);

    BIO_free_all(bio);
    return decoded;
}