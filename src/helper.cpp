
#include "helper.h"

void Helper::loadInitFiles(const fs::path &dirPath, std::vector<std::string> &files, const std::string &passwordVerifyFileName)
{
  try
  {
    for (const auto &entry : fs::directory_iterator(dirPath))
    {
      if (fs::is_regular_file(entry))
      {
        // Get only the filename, not the full path
        const std::string filename = "/" + entry.path().filename().string();

        if (filename != passwordVerifyFileName && filename != passwordVerifyFileName + ".enc")
        {
          files.push_back(entry.path().string());
        }
      }
      else if (fs::is_directory(entry))
      {
        Helper::loadInitFiles(entry.path(), files, passwordVerifyFileName); // Recursive call for subdirectory
      }
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void Helper::deleteFile(const std::string &filePath)
{
  if (fs::exists(filePath))
  {
    fs::remove(filePath);
  }
  else
  {
    std::cerr << "File does not exist" << std::endl;
  }
}

void Helper::createPasswordVerificationFile(const std::string &dirPath, const std::string &passwordVerifyFileName)
{
  std::ofstream file(dirPath + passwordVerifyFileName);
  if (file.is_open())
  {
    file << "test";
    file.close();
  }
  else
  {
    std::cerr << "Error creating password verification file" << std::endl;
  }
}

bool Helper::verifyDecryptionPassword(const std::string &checkFile)
{
  std::ifstream file(checkFile);
  if (file.is_open())
  {
    std::string storedData;
    file >> storedData;
    file.close();
    return storedData == "test";
  }
  return false;
}

bool Helper::checkIfFileExists(const std::string &filePath)
{
  return fs::exists(filePath);
}

int Helper::getNumberOfCores()
{
  return std::thread::hardware_concurrency();
}

std::string Helper::generateRandomFile(std::string dirPath) {
  std::string filename = "test" + std::to_string(rand() % 1000) + ".txt";
  std::ofstream file(dirPath + filename);
  // If the directory does not exist, create it
  if (!fs::exists(dirPath))
  {
    Helper::createDirectory(dirPath);
  }
  
  if (file.is_open())
  {
    file << "test";
    file.close();
  }
  else
  {
    std::cerr << "Error creating password verification file" << std::endl;
  }
  return filename;
}

bool Helper::createDirectory(const std::string &dirPath)
{
  if (!fs::exists(dirPath))
  {
    fs::create_directory(dirPath);
    return true;
  }
  return false;
}