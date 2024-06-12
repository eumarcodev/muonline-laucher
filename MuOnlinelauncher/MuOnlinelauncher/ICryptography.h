#ifndef ICRYPTOGRAPHY_H
#define ICRYPTOGRAPHY_H

class ICryptography {
public:
    virtual ~ICryptography() = default;
    virtual std::vector<unsigned char> encrypt(const std::string& plaintext, const std::string& key, const std::string& iv) = 0;
    virtual std::string decrypt(const std::vector<unsigned char>& ciphertext, const std::string& key, const std::string& iv) = 0;
};

#endif // ICRYPTOGRAPHY_H