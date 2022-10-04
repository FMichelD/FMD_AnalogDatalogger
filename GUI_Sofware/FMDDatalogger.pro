QT += widgets serialport
requires(qtConfig(combobox))

TARGET = FMDDatalogger
TEMPLATE = app

SOURCES += \
    QSerialPortInfoOperatores.cpp \
    consolemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp

HEADERS += \
    QSerialPortInfoOperatores.h \
    consolemanager.h \
    mainwindow.h \
    settingsdialog.h \
    console.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    FMDDatalogger.qrc

RC_ICONS = app.ico

ONFIG += console \
    static \
    staticlib

target.path = ./build
INSTALLS += target
