#include <QApplication>
#include <QComboBox>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QtPlugin>
#include <QFileDialog>
#include <iostream>
#include <QGraphicsTextItem>
#include <QLabel>

Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);

int main(int argc, char *argv[])
{
  // SDR sdr;
  // std::vector<std::string> devices = sdr.getDevices();
  std::vector<std::string> devices = {"Device 1", "Device 2", "Device 3"};

  QApplication app(argc, argv);

  QMainWindow mainWindow;
  QWidget *centralWidget = new QWidget(&mainWindow);
  mainWindow.setCentralWidget(centralWidget);

  QVBoxLayout *layout = new QVBoxLayout(centralWidget);

  QComboBox *comboBox = new QComboBox();
  for (int i = 0; i < devices.size(); ++i)
  {
    comboBox->addItem(QString::fromStdString(devices[i]));
  }

  layout->addWidget(comboBox);

  //  Create a button to start the SDR
  QPushButton *selectDir = new QPushButton("Select directory");
  layout->addWidget(selectDir);

  //  Create a button to start the SDR
  QPushButton *startButton = new QPushButton("Start");
  layout->addWidget(startButton);

  // Creta a directory select button
  QString dir;
  QLabel *label = new QLabel("Selected directory:");
  layout->addWidget(label);

  // If the button is clicked, start the SDR
  QObject::connect(selectDir, &QPushButton::clicked, [&label, &dir]()
                   {
                     dir = QFileDialog::getExistingDirectory(NULL, "Open Directory", "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                     // Update the label with the selected directory
                     label->setText("Selected directory: " + dir); });

  // If the button is clicked, start the SDR
  QObject::connect(startButton, &QPushButton::clicked, [&dir, &comboBox]()
                   { 
                    std::cout << "Starting SDR with device: " << comboBox->currentText().toStdString() << std::endl; 
                    std::cout << "Starting SDR with directory: " << dir.toStdString() << std::endl; });

  mainWindow.setWindowTitle("Dropdown Example");
  mainWindow.show();

  return app.exec();
}