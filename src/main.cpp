#include "CryptIT.h"


int main(int argc, char** argv) {

    CryptIT crypt;

    std::cout << "Enter password: ";
    crypt.setPassword();
    std::cout << "Password: " << crypt.getPassword() << std::endl;
    
    // Check so the keys exsist
    if(!crypt.checkIfKeysExsist()) {
      std::cout << "Keys does not exsist, exiting the program!" << std::endl;
      std::cin.get();
      return 0;
    }

    std::cout << "Keys exsist!" << std::endl;
    std::cout << "Validating password" << std::endl;

    // Validate the password with the private key
    if(!crypt.vildatePasswordWithPrivateKey()) {
      std::cout << "Wrong password, exiting the program!" << std::endl;
      std::cin.get();
      return 0;
    }

    // // Load all the files
    // crypt.fileManager.setFiles(crypt.fileManager.getFilesRecursivly("."));

    // // print the files
    // crypt.fileManager.printFiles();

    std::cin.get();

    return 0;
}
