#include "CryptIT.h"


int main(int argc, char** argv) {

    CryptIT crypt;

    std::cout << "Enter password: ";
    crypt.setPassword();
    
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

    // Load all the files
    crypt.fileManager.setFiles(crypt.fileManager.getFilesRecursivly("."));

    // Ask the user if they want to encrypt or decrypt
    std::cout << "Do you want to encrypt or decrypt? (e/d): ";
    std::string ipt;
    getline(std::cin, ipt);

    // Encrypt
    if(ipt == "e") {
      crypt.encrypt();
    } else if(ipt == "d") {
      crypt.decrypt();
    } else {
      std::cout << "Wrong input, exiting the program!" << std::endl;
      std::cin.get();
      return 0;
    }

    return 0;
}
