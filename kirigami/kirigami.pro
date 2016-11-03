TEMPLATE = app
TARGET = fuoten

VER_MAJ = 0
VER_MIN = 2
VER_PAT = 0

VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

DEFINES += VERSION_STRING=\"\\\"$${VERSION}\\\"\"

include(../common/common.pri)

CONFIG += c++11
CONFIG += link_pkgconfig

QT += core qml quick svg

PKGCONFIG += fuoten

SOURCES += \
    src/main.cpp

RESOURCES += \
    qml.qrc
