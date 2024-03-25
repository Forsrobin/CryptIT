#include <gtest/gtest.h>
#include "crypto.h"
#include "helper.h"


TEST(CryptoTest, encryptTest)
{

  // DISABLE CONSOLE OUTPUT
  std::cout.setstate(std::ios_base::failbit);
  std::cerr.setstate(std::ios_base::failbit);

  const fs::path dirPath = "./";
  std::vector<std::string> files;
  const std::string passwordVerifyFileName = "test.txt";

  Helper::loadInitFiles(dirPath, files, passwordVerifyFileName);

  Helper::createPasswordVerificationFile("./", "test.txt");
  Crypto crypto;

  ASSERT_EQ(true, Helper::checkIfFileExists("./test.txt"));

  bool success = crypto.encryptFile("./test.txt", "./test.txt.enc", "test");
  bool no_success = crypto.encryptFile("./testdas.txt", "./testdas.txt.enc", "test");

  ASSERT_EQ(true, success);
  ASSERT_EQ(false, no_success);

  // 


}