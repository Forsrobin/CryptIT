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
#include "threadpool.hpp"

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

  QLabel *messageDisplay = new QLabel();

public:
  std::string directoryPath;
  std::vector<std::string> files;
  ThreadPool *pool = new ThreadPool(1);

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

    layout->addWidget(messageDisplay);

    // ============================================================================
    // Create the button to select the directory
    QPushButton *loadFilesButton = new QPushButton("Load files", this);
    connect(loadFilesButton, &QPushButton::clicked, this, &Crypto::processFiles);
    layout->addWidget(loadFilesButton);

    // Create a text label that display the number of files loaded in the directory
    layout->addWidget(numOfFiles);

    // ============================================================================
    // Create the button to encrypt and decrypt the files
    connect(encryptButton, &QPushButton::clicked, this, &Crypto::encrypt);
    connect(decryptButton, &QPushButton::clicked, this, &Crypto::decrypt);
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
    foundPasswordFile = checkIfFileExists(directoryPath + passwordVerifyFileName + ".enc");

    // Reset the message label
    messageDisplay->setText("");

    fs::path _dir = directoryPath;
    files.clear();
    if (!fs::exists(_dir) && fs::is_directory(_dir))
    {
      std::cerr << "Invalid directory path or directory does not exist." << std::endl;
    }

    loadInitFiles(_dir, files, passwordVerifyFileName);
    updateGUI();
  }

  // ============================================================================
  // Crypto functions
  // ============================================================================
  // ============================================================================
  // Encrypt
  // ============================================================================

  static bool encryptFile(const char *fin, const char *fout, const char *passwd)
  {
    try
    {
      CryptoPP::FileSource f(fin, true, new CryptoPP::DefaultEncryptor(passwd, new CryptoPP::FileSink(fout)));
      return true;
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
      return false;
    }
  }

  void encryptFiles(std::vector<std::string> &files_, const std::string &password)
  {
    for (const auto &file : files_)
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
  }

  void encrypt()
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
      if (!Crypto::encryptFile(checkFile.c_str(), checkFileOut.c_str(), password.c_str()))
      {
        std::cerr << "Error encrypting file: " << checkFile << std::endl;
        return;
      }

      deleteFile(checkFile);
    }

    encryptButton->setDisabled(true);

    // If the number of threads is greater then the number of files, set the number of threads to the number of files
    if (pool->getNumberOfThreads() > static_cast<int>(files.size()))
      pool->setNumberOfThreads(static_cast<int>(files.size()));
    std::vector<std::vector<std::string>> filesSplit = splitVector(files, pool->getNumberOfThreads());

    // Start a timer so we can measure the time it takes to encrypt the files
    auto start = std::chrono::high_resolution_clock::now();

    // Add the files to the threadpool HERE
    for (const auto &filesSubset : filesSplit)
      pool->enqueue(&Crypto::encryptFiles, this, filesSubset, password);

    // Wait for all threads to finish
    pool->clear();
    pool->resetThreads();

    // Stop the timer
    auto end = std::chrono::high_resolution_clock::now();

    messageDisplay->setStyleSheet("QLabel { color : green; font-size: 14px;}");
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(end - start).count();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count() - minutes * 60;
    messageDisplay->setText("Successfully encrypted " + QString::number(files.size()) + " files with " + QString::number(pool->getNumberOfThreads()) + " threads\nTime taken: " + QString::number(minutes) + " minutes and " + QString::number(seconds) + " seconds");

    reset();
  }

  // ============================================================================
  // Encrypt
  // ============================================================================

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

  void decryptFiles(std::vector<std::string> &files_, const std::string &password)
  {
    for (const auto &file : files_)
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
  }

  void decrypt()
  {
    // Get the password from the input field
    std::string password = passwordInput->text().toStdString();
    std::string checkFile = directoryPath + passwordVerifyFileName + ".enc";
    std::string checkFileOut = directoryPath + passwordVerifyFileName;

    // Check if the passwordVerifyFile exists
    if (!checkIfFileExists(checkFile))
    {
      std::cerr << "Password verification file does not exist, select a directory that has been encrypted before!" << std::endl;
      messageDisplay->setStyleSheet("QLabel { color : red; font-size: 14px;}");
      messageDisplay->setText("Password verification file does not exist, select a directory that has been encrypted before!");
      return;
    }

    // Try to decrypt the file
    if (!decryptFile(checkFile.c_str(), checkFileOut.c_str(), password.c_str()))
    {
      std::cerr << "Error decrypting checkfile: " << directoryPath << std::endl;
      messageDisplay->setStyleSheet("QLabel { color : red; font-size: 14px;}");
      messageDisplay->setText("Password verification failed, please enter the correct password!");
      deleteFile(checkFileOut);
      return;
    }

    std::cerr << "Password verified, deleting the password verification files" << std::endl;
    deleteFile(checkFileOut);
    deleteFile(checkFile);

    // Threadpool logic here
    decryptButton->setDisabled(true);

    // If the number of threads is greater then the number of files, set the number of threads to the number of files
    if (pool->getNumberOfThreads() > static_cast<int>(files.size()))
      pool->setNumberOfThreads(static_cast<int>(files.size()));
    std::vector<std::vector<std::string>> filesSplit = splitVector(files, pool->getNumberOfThreads());

    // Start a timer so we can measure the time it takes to encrypt the files
    auto start = std::chrono::high_resolution_clock::now();

    // Add the files to the threadpool HERE
    for (const auto &filesSubset : filesSplit)
      pool->enqueue(&Crypto::decryptFiles, this, filesSubset, password);

    // Wait for all threads to finish
    pool->clear();
    pool->resetThreads();

    // Stop the timer
    auto end = std::chrono::high_resolution_clock::now();

    messageDisplay->setStyleSheet("QLabel { color : green; font-size: 14px;}");
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(end - start).count();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count() - minutes * 60;
    messageDisplay->setText("Successfully decrypted " + QString::number(files.size()) + " files with " + QString::number(pool->getNumberOfThreads()) + " threads\nTime taken: " + QString::number(minutes) + " minutes and " + QString::number(seconds) + " seconds");

    reset();
  }

  std::vector<std::vector<std::string>> splitVector(const std::vector<std::string> &_files, int numberOfThreads)
  {
    std::vector<std::vector<std::string>> filesSplit(numberOfThreads);

    int i = 0;
    for (const auto &file : _files)
    {
      filesSplit[i % numberOfThreads].push_back(file);
      i++;
    }

    return filesSplit;
  }
};
