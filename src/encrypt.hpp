#include <QApplication>
#include <QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>

class EncryptionTab : public QWidget {
public:
    EncryptionTab(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        textEdit = new QTextEdit(this);
        QPushButton *encryptButton = new QPushButton("Encrypt", this);
        
        connect(encryptButton, &QPushButton::clicked, this, &EncryptionTab::encryptText);

        layout->addWidget(textEdit);
        layout->addWidget(encryptButton);
        setLayout(layout);
    }

    void encryptText() {
        // Implement encryption logic here
        // For demonstration, let's just display the text
        QString encryptedText = textEdit->toPlainText();
        qDebug() << "Encrypted Text: " << encryptedText;
    }

private:
    QTextEdit *textEdit;
};
