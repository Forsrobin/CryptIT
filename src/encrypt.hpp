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

#include <fstream>

#include <cryptopp/files.h>
#include <cryptopp/default.h>

#include "helper.hpp"

class EncryptionTab : public QWidget
{

private:
  QTextEdit *textEdit;
  QVBoxLayout *layout;

  // UI Components
  QVBoxLayout *loadFileLayout;

public:
  std::string directoryPath;
  std::vector<std::string> files;

  EncryptionTab(QWidget *parent = nullptr) : QWidget(parent)
  {

    loadGUI();
    setLayout(this->layout);
    // Add the load file layout
  }

  void reset()
  {
    this->files.clear();
  }

  void loadGUI()
  {
    // Create the different layouts
    layout = new QVBoxLayout(this);
    loadFileLayout = new QVBoxLayout(this);

    QPushButton *loadFilesButton = new QPushButton("Load files", this);

    connect(loadFilesButton, &QPushButton::clicked, this, &EncryptionTab::loadFiles);

    this->layout->addWidget(loadFilesButton);
    this->layout->addLayout(this->loadFileLayout);
  }

  void loadFiles()
  {

    // If layout is not empty, clear it
    if (this->loadFileLayout->count() > 0)
    {
      QLayoutItem *child;
      while ((child = this->loadFileLayout->takeAt(0)) != 0)
      {
        delete child->widget();
        delete child;
      }
    }

    loadAllFiles();
    // Create a text lables that displays the number of files in the directory
    QLabel *numberOfFilesLable = new QLabel(this);
    numberOfFilesLable->setText(QString::fromStdString("Number of files in the directory: " + std::to_string(files.size())));
    this->loadFileLayout->addWidget(numberOfFilesLable);

    // Create a Text field that displays the first 10 selected files
    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    for (size_t i = 0; i < files.size() && i < 10; i++)
    {
      textEdit->append(QString::fromStdString(files[i]));
    }

    this->loadFileLayout->addWidget(textEdit);

    // Create text input field to enter the password
    // Create a label for the password field
    QLabel *passwordLabel = new QLabel("Enter password: ", this);
    this->loadFileLayout->addWidget(passwordLabel);

    // Create a text input field for the password
    QLineEdit *passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);
    this->loadFileLayout->addWidget(passwordInput);

    // Create a button to encrypt the files
    QPushButton *encryptButton = new QPushButton("Encrypt", this);
    connect(encryptButton, &QPushButton::clicked, this, &EncryptionTab::encryptFiles);

    if (files.size() == 0)
    {
      encryptButton->setDisabled(true);
    }

    this->loadFileLayout->addWidget(encryptButton);
  }

  void loadAllFiles()
  {
    fs::path _dir = directoryPath;
    if (fs::exists(_dir) && fs::is_directory(_dir))
    {
      files.clear();
      loadInitFiles(_dir, files);
    }
    else
    {
      std::cerr << "Invalid directory path or directory does not exist." << std::endl;
    }
  }

  void encryptFiles()
  {
    // Get the password from the input field
    QLineEdit *passwordInput = qobject_cast<QLineEdit *>(this->loadFileLayout->itemAt(3)->widget());
    std::string password = passwordInput->text().toStdString();

    // Encrypt the files
    for (const auto &file : files)
    {
      std::string outputFilePath = file + ".enc";

      // Convert the file path to a char array
      const char *file_ = file.c_str();
      const char *outputFilePath_ = outputFilePath.c_str();
      const char *password_ = password.c_str();

      if (!encryptFile(file_, outputFilePath_, password_))
      {
        std::cerr << "Error encrypting file: " << file << std::endl;
      }
    }
  }

  bool encryptFile(const char *fin, const char *fout, const char *passwd)
  {
    CryptoPP::FileSource f(fin, true, new CryptoPP::DefaultEncryptor(passwd, new CryptoPP::FileSink(fout)));

    return true;
  }
};
