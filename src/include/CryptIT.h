#include <stdio.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <windows.h>

class CryptIT {
public:
    CryptIT();
    ~CryptIT();

    void setPassword();

    std::string encrypt(std::string str);
    std::string decrypt(std::string str);

    std::string getPassword();

private:
    std::string password;
};