#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H

#include <QComboBox>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QWidget>

#include "console.h"
#include "settingsdialog.h"

class ConsoleManager : public QWidget {
private:
    QLabel* lblDataloggerID = nullptr;
    void configured();

    Q_OBJECT
public:
    explicit ConsoleManager(QSerialPort* serialPort, QSerialPortInfo* serialInfo, QWidget* parent = nullptr);
    void setDataloggerId(QString dataloggerID);
    void openSerialPort();
    void openSerialPort2();
    void closeSerialPort();
    void writeData(const QByteArray& data);
    void readData();
    void createFile(QString fileName);
    void setRegisterInterval(int seconds);
    QByteArray getSerialInput(QByteArray* data);

    QFile file;
    QString fileName;
    QString serialResponse;
    int registerInterval = 900;
    QString filePath = "";

    void showStatusMessage(const QString& message);

    Console* console = nullptr;
    QPushButton* pbSettings;
    QPushButton* pbConnect;
    QPushButton* pbDisconnect;
    QPushButton* pbClear;
    SettingsDialog* settings = nullptr;
    QSerialPort* serial = nullptr;
    QSerialPortInfo* serialInfo;
    QLabel* status = nullptr;
    QString dtlgID = "";

signals:
};

#endif // CONSOLEMANAGER_H
