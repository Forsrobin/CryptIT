#include <vector>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <windows.h>
#include <dirent.h>

// Blacklisted directories and files
class Blacklist
{
public:
  std::vector<std::string> directories;
  std::vector<std::string> files;

  Blacklist()
  {
    // Default blacklisted directories and files
    directories.push_back(".data");
    
    files.push_back("CryptIT.exe");
  }
};

class FileManager
{
public:
  FileManager();
  ~FileManager();

  void deleteFile(const char *filename);
  void renameFile(const char *oldname, const char *newname);
  void printFiles();

  std::vector<std::string> getFiles();

  void setFiles(std::vector<std::string> files);
  void createDirectory(const char *path);

  bool checkIfFileIsBlacklisted(const char *filename);
  bool checkIfDirectoryIsBlacklisted(const char *path);

  bool checkIfFileExists(std::string _filename);
  bool checkIfDirectoryExists(const char *path);

  std::vector<std::string> getFilesRecursivly(const char *path);

private:
  std::vector<std::string> files;
  Blacklist blacklist;
};