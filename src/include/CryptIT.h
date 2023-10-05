#include <stdio.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <windows.h>
#include <fstream>
#include "FileManager.h"

class CryptIT {
public:
    FileManager fileManager;
    
    CryptIT();
    ~CryptIT();

    void setPassword();
    void generateRSAKeys();
    bool checkIfKeysExsist();
    bool vildatePasswordWithPrivateKey();

    std::string encrypt(std::string str);
    std::string decrypt(std::string str);

    std::string getPassword();

private:
    std::string password;
    std::string keyPath;
};