#include <vector>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <windows.h>
#include <dirent.h>

class FileManager {
public:
    FileManager();
    ~FileManager();

    void loadFile(const char* filename);
    void saveFile(const char* filename);
    void deleteFile(const char* filename);
    void printFiles();

    void setFiles(std::vector<std::string> files);
    void createDirectory(const char* path);

    bool checkIfFileExists(std::string _filename);
    bool checkIfDirectoryExists(const char* path);
    
    std::vector<std::string> getFilesRecursivly(const char* path);

private:
    std::vector<std::string> files;

};