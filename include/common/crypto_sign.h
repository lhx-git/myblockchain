//
// Created by peng on 2021/3/24.
//

#ifndef NEUBLOCKCHAIN_CRYPTO_SIGN_H
#define NEUBLOCKCHAIN_CRYPTO_SIGN_H

#include <string>
#include <openssl/evp.h>

class CryptoSign {
public:
    explicit CryptoSign(const std::string& pubKeyFile,
                        const std::string& pivKeyFile,
                        const std::string& password = "");
    //用私要对message加密, 并生成签名
    bool rsaEncrypt(const std::string& message, std::string* signature);
    //用公钥对签名解密,并与message对比, 验证签名
    bool rsaDecrypt(const std::string& message, const std::string& signature) const;
    //生成公钥 私要文件
    static bool generateKeyFiles(const char *pub_keyfile, const char *pri_keyfile,
                          const unsigned char *passwd, int passwd_len);

protected:
    //读文件,load私要
    static EVP_PKEY* openPrivateKey(const char *keyfile, const unsigned char *passwd);
    //写文件,load公钥
    static EVP_PKEY* openPublicKey(const char *keyfile);
    static void printDigest(const std::string& signature);

private:
    EVP_PKEY *pubKey;
    EVP_PKEY *pivKey;
    EVP_MD_CTX* mdCtx; //摘要上下文
};


#endif //NEUBLOCKCHAIN_CRYPTO_SIGN_H
