#include "pch.h"
#include "AESCryptography.h"
#include <cryptopp/hex.h>
#include <cryptopp/cryptlib.h>
#include <stdexcept>

std::vector<unsigned char> AESCryptography::encrypt(const std::string& plaintext, const std::string& key, const std::string& iv) {
    std::vector<unsigned char> ciphertext;
    try {
        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryption((const byte*)key.data(), key.size(), (const byte*)iv.data());

        CryptoPP::StringSource ss(plaintext, true,
            new CryptoPP::StreamTransformationFilter(encryption,
                new CryptoPP::VectorSink(ciphertext)
            )
        );
    }
    catch (const CryptoPP::Exception& e) {
        throw std::runtime_error(e.what());
    }
    return ciphertext;
}

std::string AESCryptography::decrypt(const std::vector<unsigned char>& ciphertext, const std::string& key, const std::string& iv) {
    std::string decryptedtext;
    try {
        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption((const byte*)key.data(), key.size(), (const byte*)iv.data());

        CryptoPP::StringSource ss(ciphertext.data(), ciphertext.size(), true,
            new CryptoPP::StreamTransformationFilter(decryption,
                new CryptoPP::StringSink(decryptedtext)
            )
        );
    }
    catch (const CryptoPP::Exception& e) {
        throw std::runtime_error(e.what());
    }
    return decryptedtext;
}
