/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "QSerialPortInfoOperatores.h"
#include "consolemanager.h"
//#include "settingsdialog.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QTimer>
#include <QRegularExpressionMatch>
#include <QDebug>

// bool operator==(const QSerialPortInfo& a, const QSerialPortInfo& b)
//{
//     return std::forward_as_tuple(a.description(),
//                a.manufacturer(),
//                a.portName(),
//                a.productIdentifier(),
//                a.serialNumber())
//         == std::forward_as_tuple(b.description(),
//             b.manufacturer(),
//             b.portName(),
//             b.productIdentifier(),
//             b.serialNumber());
// }

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_status(new QLabel)
    //    , m_console1(new ConsoleManager)
    //    , m_console2(new ConsoleManager)
    //    , m_console3(new ConsoleManager)
    //    , m_console4(new ConsoleManager)
    //    ,m_settings1(new SettingsDialog)
    //    ,m_settings2(new SettingsDialog)
    //    , m_serial1(new QSerialPort(this))
    //    , m_serial2(new QSerialPort(this))
    //    , m_serial3(new QSerialPort(this))
    //    , m_serial4(new QSerialPort(this))
    , registerFrequency(new QComboBox)
{
    //    listOfConnectedArduinosSerialPortsInfo = QSerialPortInfo::availablePorts();
    //    listOfAvaliableSystemSerialPorts = QSerialPortInfo::availablePorts();
    //    QMessageBox msgBox;

    //    QString avaliablePorts;
    //    foreach (QSerialPortInfo item, listOfConnectedArduinosSerialPortsInfo) {

    //#ifdef Q_OS_LINUX
    //        if (item.portName() == "/dev/tty0")
    //            serialPortList.removeOne(item);

    //        if (item.portName() == "/dev/tty1")
    //            serialPortList.removeOne(item);
    //#endif // Q_OS_LINUX

    //#ifdef Q_OS_WIN
    //        if (item.portName() == "COM1")
    //            listOfConnectedArduinosSerialPortsInfo.removeOne(item);

    //        if (item.portName() == "COM2")
    //            listOfConnectedArduinosSerialPortsInfo.removeOne(item);
    //#endif // Q_OS_WIN

    //        //        item.portName();

    //        if (item.description().contains("CH430")) {
    //        }
    //    }

    //    foreach (QSerialPortInfo item, listOfConnectedArduinosSerialPortsInfo) {
    //        avaliablePorts += item.portName() + QString(" ") + item.description() + QString("\n");
    //    }

    getListConnectedArduinosPortInfo();
    //    msgBox.setText(avaliablePorts);
    //    msgBox.exec();

    m_ui->setupUi(this);

    registerFrequency->addItem(QString("5 segundos"), QVariant(5));
    registerFrequency->addItem(QString("10 segundos"), QVariant(10));
    registerFrequency->addItem(QString("1 minutos"), QVariant(1 * 60));
    registerFrequency->addItem(QString("5 minutos"), QVariant(5 * 60));
    registerFrequency->addItem(QString("15 minutos"), QVariant(15 * 60));
    registerFrequency->addItem(QString("30 minutos"), QVariant(30 * 60));
    registerFrequency->addItem(QString("60 minutos"), QVariant(60 * 60));
    registerFrequency->addItem(QString("120 minutos"), QVariant(120 * 60));
    registerFrequency->setCurrentIndex(4);
    connect(registerFrequency,
        QOverload<int>::of(&QComboBox::currentIndexChanged), this,
        &MainWindow::signalSlotTest);

    autoConnect();

    //    for (int i = 0; i < listOfConnectedArduinosSerialPortsInfo.length(); ++i) {
    //        //        listOfSerialPorts.append(new QSerialPort);
    //        //        listOfConsoleManagers.append(new ConsoleManager(listOfSerialPorts[i], &listOfSerialPortsInfo[i], this));

    //        const int COLUMNS = 2;
    //        int column = i % COLUMNS;
    //        int row = (i - column) / COLUMNS;
    //        mainLayout->addWidget(listOfConsoleManagers[i], row + 1, column);

    //        //        listOfConsoleManagers[i]->setRegisterInterval(registerFrequency->itemData(registerFrequency->currentIndex()).toInt());
    //    }

    //    connect(m_serial1, &QSerialPort::errorOccurred, this,
    //        [=]() { handleError(m_serial1, m_console1); });
    //    connect(m_serial1, &QSerialPort::errorOccurred, this,
    //        [=]() { handleError(m_serial1, m_console1); });

    //    connect(m_serial1, &QSerialPort::readyRead, this,
    //        [=]() { readData(m_serial1, m_console1); });
    //    connect(m_serial1, &QSerialPort::readyRead, this,
    //        [=]() { readData(m_serial2, m_console2); });

    //    connect(m_console1->console, &Console::getData, this,
    //    [=](){writeData(m_serial1);}); connect(m_console2->console,
    //    &Console::getData, this, &MainWindow::writeData);
    //    connect(m_console3->console, &Console::getData, this,
    //    &MainWindow::writeData3); connect(m_console4->console,
    //    &Console::getData, this, &MainWindow::writeData4);

    //    QString dir = QFileDialog::getExistingDirectory(this, tr("Open
    //    Directory"), QDir::home(), QFileDialog::ShowDirsOnly |
    //    QFileDialog::DontResolveSymlinks);

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(scanSerialPorts()));

    timer->start(1000);

    this->showMaximized();
}

void MainWindow::autoConnect()
{
    listOfSerialPorts.clear();
    listOfConsoleManagers.clear();

    centralWidget->deleteLater();
    mainLayout->deleteLater();
    centralWidget = new QWidget;
    mainLayout = new QGridLayout;

    for (int i = 0; i < listOfConnectedArduinosSerialPortsInfo.length(); ++i) {
        listOfSerialPorts.append(new QSerialPort);
        listOfConsoleManagers.append(new ConsoleManager(listOfSerialPorts[i], &listOfConnectedArduinosSerialPortsInfo[i], this));

        const int COLUMNS = 2;
        int column = i % COLUMNS;
        int row = (i - column) / COLUMNS;
        mainLayout->addWidget(listOfConsoleManagers[i], row + 1, column);
    }

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    m_ui->statusBar->addWidget(m_status);
    mainLayout->addWidget(registerFrequency, 0, 0, 1, 2);
}

void MainWindow::getListConnectedArduinosPortInfo()
{
    listOfConnectedArduinosSerialPortsInfo = QSerialPortInfo::availablePorts();
    listOfAvaliableSystemSerialPorts = QSerialPortInfo::availablePorts();
    QMessageBox msgBox;

    QString avaliablePorts;
    foreach (QSerialPortInfo item, listOfConnectedArduinosSerialPortsInfo) {

#ifdef Q_OS_LINUX
        qInfo() << item.portName();

        QRegularExpression re("ttyUSB");

        qInfo() << "padrao: " + re.pattern();

        QRegularExpressionMatch match = re.match(item.portName());
        if (!match.hasMatch()){
            qInfo() << "Nao Bateu: " + match.captured(0);

            listOfConnectedArduinosSerialPortsInfo.removeOne(item);
        }else{
            qInfo() << "Bate: " + match.captured(0);
        }

//        if (item.portName() == "/dev/tty1")
//            listOfConnectedArduinosSerialPortsInfo.removeOne(item);
#endif // Q_OS_LINUX

#ifdef Q_OS_WIN
        if (item.portName() == "COM1")
            listOfConnectedArduinosSerialPortsInfo.removeOne(item);

        if (item.portName() == "COM2")
            listOfConnectedArduinosSerialPortsInfo.removeOne(item);
#endif // Q_OS_WIN

        //        item.portName();

        if (item.description().contains("CH430")) {
        }
    }

    foreach (QSerialPortInfo item, listOfConnectedArduinosSerialPortsInfo) {
        avaliablePorts += item.portName() + QString(" ") + item.description() + QString("\n");
    }

//    msgBox.setText(avaliablePorts);
//    msgBox.exec();
}

void MainWindow::scanSerialPorts()
{
    QString currentPortName;

    if (QSerialPortInfo::availablePorts() != this->listOfAvaliableSystemSerialPorts) {
        for (int i = 0; i < listOfConnectedArduinosSerialPortsInfo.length(); ++i) {
            listOfConsoleManagers[i]->closeSerialPort();
        }

        getListConnectedArduinosPortInfo();
        if (listOfConnectedArduinosSerialPortsInfo.length() > 0) {
            autoConnect();
        }
    }

    foreach (const QSerialPortInfo& info, QSerialPortInfo::availablePorts()) {
        currentPortName = info.systemLocation();
//        qInfo() << info.portName();
    }

    //    if (currentPortName == "My Desired PortName" ) {
    //        updateSettings();

    //        if ( !serial->isOpen()){
    //            qDebug() << "Serial Not Open";
    //            openSerialPort();
    //        }
    //    } else
    //    {
    //        serial->close();
    //    }
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::signalSlotTest()
{
    //    QMessageBox msgBox;
    //    msgBox.setText(QString::number(index));
    //    msgBox.exec();
    for (int i = 0; i < listOfConnectedArduinosSerialPortsInfo.length(); ++i) {
        listOfConsoleManagers[i]->setRegisterInterval(registerFrequency->itemData(registerFrequency->currentIndex()).toInt());
    }
}

void MainWindow::about()
{
    QMessageBox::about(
        this, tr("About Simple Terminal"),
        tr("The <b>Simple Terminal</b> example demonstrates how to "
           "use the Qt Serial Port module in modern GUI applications "
           "using Qt, with a menu bar, toolbars, and a status bar."));
}

void MainWindow::showStatusMessage(const QString& message)
{
    m_status->setText(message);
}
