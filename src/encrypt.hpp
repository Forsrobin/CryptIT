#include <QApplication>
#include <QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <iostream>
#include <filesystem>

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

  void reset() {
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

    // Create a button to encrypt the files
    QPushButton *encryptButton = new QPushButton("Encrypt", this);
    connect(encryptButton, &QPushButton::clicked, this, &EncryptionTab::encryptFiles);
    if (files.size() == 0)
    {
      encryptButton->setDisabled(true);
    }
    this->loadFileLayout->addWidget(encryptButton);

    this->layout->addLayout(this->loadFileLayout);
  }

  void loadAllFiles()
  {
    fs::path _dir = directoryPath;
    if (fs::exists(_dir) && fs::is_directory(_dir))
    {
      files.clear();
      countFiles(_dir, files);
    }
    else
    {
      std::cerr << "Invalid directory path or directory does not exist." << std::endl;
    }
  }

  void encryptFiles()
  {
    // Encrypt the files
  }
};
