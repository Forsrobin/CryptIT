#include "include/crypto.h"

Crypto::Crypto()
{
  std::cout << "Crypto object created" << std::endl;
}

// Load the files in the directory
void Crypto::processFiles()
{

  // Check if the passwordVerifyFile exists
  foundPasswordFile = Helper::checkIfFileExists(directoryPath + passwordVerifyFileName + ".enc");

  // Reset the message label
  fs::path _dir = directoryPath;
  files.clear();
  if (!fs::exists(_dir) && fs::is_directory(_dir))
  {
    std::cerr << "Invalid directory path or directory does not exist." << std::endl;
  }

  Helper::loadInitFiles(_dir, files, passwordVerifyFileName);
}

// ============================================================================
// Crypto functions
// ============================================================================
// Encrypt
// ============================================================================

bool Crypto::encryptFile(const char *fin, const char *fout, const char *passwd)
{
  try
  {
    CryptoPP::FileSource f(fin, true, new CryptoPP::DefaultEncryptor(passwd, new CryptoPP::FileSink(fout)));
    return true;
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return false;
  }
}

void Crypto::encryptFiles(std::vector<std::string> &files_, const std::string &password)
{
  for (const auto &file : files_)
  {

    // If the file has the .enc extension, skip it
    if (file.size() >= 4 && file.substr(file.size() - 4) == ".enc")
    {
      continue;
    }

    std::string outputFilePath = file + ".enc";
    if (!encryptFile(file.c_str(), outputFilePath.c_str(), password.c_str()))
    {
      std::cerr << "Error encrypting file: " << file << std::endl;
    }
    else
    {
      Helper::deleteFile(file);
    }
  }
}

void Crypto::encrypt(std::string password)
{
  // Get the password from the input field
  std::string checkFile = directoryPath + passwordVerifyFileName;
  std::string checkFileOut = directoryPath + passwordVerifyFileName + ".enc";

  if (!Helper::checkIfFileExists(checkFile))
  {
    std::cerr << "Password verification file does not exist, creating one" << std::endl;
    Helper::createPasswordVerificationFile(directoryPath, passwordVerifyFileName);
    // Encrypt the file with the password
    if (!Crypto::encryptFile(checkFile.c_str(), checkFileOut.c_str(), password.c_str()))
    {
      std::cerr << "Error encrypting file: " << checkFile << std::endl;
      return;
    }

    Helper::deleteFile(checkFile);
  }


  // If the number of threads is greater then the number of files, set the number of threads to the number of files
  if (pool->getNumberOfThreads() > static_cast<int>(files.size()))
    pool->setNumberOfThreads(static_cast<int>(files.size()));
  std::vector<std::vector<std::string>> filesSplit = splitVector(files, pool->getNumberOfThreads());

  // Start a timer so we can measure the time it takes to encrypt the files
  auto start = std::chrono::high_resolution_clock::now();

  // Add the files to the threadpool HERE
  for (const auto &filesSubset : filesSplit)
    pool->enqueue(&Crypto::encryptFiles, this, filesSubset, password);

  // Wait for all threads to finish
  pool->clear();
  pool->resetThreads();

  // Stop the timer
  auto end = std::chrono::high_resolution_clock::now();

  auto minutes = std::chrono::duration_cast<std::chrono::minutes>(end - start).count();
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count() - minutes * 60;
  totalTime = std::to_string(minutes) + " minutes and " + std::to_string(seconds) + " seconds";
}

// ============================================================================
// Encrypt
// ============================================================================

bool Crypto::decryptFile(const char *fin, const char *fout, const char *passwd)
{

  try
  {
    CryptoPP::FileSource f(fin, true, new CryptoPP::DefaultDecryptor(passwd, new CryptoPP::FileSink(fout)));
    return true;
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return false;
  }
}

void Crypto::decryptFiles(std::vector<std::string> &files_, const std::string &password)
{
  for (const auto &file : files_)
  {
    // If the file does not have the .enc extension, skip it
    if (file.size() < 4 || file.substr(file.size() - 4) != ".enc")
    {
      continue;
    }

    // Remove the .enc extension
    std::string outputFilePath = file.substr(0, file.size() - 4);
    if (!decryptFile(file.c_str(), outputFilePath.c_str(), password.c_str()))
    {
      std::cerr << "Error decrypting file: " << file << std::endl;
    }
    else
    {
      Helper::deleteFile(file);
    }
  }
}

void Crypto::decrypt(std::string password)
{
  errorMessage = "";

  // Get the password from the input field
  std::string checkFile = directoryPath + passwordVerifyFileName + ".enc";
  std::string checkFileOut = directoryPath + passwordVerifyFileName;

  // Check if the passwordVerifyFile exists
  if (!Helper::checkIfFileExists(checkFile))
  {
    std::cerr << "Password verification file does not exist, select a directory that has been encrypted before!" << std::endl;
    return;
  }

  // Try to decrypt the file
  if (!decryptFile(checkFile.c_str(), checkFileOut.c_str(), password.c_str()))
  {
    std::cerr << "Error decrypting checkfile: " << directoryPath << std::endl;
    Helper::deleteFile(checkFileOut);
    errorMessage = "Wrong password! Please try again";
    return;
  }

  std::cerr << "Password verified, deleting the password verification files" << std::endl;
  Helper::deleteFile(checkFileOut);
  Helper::deleteFile(checkFile);

  // If the number of threads is greater then the number of files, set the number of threads to the number of files
  if (pool->getNumberOfThreads() > static_cast<int>(files.size()))
    pool->setNumberOfThreads(static_cast<int>(files.size()));
  std::vector<std::vector<std::string>> filesSplit = splitVector(files, pool->getNumberOfThreads());

  // Start a timer so we can measure the time it takes to encrypt the files
  auto start = std::chrono::high_resolution_clock::now();

  // Add the files to the threadpool HERE
  for (const auto &filesSubset : filesSplit)
    pool->enqueue(&Crypto::decryptFiles, this, filesSubset, password);

  // Wait for all threads to finish
  pool->clear();
  pool->resetThreads();

  // Stop the timer
  auto end = std::chrono::high_resolution_clock::now();

  auto minutes = std::chrono::duration_cast<std::chrono::minutes>(end - start).count();
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count() - minutes * 60;
  totalTime = std::to_string(minutes) + " minutes and " + std::to_string(seconds) + " seconds";
}

std::vector<std::vector<std::string>> Crypto::splitVector(const std::vector<std::string> &_files, int numberOfThreads)
{
  std::vector<std::vector<std::string>> filesSplit(numberOfThreads);

  int i = 0;
  for (const auto &file : _files)
  {
    filesSplit[i % numberOfThreads].push_back(file);
    i++;
  }

  return filesSplit;
}

bool Crypto::foundPasswordFileInDirectory()
{
  return foundPasswordFile;
}

std::string Crypto::getTotalTimeMessage()
{
  
  return totalTime;
}

std::string Crypto::getErrorMessage()
{
  return errorMessage;
}