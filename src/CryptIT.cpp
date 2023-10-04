#include "CryptIT.h"

// Constructor
CryptIT::CryptIT()
{
  // Set the password
}

// Destructor
CryptIT::~CryptIT()
{
  // Clear the password
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