#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void loadInitFiles(const fs::path &dirPath, std::vector<std::string> &files)
{
  try
  {
    for (const auto &entry : fs::directory_iterator(dirPath))
    {
      if (fs::is_regular_file(entry))
      {
        files.push_back(entry.path().string());
      }
      else if (fs::is_directory(entry))
      {
        loadInitFiles(entry.path(), files); // Recursive call for subdirectory
      }
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}