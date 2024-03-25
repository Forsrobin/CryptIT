#ifndef CRYPTIT_HELPER_H
#define CRYPTIT_HELPER_H

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;

namespace Helper
{
    void loadInitFiles(const fs::path &dirPath, std::vector<std::string> &files, const std::string &passwordVerifyFileName);
    void deleteFile(const std::string &filePath);
    void deleteDirectory(const std::string &dirPath);
    void createPasswordVerificationFile(const std::string &dirPath, const std::string &passwordVerifyFileName);
    
    int getNumberOfCores();
    
    bool verifyDecryptionPassword(const std::string &checkFile);
    bool checkIfFileExists(const std::string &filePath);
    bool checkIfDirectoryExists(const std::string &dirPath);
    bool createDirectory(const std::string &dirPath);

    std::string generateRandomFile(std::string dirPath);
};

#endif // CRYPTIT_HELPER_H