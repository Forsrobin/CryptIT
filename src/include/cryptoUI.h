#ifndef CRYPTIT_CRYPTO_UI_H
#define CRYPTIT_CRYPTO_UI_H

#include <QApplication>
#include <QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <fstream>

#include <cryptopp/files.h>
#include <cryptopp/default.h>

#include "crypto.h"

class CryptoUI : public Crypto, public QWidget
{

private:
  // Layouts
  QVBoxLayout *layout = new QVBoxLayout(this);
  QHBoxLayout *encryptDecryptLayout = new QHBoxLayout(this);

  // Components
  QPushButton *encryptButton = new QPushButton("Encrypt", this);
  QPushButton *decryptButton = new QPushButton("Decrypt", this);

  QTextEdit *fileDisplay = new QTextEdit();
  QLineEdit *passwordInput = new QLineEdit();

  QLabel *numOfFiles = new QLabel();
  QLabel *messageDisplay = new QLabel();

public:
  CryptoUI(QWidget *parent = nullptr);
  void reset();
  void updateGUI();
  void loadGUI();
};

#endif // CRYPTIT_CRYPTO_UI_H
