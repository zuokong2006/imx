include (../../demos.pri)

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += serialData.h serialSet.h serialTrans.h serialWidget.h
SOURCES += main.cpp serialSet.cpp serialTrans.cpp serialWidget.cpp

target.path = $$DEMOS_INSTALL/tools/serialport

INSTALLS += target
