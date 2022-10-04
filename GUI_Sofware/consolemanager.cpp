#include "consolemanager.h"
#include "console.h"

#include <QDebug>
#include <QDir>
#include <QGridLayout>
#include <QIcon>
#include <QMessageBox>
#include <QPixmap>
#include <QSerialPortInfo>

ConsoleManager::ConsoleManager(QSerialPort* serialPort, QSerialPortInfo* serialInfo, QWidget* parent)
    : QWidget { parent }
    , lblDataloggerID(new QLabel(this))
    , console(new Console(this))
    , pbSettings(new QPushButton(this))
    , pbConnect(new QPushButton(this))
    , pbDisconnect(new QPushButton(this))
    , pbClear(new QPushButton(this))
    , settings(new SettingsDialog(this))
    //    , serial(new QSerialPort(this))
    , status(new QLabel(this))
{
    this->serial = serialPort;
    this->serialInfo = serialInfo;

    QGridLayout* layout = new QGridLayout;
    QHBoxLayout* headerLayout = new QHBoxLayout;

    lblDataloggerID->setText("Datalogger:  Não conectado");
    lblDataloggerID->setEnabled(false);

    status->setText("Não configurado");
    status->setEnabled(false);

    lblDataloggerID->setObjectName("info");
    status->setObjectName("info");
    this->setStyleSheet("QLabel#info {font: 12pt bold;}");

    pbSettings->setText("Configuração");
    pbSettings->setIcon(QIcon(QPixmap(":/images/settings.png")));
    pbSettings->setIconSize(QSize(32, 32));
    pbSettings->setFixedSize(QSize(100, 36));

    pbConnect->setText("Conectar");
    pbConnect->setIcon(QIcon(QPixmap(":/images/connect.png")));
    pbConnect->setIconSize(QSize(32, 32));
    pbConnect->setFixedSize(QSize(100, 36));
    pbConnect->setVisible(true);

    pbDisconnect->setText("Desconectar");
    pbDisconnect->setIcon(QIcon(QPixmap(":/images/disconnect.png")));
    pbDisconnect->setIconSize(QSize(32, 32));
    pbDisconnect->setFixedSize(QSize(100, 36));
    pbDisconnect->setVisible(false);

    pbClear->setText("Limpar");
    pbClear->setIcon(QIcon(QPixmap(":/images/clear.png")));
    pbClear->setIconSize(QSize(32, 32));
    pbClear->setFixedSize(QSize(100, 36));

    lblDataloggerID->setText(dtlgID);
    //    headerLayout->addWidget(lblDataloggerID);
    //    headerLayout->addWidget(status);
    headerLayout->addWidget(pbConnect);
    headerLayout->addWidget(pbDisconnect);
    headerLayout->addWidget(pbClear);
    headerLayout->addWidget(pbSettings);
    layout->addLayout(headerLayout, 0, 0);
    layout->addWidget(status, 1, 0);
    layout->addWidget(lblDataloggerID, 2, 0);

    layout->addWidget(console, 3, 0);
    this->setLayout(layout);

    connect(pbSettings, &QPushButton::clicked, settings, &SettingsDialog::show);
    connect(pbConnect, &QPushButton::clicked, this, &ConsoleManager::openSerialPort);
    connect(pbDisconnect, &QPushButton::clicked, this, &ConsoleManager::closeSerialPort);
    connect(pbClear, &QPushButton::clicked, console, &Console::clear);

    connect(this->serial, &QSerialPort::readyRead, this, &ConsoleManager::readData);
    //    connect(serialPort, &QSerialPort::readyRead, this, &ConsoleManager::readData);

    //    connect(settings, &SettingsDialog::applied, this, [=](){status->setEnabled(true);});
    connect(settings, &SettingsDialog::applied, this, &ConsoleManager::configured);

    openSerialPort2();
}

void ConsoleManager::setDataloggerId(QString dataloggerID)
{
    lblDataloggerID->setText("Datalogger:  " + dataloggerID);
}

void ConsoleManager::configured()
{
    const SettingsDialog::Settings p = settings->settings();
    showStatusMessage(tr("Configurado como %1 : %2, %3, %4, %5, %6")
                          .arg(p.name)
                          .arg(p.stringBaudRate)
                          .arg(p.stringDataBits)
                          .arg(p.stringParity)
                          .arg(p.stringStopBits)
                          .arg(p.stringFlowControl));
}

void ConsoleManager::openSerialPort()
{
    const SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        pbConnect->setVisible(false);
        pbDisconnect->setVisible(true);
        console->setEnabled(true);
        pbSettings->setEnabled(false);
        lblDataloggerID->setEnabled(true);
        status->setEnabled(true);

        console->setLocalEchoEnabled(p.localEchoEnabled);

        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg(p.name)
                              .arg(p.stringBaudRate)
                              .arg(p.stringDataBits)
                              .arg(p.stringParity)
                              .arg(p.stringStopBits)
                              .arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        showStatusMessage(tr("Open error"));
    }
}

void ConsoleManager::openSerialPort2()
{
    const SettingsDialog::Settings p = settings->settings();
    qInfo() << serialInfo->portName();
    serial->setPortName(serialInfo->portName());
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        pbConnect->setVisible(false);
        pbDisconnect->setVisible(true);
        console->setEnabled(true);
        pbSettings->setEnabled(false);
        lblDataloggerID->setEnabled(true);
        status->setEnabled(true);

        console->setLocalEchoEnabled(p.localEchoEnabled);

        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg(p.name)
                              .arg(p.stringBaudRate)
                              .arg(p.stringDataBits)
                              .arg(p.stringParity)
                              .arg(p.stringStopBits)
                              .arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        showStatusMessage(tr("Open error"));
    }
}

void ConsoleManager::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();

    console->setEnabled(false);
    pbDisconnect->setVisible(false);
    pbConnect->setVisible(true);
    pbSettings->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
}

void ConsoleManager::showStatusMessage(const QString& message)
{
    status->setText(message);
}

void ConsoleManager::writeData(const QByteArray& data)
{
    serial->write(data);
}

QByteArray ConsoleManager::getSerialInput(QByteArray* data)
{
    //    QString tmp = QString(*data);
    QByteArray rawInput = *data;
    QByteArray input;

    //    qDebug() << "data: " << *data;
    //    QStringList inputs = tmp.split("\r\n");

    foreach (char c, rawInput) {
        if (c != '\n') {
            rawInput.remove(0, 1);
            input.append(c);
            //            qDebug() << tmp;
        } else if (c == '\n') {
            //           qDebug() << "new line found";
            rawInput.remove(0, 1);
            *data = rawInput;
            break;
        }
    }
    //    qDebug() << "inputs: " << inputs;
    //    qDebug() << "input: " << input;
    //    qDebug() << "rawInput: " << rawInput;
    return input;
}

void ConsoleManager::readData()
{
    //    qint64 NumberOfBytesToRead = serial->bytesAvailable();

    //    serial->waitForReadyRead(50);
    static QByteArray data;
    QByteArray input;

    data.append(serial->readAll());
    //    serial->clear(QSerialPort::Input);

    if (data.contains('\n')) // read into a structure until newline received.
    {
        while (data.contains('\n')) {
            input = getSerialInput(&data);
            console->putData("Recebido: " + input + "\n");
            serial->clear(QSerialPort::Output);

            if (input.contains("<Arduino is ready>")) {
                console->putData("Sending: AT+ID\n");
                writeData("AT+ID\n");
            }


            // Get datalogger ID
            if (QString(input).contains((QRegExp("^#[0-9]")))) {
                //                QRegExp rx("#[0-9]");
                //                int pos = rx.indexIn(serialResponse);
                //                QStringList list = rx.capturedTexts();
                //                dtlgID = list[pos];
                dtlgID = input;
                //                console->putData("Info: Obtido o ID ");
                //                console->putData(input);
                setDataloggerId(dtlgID);
                qDebug() << "ID Recebido: " << input;

                console->putData("Sending: AT+TIME#" + QString::number(registerInterval).toUtf8() + '\n');
                writeData("AT+TIME#" + QString::number(registerInterval).toUtf8() + '\n');
            }

            if (input.contains("save")) {
                console->putData("Salvando: ");
                console->putData(input);

                dtlgID.replace('\r', "");
                input.replace('\r', '#');

                if (!QDir(QDir::homePath() + "/Área de Trabalho/Datalogger").exists()) {
                    QDir().mkdir(QDir::homePath() + "/Área de Trabalho/Datalogger");
                }

                fileName = QDir::homePath() + "/Área de Trabalho/Datalogger/Notebook_arduino_" + dtlgID + ".csv";
                qDebug() << fileName;
                file.setFileName(fileName);
                qDebug() << "Tentando salvar '" << input << "' no arquivo " << fileName;

                if (file.exists()) {
                    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
                        file.write('*' + input + '\n');
                        file.close();
                        console->putData("\tSalvo\n");
                        qDebug() << "*Salvando '" << input << "' no arquivo " << fileName;
                    }

                } else {
                    qDebug() << "Arquivo não existe: " << fileName;
                    createFile(fileName);
                }
            }
        }
    }
}

void ConsoleManager::createFile(QString fileName)
{
    file.setFileName(fileName);
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly)) {
            file.write("Saved;DataloggerID;Data;Horario;sensorID;UmidadeSolo;UmidadeAmbiente;TemperaturaAmbiente\n");
            file.close();
            qDebug() << "Criado o arquivo: " << fileName;
        } else {
            qDebug() << "Falha ao criar arquivo: " << fileName;
        }
    }
}

void ConsoleManager::setRegisterInterval(int seconds)
{
    registerInterval = seconds;

    console->putData("Sending: AT+TIME#" + QString::number(registerInterval).toUtf8() + '\n');
    writeData("AT+TIME#" + QString::number(registerInterval).toUtf8() + '\n');
}
