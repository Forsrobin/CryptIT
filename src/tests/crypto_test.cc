#include <gtest/gtest.h>
#include "crypto.h"
#include "helper.h"


TEST(CryptoTest, encryptTest)
{

  // DISABLE CONSOLE OUTPUT
  std::cout.setstate(std::ios_base::failbit);
  std::cerr.setstate(std::ios_base::failbit);

  Crypto crypto;
  std::string password = "password";
  std::string file = "./test.txt";
  std::string fileEnc = file + ".enc";

  // Create a file
  std::ofstream out(file);
  out << "test";
  out.close();

  // Encrypt the file
  crypto.encryptFile(file.c_str(), fileEnc.c_str(), password.c_str());

  // Check if the file exists
  bool file_check = Helper::checkIfFileExists(fileEnc);
  ASSERT_EQ(true, file_check);

  // Delete the file
  Helper::deleteFile(file);
  
  ASSERT_EQ(false, Helper::checkIfFileExists(file));
  ASSERT_EQ(true, Helper::checkIfFileExists(fileEnc));

  // Delete the file
  Helper::deleteFile(fileEnc);
  ASSERT_EQ(false, Helper::checkIfFileExists(fileEnc));

}

TEST(CryptoTest, decryptTest)
{

  // DISABLE CONSOLE OUTPUT
  std::cout.setstate(std::ios_base::failbit);
  std::cerr.setstate(std::ios_base::failbit);

  Crypto crypto;
  std::string password = "password";
  std::string file = "./test.txt";
  std::string fileEnc = file + ".enc";

  // Create a file
  std::ofstream out(file);
  out << "test";
  out.close();

  // Encrypt the file
  crypto.encryptFile(file.c_str(), fileEnc.c_str(), password.c_str());
  ASSERT_EQ(true, Helper::checkIfFileExists(fileEnc));
  ASSERT_EQ(true, Helper::checkIfFileExists(file));

  // Delete the file
  Helper::deleteFile(file);
  ASSERT_EQ(false, Helper::checkIfFileExists(file));

  // Decrypt the file
  crypto.decryptFile(fileEnc.c_str(), file.c_str(), password.c_str());
  ASSERT_EQ(true, Helper::checkIfFileExists(fileEnc));
  ASSERT_EQ(true, Helper::checkIfFileExists(file));

  // Delete the file
  Helper::deleteFile(fileEnc);
  ASSERT_EQ(false, Helper::checkIfFileExists(fileEnc));

  Helper::deleteFile(file);
  ASSERT_EQ(false, Helper::checkIfFileExists(file));

}