#include <gtest/gtest.h>
#include "../include/crypto.h"
#include "../include/helper.h"

TEST(HelperTest, passwordVerificationFileTest)
{

  createPasswordVerificationFile("./", "test.txt");

  ASSERT_EQ(true, verifyDecryptionPassword("./test.txt"));
  ASSERT_EQ(false, verifyDecryptionPassword("./test1.txt"));
}

TEST(HelperTest, fileCheckTest)
{

  bool file_check = checkIfFileExists("./test.txt");

  ASSERT_EQ(true, file_check);

  // Delete the file
  deleteFile("./test.txt");

  file_check = checkIfFileExists("./test.txt");

  ASSERT_EQ(false, file_check);
}

TEST(HelperTest, coreTest)
{
  const int numberOfCores = getNumberOfCores();

  ASSERT_EQ(true, numberOfCores > 0);
}