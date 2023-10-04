#include "CryptIT.h"


int main(int argc, char** argv) {

    CryptIT crypt;

    std::cout << "Enter password: ";
    crypt.setPassword();

    std::cout << "Password: " << crypt.getPassword() << std::endl;
    std::cin.get();

    return 0;
}
