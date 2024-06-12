#ifndef AESCRYPTOGRAPHY_H
#define AESCRYPTOGRAPHY_H

#include "ICryptography.h"
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>

class AESCryptography : public ICryptography {
public:
    std::vector<unsigned char> encrypt(const std::string& plaintext, const std::string& key, const std::string& iv) override;
    std::string decrypt(const std::vector<unsigned char>& ciphertext, const std::string& key, const std::string& iv) override;
};

#endif // AESCRYPTOGRAPHY_H
