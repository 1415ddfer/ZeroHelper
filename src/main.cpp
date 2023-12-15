#include <QApplication>
//#include "gameLogic/calculationPower.h"
#include "loginWindow/MainFrame.h"
#include "network/TcpControlApi.h"
#include <windows.h>
#include <openssl/evp.h>
#include <openssl/aes.h>


//void testCal() {
//    for (int i = 1; i <= 20; i++) {
//        auto x = calForce(0, 65, i, i);
//        if (x.has_value())
//            std::cout << "dx:" << i << "\t" << x.value() << std::endl;
//        else
//            std::cout << "dx:" << i << "\t" << "no solution" << std::endl;
//    }
//}
//
//char* hex_encode(unsigned char* data, int len) {
//    char* hex = (char*)malloc(len * 2 + 1);
//    for (int i = 0; i < len; i++) {
//        sprintf(hex + i * 2, "%02x", data[i]);
//    }
//    hex[len * 2] = '\0';
//    return hex;
//}
//
//// 使用openssl/evp.h进行AES-256-ECB加密
//char* aes_encrypt(const char* plaintext, const char* key) {
//    // 创建EVP_CIPHER_CTX对象
//    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
//    if (ctx == NULL) {
//        printf("Failed to create cipher context\n");
//        return NULL;
//    }
//    // 初始化加密上下文，指定加密算法和密钥
//    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, (unsigned char*)key, NULL) != 1) {
//        printf("Failed to initialize encryption context\n");
//        EVP_CIPHER_CTX_free(ctx);
//        return NULL;
//    }
//    // 计算加密后数据的长度
//    int len = strlen(plaintext) + EVP_CIPHER_block_size(EVP_aes_256_ecb()) - 1;
//    len -= len % EVP_CIPHER_block_size(EVP_aes_256_ecb());
//    // 分配输出缓冲区
//    unsigned char* cipherdata = (unsigned char*)malloc(len);
//    memset(cipherdata, 0, len);
//    // 加密数据
//    int outlen = 0;
//    if (EVP_EncryptUpdate(ctx, cipherdata, &outlen, (unsigned char*)plaintext, strlen(plaintext)) != 1) {
//        printf("Failed to encrypt data\n");
//        free(cipherdata);
//        EVP_CIPHER_CTX_free(ctx);
//        return NULL;
//    }
//    // 获取加密结果
//    int lastlen = 0;
//    if (EVP_EncryptFinal_ex(ctx, cipherdata + outlen, &lastlen) != 1) {
//        printf("Failed to finalize encryption\n");
//        free(cipherdata);
//        EVP_CIPHER_CTX_free(ctx);
//        return NULL;
//    }
//    // 计算加密后数据的总长度
//    int totallen = outlen + lastlen;
//    // 将加密后的数据转换为十六进制字符串
//    char* ciphertext = hex_encode(cipherdata, totallen);
//    // 释放缓冲区和上下文对象
//    free(cipherdata);
//    EVP_CIPHER_CTX_free(ctx);
//    // 返回加密后的密文
//    return ciphertext;
//}
//
//



int main(int argc, char *argv[])
{

    SetProcessDPIAware();
    streamer.init();
    QApplication app(argc, argv);
    auto api = ApiServer();
    auto test = MainFrame();
    test.show();
    QApplication::exec();
    return 0;
}
//
//void test_aes() {
//    auto plaintext = QString("123");
//    auto key = QString("lzYW5qaXVqa");
//
//    auto* ciphertext = new unsigned char[plaintext.size() + AES_BLOCK_SIZE];
//    EVP_CIPHER_CTX *ctx;
//    ctx = EVP_CIPHER_CTX_new();
//
//    auto ba_k = key.toLatin1();
//    EVP_EncryptInit_ex(
//            ctx,
//            EVP_aes_128_cbc(),
//            nullptr,
//            (unsigned char*)ba_k.data(),
//            nullptr);
//
//    int len;
//    auto ba = plaintext.toLatin1();
//    EVP_EncryptUpdate(ctx, ciphertext, &len,
//                      (unsigned char *)ba.data(), (int)plaintext.size());
//    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
//
//    EVP_CIPHER_CTX_free(ctx);
//    auto ret = QByteArray((char *)ciphertext);
//    qDebug() << ret.toBase64();
//
//    delete[] ciphertext;
//}
