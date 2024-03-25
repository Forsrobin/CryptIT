#ifndef CRYPTIT_CRYPTO_H
#define CRYPTIT_CRYPTO_H


#include <iostream>
#include <filesystem>
#include <algorithm>
#include <fstream>

#include <cryptopp/files.h>
#include <cryptopp/default.h>

#include "threadpool.h"
#include "helper.h"

class Crypto
{

private:
  std::string passwordVerifyFileName = ".jghdf78tdshui4bjdfy78";
  std::string totalTime = "";
  std::string errorMessage = "";
  bool foundPasswordFile = false;


public:
  std::string directoryPath;
  std::vector<std::string> files;

  ThreadPool *pool = new ThreadPool(1);

  Crypto();
  void processFiles();

  std::vector<std::vector<std::string>> splitVector(const std::vector<std::string> &_files, int numberOfThreads);

  bool encryptFile(const char *fin, const char *fout, const char *passwd);
  void encryptFiles(std::vector<std::string> &files_, const std::string &password);
  void encrypt(std::string password);

  bool decryptFile(const char *fin, const char *fout, const char *passwd);
  void decryptFiles(std::vector<std::string> &files_, const std::string &password);
  void decrypt(std::string password);

  bool foundPasswordFileInDirectory();

  std::string getTotalTimeMessage();
  std::string getErrorMessage();
};

#endif // CRYPTIT_CRYPTO_H
