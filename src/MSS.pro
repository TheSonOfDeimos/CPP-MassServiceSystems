QT += core gui
QT += widgets
QT += printsupport

CONFIG += c++14 console
CONFIG -= app_bundle


QMAKE_CXXFLAGS += -g
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        statisticManeger.cpp \
        ui/main.cpp \
        ui/mainwindow.cpp \
        requestPool.cpp \
        buffer.cpp \
        devicePool.cpp \
        qcustomplot/qcustomplot.cpp \
        ui/autoModeWidget.cpp \
        ui/configureWidget.cpp \
        ui/stepmodewidget.cpp




HEADERS += \
        buffer.h \
        devicePool.h \
        requestPool.h \
        timeMeneger.h \
        statisticManeger.hpp \
        ui/mainwindow.h \
        qcustomplot/qcustomplot.h \
        ui/autoModeWidget.hpp \
        ui/configureWidget.hpp \
        ui/stepmodewidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS +=
