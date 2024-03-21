#include <QApplication>
#include <QComboBox>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QtPlugin>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QFileDialog>

#include "encrypt.hpp"
#include "decrypt.hpp"

Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);

int main(int argc, char *argv[])
{

  QApplication app(argc, argv);
  QMainWindow mainWindow;
  QWidget *centralWidget = new QWidget(&mainWindow);
  mainWindow.setCentralWidget(centralWidget);

  // Layouts
  QVBoxLayout *layout = new QVBoxLayout(centralWidget);
  QTabWidget tabWidget;
  EncryptionTab encryptionTab;
  DecryptionTab decryptionTab;

  // // Create a QT Widget that display the image from assets/logo.png using QGraphicsView
  QGraphicsScene *scene = new QGraphicsScene();
  QGraphicsView *view = new QGraphicsView(scene);
  QPixmap image("assets/logo.png");
  scene->addPixmap(image);

  // // Creta a directory select button
  QPushButton *selectDir = new QPushButton("Select Directory", &mainWindow);
  QString dir;
  QLabel *label = new QLabel("");

  // // If the button is clicked, start the SDR
  QObject::connect(selectDir, &QPushButton::clicked, [&label, &dir, &encryptionTab, &decryptionTab, &tabWidget, &layout]()
                   {
                     dir = QFileDialog::getExistingDirectory(NULL, "Open Directory", "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                     encryptionTab.directoryPath = dir.toStdString();
                     encryptionTab.reset();
                     label->setText("Selected directory: " + dir);

                    //  decryptionTab.dir = dir.toStdString();
                     // Update the label with the selected directory
                     tabWidget.addTab(&encryptionTab, "Encrypt");
                     tabWidget.addTab(&decryptionTab, "Decrypt");
                     
                     layout->addWidget(&tabWidget); });

  // Add all the widgets to the layout
  layout->addWidget(view);

  layout->addWidget(label);
  layout->addWidget(selectDir);

  mainWindow.setWindowTitle("CryptIT");
  mainWindow.resize(400, 300);
  mainWindow.show();

  return app.exec();

  // // Creta a directory select button
  // QString dir;
  // QLabel *label = new QLabel("Selected directory:");
  // layout->addWidget(label);

  // mainWindow.setWindowTitle("Dropdown Example");
  // mainWindow.show();

  // return app.exec();
}