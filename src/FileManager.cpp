#include "FileManager.h"

// Constructor
FileManager::FileManager()
{
    // Set the files vector
    files = std::vector<std::string>();
}

// Destructor
FileManager::~FileManager()
{
    // Clear the files vector
    files.clear();
}

// Get files recursivly from the directory given
std::vector<std::string> FileManager::getFilesRecursivly(const char* path)
{
    // Create a pointer to the directory
    DIR* dirp = opendir(path);
    struct dirent * dp;
    // If the directory pointer is not null
    if (dirp != NULL) {
        // While the directory pointer is not null
        while ((dp = readdir(dirp)) != NULL) {
            // If the file is a directory
            if (dp->d_type == DT_DIR) {
                // If the file is not the current directory
                if (strcmp(dp->d_name, ".") != 0) {
                    // If the file is not the parent directory
                    if (strcmp(dp->d_name, "..") != 0) {
                        // Create a new path
                        char* newPath = (char*)malloc(strlen(path) + strlen(dp->d_name) + 2);
                        strcpy(newPath, path);
                        strcat(newPath, "/");
                        strcat(newPath, dp->d_name);
                        // Get the files from the new path
                        getFilesRecursivly(newPath);
                        // Free the memory
                        free(newPath);
                    }
                }
            } else {
                // Create a new path
                char* newPath = (char*)malloc(strlen(path) + strlen(dp->d_name) + 2);
                strcpy(newPath, path);
                strcat(newPath, "/");
                strcat(newPath, dp->d_name);
                // Add the file to the vector
                files.push_back(newPath);
                // Free the memory
                free(newPath);
            }
        }
        // Close the directory
        closedir(dirp);
    }
    // Return the vector
    return files;
}

// Set Files
void FileManager::setFiles(std::vector<std::string> files)
{
    this->files = files;
}

// Print files
void FileManager::printFiles()
{
    for (int i = 0; i < files.size(); i++) {
        std::cout << files[i] << std::endl;
    }
}

// Check if a file exists
bool FileManager::checkIfFileExists(std::string _filename)
{
    const char* filename = _filename.c_str();
    // Create a file pointer
    FILE* file;
    // Open the file
    file = fopen(filename, "r");
    // If the file is not null
    if (file != NULL) {
        // Close the file
        fclose(file);
        // Return true
        return true;
    }
    // Return false
    return false;
}

// Create directory
void FileManager::createDirectory(const char* path)
{
    // Create a directory
    mkdir(path);
}

// Check if a directory exists
bool FileManager::checkIfDirectoryExists(const char* path)
{
    // Create a directory pointer
    DIR* dirp = opendir(path);
    // If the directory pointer is not null
    if (dirp != NULL) {
        // Close the directory
        closedir(dirp);
        // Return true
        return true;
    }
    // Return false
    return false;
}

// Delete file
void FileManager::deleteFile(const char* filename)
{
    // Delete the file
    remove(filename);
}