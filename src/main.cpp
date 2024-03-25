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

#include <iostream>

#include "cryptoUI.h"

Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);

// Disable the console window on Windows
// #ifdef _WIN32
// #include <windows.h>
// #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
// #endif

int main(int argc, char *argv[])
{

  QApplication app(argc, argv);
  QMainWindow mainWindow;
  QWidget *centralWidget = new QWidget(&mainWindow);

  mainWindow.setCentralWidget(centralWidget);

  // Layouts
  QVBoxLayout *layout = new QVBoxLayout(centralWidget);
  CryptoUI cryptoSection;

  // // Create a QT Widget that display the image from assets/logo.png using QGraphicsView
  QGraphicsScene *scene = new QGraphicsScene();
  QGraphicsView *view = new QGraphicsView(scene);
  // Get the relativ path from the executable to the logo.png
  std::string path = std::string(argv[0]);
  QPixmap image(QString::fromStdString(path.substr(0, path.find_last_of("\\")) + "\\assets\\logo.png"));
  scene->addPixmap(image);

  // // Creta a directory select button
  QPushButton *selectDir = new QPushButton("Select Directory", &mainWindow);
  QString dir;
  QLabel *label = new QLabel("");

  // Create a dropdown menu to select the number of threads
  int numberOfCores = Helper::getNumberOfCores();
  QLabel *threadsLabel = new QLabel("Select number of threads");
  QComboBox *threads = new QComboBox(&mainWindow);
  for (int i = 1; i <= numberOfCores; i++)
  {
    threads->addItem(QString::number(i));
  }

  // Add en eventlistener to the getNumberOfCores dropdown
  QObject::connect(threads, QOverload<int>::of(&QComboBox::activated), [&threads, &cryptoSection]()
                   { 
                    
                    // If no pool has been created, create one
                      cryptoSection.pool = new ThreadPool(threads->currentText().toInt()); });

  // // If the button is clicked, start the SDR
  QObject::connect(selectDir, &QPushButton::clicked, [&layout, &label, &cryptoSection, &dir]()
                   {
                     dir = QFileDialog::getExistingDirectory(NULL, "Open Directory", "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                     cryptoSection.directoryPath = dir.toStdString();
                     cryptoSection.updateGUI();
                     label->setText("Selected directory: " + dir);

                     // If the cryptoSection has not been added to the layout, add it
                     if (cryptoSection.parent() == nullptr)
                     {
                       layout->addWidget(&cryptoSection);
                     } });

  // Add all the widgets to the layout
  layout->addWidget(view);

  layout->addWidget(label);
  layout->addWidget(selectDir);

  // Create a horizontal layout for the threads dropdown
  layout->addWidget(threadsLabel);
  layout->addWidget(threads);
  // Set a max width for the threads dropdown
  threads->setMaximumWidth(100);

  mainWindow.setWindowTitle("CryptIT");
  mainWindow.resize(400, 300);
  mainWindow.show();

  return app.exec();
}