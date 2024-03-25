#include <gtest/gtest.h>
#include "helper.h"

TEST(HelperTest, passwordVerificationFileTest)
{

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
