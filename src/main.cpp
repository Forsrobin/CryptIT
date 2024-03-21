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

#include "crypto.hpp"

Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);

int main(int argc, char *argv[])
{

  QApplication app(argc, argv);
  QMainWindow mainWindow;
  QWidget *centralWidget = new QWidget(&mainWindow);

  mainWindow.setCentralWidget(centralWidget);

  // Layouts
  QVBoxLayout *layout = new QVBoxLayout(centralWidget);
  Crypto cryptoSection;

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

  mainWindow.setWindowTitle("CryptIT");
  mainWindow.resize(400, 300);
  mainWindow.show();

  return app.exec();
}