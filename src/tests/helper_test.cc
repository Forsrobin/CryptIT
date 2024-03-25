#include <gtest/gtest.h>
#include "helper.h"

TEST(HelperTest, passwordVerificationFileTest)
{
  // DISABLE CONSOLE OUTPUT
  std::cout.setstate(std::ios_base::failbit);
  std::cerr.setstate(std::ios_base::failbit);

  Helper::createPasswordVerificationFile("./", "test.txt");

  ASSERT_EQ(true, Helper::verifyDecryptionPassword("./test.txt"));
  ASSERT_EQ(false, Helper::verifyDecryptionPassword("./test1.txt"));
}

TEST(HelperTest, fileCheckTest)
{

  bool file_check = Helper::checkIfFileExists("./test.txt");

  ASSERT_EQ(true, file_check);

  // Delete the file
  Helper::deleteFile("./test.txt");

  file_check = Helper::checkIfFileExists("./test.txt");

  ASSERT_EQ(false, file_check);
}

TEST(HelperTest, coreTest)
{
  const int numberOfCores = Helper::getNumberOfCores();
  ASSERT_EQ(true, numberOfCores > 0);
}


TEST(HelperTest, loadFilesTest) {
  const fs::path dirPath = "./test_case/";
  std::vector<std::string> files;
  const std::string passwordVerifyFileName = "test.txt";

  ASSERT_EQ(false, Helper::checkIfDirectoryExists(dirPath.string()));

  // Create the directory
  Helper::createDirectory(dirPath.string());
  ASSERT_EQ(true, Helper::checkIfDirectoryExists(dirPath.string()));

  // Load in all the files in the directory
  Helper::loadInitFiles(dirPath, files, passwordVerifyFileName);
  ASSERT_EQ(true, files.size() == 0);

  // Create a random file in the directory and load in all the files in the directory
  std::string randomFile = Helper::generateRandomFile(dirPath.string());
  Helper::loadInitFiles(dirPath, files, passwordVerifyFileName);
  ASSERT_EQ(true, files.size() == 1);

  // Create a pssword verification file
  Helper::createPasswordVerificationFile(dirPath.string(), passwordVerifyFileName);

  // Create a fake password verification file
  Helper::createDirectory(dirPath.string() + "secret/");
  Helper::createPasswordVerificationFile(dirPath.string() + "secret/", passwordVerifyFileName);

  // Clear the directory
  files.clear();

  Helper::loadInitFiles(dirPath, files, passwordVerifyFileName);

  ASSERT_EQ(true, files.size() == 2);
  ASSERT_EQ(true, Helper::checkIfFileExists(dirPath.string() + passwordVerifyFileName));
  ASSERT_EQ(false, Helper::checkIfFileExists(randomFile));
  ASSERT_EQ(true, Helper::checkIfFileExists(dirPath.string() + randomFile));
  ASSERT_EQ(true, Helper::checkIfFileExists(dirPath.string() + "secret/" + passwordVerifyFileName));

  // Delete the directory
  Helper::deleteDirectory(dirPath.string());

  // Check if the directory exists
  ASSERT_EQ(false, Helper::checkIfDirectoryExists(dirPath.string()));
}
