#include "CryptIT.h"

// Constructor
CryptIT::CryptIT()
{
  // Set the key path
  this->keyPath = "./.data/";
}

// Destructor
CryptIT::~CryptIT()
{
  password.clear();
}

// Set the password
void CryptIT::setPassword()
{
  HANDLE hStdInput = GetStdHandle(STD_INPUT_HANDLE);
  DWORD mode = 0;
  // Create a restore point Mode
  GetConsoleMode(hStdInput, &mode);
  // Enable echo input
  SetConsoleMode(
      hStdInput,
      mode & (~ENABLE_ECHO_INPUT));
  // Take input
  std::string ipt;
  getline(std::cin, ipt);
  std::cout << std::endl;
  SetConsoleMode(hStdInput, mode);
  this->password = ipt;
}

// Get the password
std::string CryptIT::getPassword()
{
  return this->password;
}

// Generate rsa keys
void CryptIT::generateRSAKeys()
{

  // openssl genrsa -aes128 -passout pass:test -out ./.data/private.key 3072
  std::string private_key_command = "openssl genrsa -aes128 -passout pass:" + this->password + " -out " + this->keyPath + "private.key 3072";
  std::string public_key_command = "openssl rsa -in " + this->keyPath + "private.key -outform PEM -pubout -passin pass:" + this->password + " -out " + this->keyPath + "public.key";
  // Create a new private and public key
  system(private_key_command.c_str());
  system(public_key_command.c_str());
}

// Check if keys exsist
bool CryptIT::checkIfKeysExsist()
{
  std::cout << "Checking if key exsists" << std::endl;
  // Check so the .data folder exsists
  if (!fileManager.checkIfDirectoryExists("./.data"))
  {
    // Create the .data folder
    fileManager.createDirectory("./.data");
  }

  // Check if a private and public key exsists
  if (!fileManager.checkIfFileExists(this->keyPath + "private.key") || !fileManager.checkIfFileExists(this->keyPath + "public.key"))
  {
    // Create a new private and public key
    generateRSAKeys();
    // Make sure the keys got created, esle return false
    if (!fileManager.checkIfFileExists(this->keyPath + "private.key") || !fileManager.checkIfFileExists(this->keyPath + "public.key"))
    {
      return false;
    }
  }

  return true;
}

// Validate the password with the private key
bool CryptIT::vildatePasswordWithPrivateKey()
{
  // Command constants
  std::string command = "openssl rsa -in " + this->keyPath + "private.key -passin pass:" + this->password + " -check";
  std::string check_file = this->keyPath + "validate.txt";

  // Run the command and save the output in a file and check if the "RSA key ok" string is in the file
  if (system((command + " > " + check_file).c_str()) == 0)
  {
    // Check if the file contains the string
    if (fileManager.checkIfFileExists(check_file))
    {
      // Open the file
      std::ifstream file(check_file.c_str());
      // Create a string
      std::string str;
      // Get the first line
      std::getline(file, str);
      // Close the file
      file.close();
      // Check if the string contains the "RSA key ok" string
      if (str.find("RSA key ok") != std::string::npos)
      {
        // Remove the file
        fileManager.deleteFile(check_file.c_str());
        // Return true
        return true;
      }
      else
      {
        // Remove the file
        fileManager.deleteFile(check_file.c_str());
        // Return false
        return false;
      }
    }
  }
}

// encryptFile
void CryptIT::encrypt()
{
  // Print the files
  fileManager.printFiles();

  std::cout << "These are the files you arew about to encrypt, do you want to continue? (y/n): ";
  std::string ipt;
  getline(std::cin, ipt);
  if (ipt == "y")
  {
    std::cout << "Encrypting files" << std::endl;
    // For each file
    for (auto file : fileManager.getFiles())
    {
      // Encrypt the file
      system(("openssl rsautl -encrypt -inkey " + this->keyPath + "public.key -pubin -in " + file + " -out " + file + ".enc").c_str());
      // Delete the file old file
      fileManager.deleteFile(file.c_str());
    }
  }
  else
  {
    std::cout << "Exiting the program!" << std::endl;
    std::cin.get();
    return;
  }
}

// decryptFile
void CryptIT::decrypt()
{
  // For each file
  for (auto file : fileManager.getFiles())
  {
    // Decrypt the file
    system(("openssl rsautl -decrypt -inkey " + this->keyPath + "private.key -passin pass:" + this->password + " -in " + file + " -out " + file + ".dec").c_str());
    // Delete the file old file
    fileManager.deleteFile(file.c_str());

    // Remove the .enc.dec exstenbtion from the newly decrypted file
    std::string new_file_name = file.substr(0, file.size() - 4);

    // Rename the file to the old name
    fileManager.renameFile((file + ".dec").c_str(), new_file_name.c_str());
  }
}