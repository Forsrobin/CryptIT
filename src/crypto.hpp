#include <QApplication>
#include <QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <iostream>
#include <filesystem>
#include <QLabel>
#include <QLineEdit>
#include <algorithm>

#include <fstream>

#include <cryptopp/files.h>
#include <cryptopp/default.h>

#include "helper.hpp"

class Crypto : public QWidget
{

private:
  std::string passwordVerifyFileName = "/.jghdf78tdshui4bjdfy78";
  bool foundPasswordFile = false;

  // Layouts
  QVBoxLayout *layout = new QVBoxLayout(this);
  QHBoxLayout *encryptDecryptLayout = new QHBoxLayout(this);

  // Components
  QPushButton *encryptButton = new QPushButton("Encrypt", this);
  QPushButton *decryptButton = new QPushButton("Decrypt", this);
  QLabel *numOfFiles = new QLabel();
  QTextEdit *fileDisplay = new QTextEdit();
  QLineEdit *passwordInput = new QLineEdit();

public:
  std::string directoryPath;
  std::vector<std::string> files;

  Crypto(QWidget *parent = nullptr) : QWidget(parent)
  {
    loadGUI();
    updateGUI();

    // Create the layout structure
    setLayout(layout);
    layout->addLayout(encryptDecryptLayout);
  }

  void reset()
  {
    this->files.clear();
    this->updateGUI();
  }

  void updateGUI()
  {
    // Update the number of files loaded in the directory
    numOfFiles->setText("Number of files loaded: " + QString::number(files.size()));

    // Display the 10 first files available loaded in the directory
    // using an itterator to break out if the number of files is greater than 10
    fileDisplay->clear();
    int i = 0;
    for (const auto &file : files)
    {
      fileDisplay->append(file.c_str());
      i++;
      if (i == 10)
      {
        break;
      }
    }

    // If the number of files in files is zero, disable the encrypt and decrypt buttons
    if (files.size() == 0 || passwordInput->text().isEmpty())
    {
      encryptButton->setDisabled(true);
      decryptButton->setDisabled(true);
    }
    else
    {
      if (foundPasswordFile)
      {
        encryptButton->setDisabled(true);
        decryptButton->setDisabled(false);
      }
      else
      {
        encryptButton->setDisabled(false);
        decryptButton->setDisabled(true);
      }
    }
  }

  void loadGUI()
  {
    // ============================================================================
    // Create the button to select the directory
    QPushButton *loadFilesButton = new QPushButton("Load files", this);
    connect(loadFilesButton, &QPushButton::clicked, this, &Crypto::processFiles);
    layout->addWidget(loadFilesButton);

    // Create a text label that display the number of files loaded in the directory
    layout->addWidget(numOfFiles);

    // ============================================================================
    // Create the button to encrypt and decrypt the files
    connect(encryptButton, &QPushButton::clicked, this, &Crypto::encryptFiles);
    connect(decryptButton, &QPushButton::clicked, this, &Crypto::decryptFiles);
    encryptDecryptLayout->addWidget(encryptButton);
    encryptDecryptLayout->addWidget(decryptButton);

    // ============================================================================
    // Create a text edit widget to display the files loaded in the directory
    fileDisplay->setReadOnly(true);
    fileDisplay->setFixedHeight(100);
    encryptDecryptLayout->addWidget(fileDisplay);

    // ============================================================================
    // Create a password input field
    passwordInput->setPlaceholderText("Enter password");
    passwordInput->setEchoMode(QLineEdit::Password);
    // Add a eventlistner that runs the updateGUI function when the text in the input field changes
    connect(passwordInput, &QLineEdit::textChanged, this, &Crypto::updateGUI);
    layout->addWidget(passwordInput);
  }

  // Load the files in the directory
  void processFiles()
  {

    // Check if the passwordVerifyFile exists
    if (checkIfFileExists(directoryPath + passwordVerifyFileName + ".enc"))
    {
      foundPasswordFile = true;
    }
    else
    {
      foundPasswordFile = false;
    }

    fs::path _dir = directoryPath;
    if (fs::exists(_dir) && fs::is_directory(_dir))
    {
      files.clear();
      loadInitFiles(_dir, files, passwordVerifyFileName);
      updateGUI();
    }
    else
    {
      std::cerr << "Invalid directory path or directory does not exist." << std::endl;
    }
  }

  void decryptFiles()
  {
    // Get the password from the input field
    std::string password = passwordInput->text().toStdString();
    std::string checkFile = directoryPath + passwordVerifyFileName + ".enc";
    std::string checkFileOut = directoryPath + passwordVerifyFileName;

    // Check if the passwordVerifyFile exists
    if (!checkIfFileExists(checkFile))
    {
      std::cerr << "Password verification file does not exist, select a directory that has been encrypted before!" << std::endl;
      return;
    }

    // Try to decrypt the file
    if (!decryptFile(checkFile.c_str(), checkFileOut.c_str(), password.c_str()))
    {
      std::cerr << "Error decrypting checkfile: " << directoryPath << std::endl;
      deleteFile(checkFileOut);
      return;
    }

    std::cerr << "Password verified, deleting the password verification files" << std::endl;
    deleteFile(checkFileOut);
    deleteFile(checkFile);

    for (const auto &file : files)
    {
      // If the file does not have the .enc extension, skip it
      if (file.size() < 4 || file.substr(file.size() - 4) != ".enc")
      {
        continue;
      }

      // Remove the .enc extension
      std::string outputFilePath = file.substr(0, file.size() - 4);
      if (!decryptFile(file.c_str(), outputFilePath.c_str(), password.c_str()))
      {
        std::cerr << "Error decrypting file: " << file << std::endl;
      }
      else
      {
        deleteFile(file);
      }
    }

    reset();
  }

  void encryptFiles()
  {
    // Get the password from the input field
    std::string password = passwordInput->text().toStdString();
    std::string checkFile = directoryPath + passwordVerifyFileName;
    std::string checkFileOut = directoryPath + passwordVerifyFileName + ".enc";

    if (!checkIfFileExists(checkFile))
    {
      std::cerr << "Password verification file does not exist, creating one" << std::endl;
      createPasswordVerificationFile(directoryPath, passwordVerifyFileName);
      // Encrypt the file with the password
      if (!encryptFile(checkFile.c_str(), checkFileOut.c_str(), password.c_str()))
      {
        std::cerr << "Error encrypting file: " << checkFile << std::endl;
        return;
      }

      deleteFile(checkFile);
    }

    for (const auto &file : files)
    {

      // If the file has the .enc extension, skip it
      if (file.size() >= 4 && file.substr(file.size() - 4) == ".enc")
      {
        continue;
      }

      std::string outputFilePath = file + ".enc";
      if (!encryptFile(file.c_str(), outputFilePath.c_str(), password.c_str()))
      {
        std::cerr << "Error encrypting file: " << file << std::endl;
      }
      else
      {
        deleteFile(file);
      }
    }

    reset();
  }

  bool encryptFile(const char *fin, const char *fout, const char *passwd)
  {
    CryptoPP::FileSource f(fin, true, new CryptoPP::DefaultEncryptor(passwd, new CryptoPP::FileSink(fout)));

    return true;
  }

  bool decryptFile(const char *fin, const char *fout, const char *passwd)
  {

    try
    {
      CryptoPP::FileSource f(fin, true, new CryptoPP::DefaultDecryptor(passwd, new CryptoPP::FileSink(fout)));
      return true;
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
      return false;
    }
  }
};
