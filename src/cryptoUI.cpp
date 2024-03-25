#include "include/cryptoUI.h"

// CryptoUI::CryptoUI(QWidget *parent) : QWidget(parent)
// {
//   loadGUI();
//   updateGUI();

//   // Create the layout structure
//   setLayout(layout);
//   layout->addLayout(encryptDecryptLayout);
// }

void CryptoUI::reset()
{
  this->files.clear();
  this->updateGUI();
}

void CryptoUI::updateGUI()
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
    if (this->foundPasswordFileInDirectory())
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

void CryptoUI::loadGUI()
{

  layout->addWidget(messageDisplay);

  // ============================================================================
  // Create the button to select the directory
  QPushButton *loadFilesButton = new QPushButton("Load files", this);
  connect(loadFilesButton, &QPushButton::clicked, [this]()
          { 
            processFiles();
            messageDisplay->setText("");
            updateGUI(); });

  layout->addWidget(loadFilesButton);

  // Create a text label that display the number of files loaded in the directory
  layout->addWidget(numOfFiles);

  // ============================================================================
  // Create the button to encrypt and decrypt the files
  // void Crypto::encrypt(std::string password)
  // Create a connection when you click the button that runs the encrypt function with the password from the input field
  connect(encryptButton, &QPushButton::clicked, [this]()
          {
            encrypt(passwordInput->text().toStdString());
            messageDisplay->setStyleSheet("QLabel { color : green; }");
            messageDisplay->setText("Total time: " + QString::fromStdString(getTotalTimeMessage()));
            reset(); });

  connect(decryptButton, &QPushButton::clicked, [this]()
          {
            decrypt(passwordInput->text().toStdString());
            messageDisplay->setStyleSheet("QLabel { color : green; }");
            messageDisplay->setText("Total time: " + QString::fromStdString(getTotalTimeMessage()));
            reset(); });

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
  connect(passwordInput, &QLineEdit::textChanged, this, &CryptoUI::updateGUI);
  layout->addWidget(passwordInput);
}
