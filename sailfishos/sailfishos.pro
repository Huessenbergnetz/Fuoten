TARGET = harbour-fuoten

CONFIG += sailfishapp
CONFIG += c++11

QT += sql network

VER_MAJ = 0
VER_MIN = 0
VER_PAT = 1

VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

DEFINES += VERSION_STRING=\"\\\"$${VERSION}\\\"\"

include(../common/common.pri)

LIBS += -L$$OUT_PWD/../libfuoten -lfuoten
INCLUDEPATH += $$PWD/../libfuoten/src

SOURCES += \
    src/main.cpp

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

isEmpty(INSTALL_TRANSLATIONS_DIR): INSTALL_TRANSLATIONS_DIR = /usr/share/harbour-fuoten/l10n

langfiles.path = $$INSTALL_TRANSLATIONS_DIR
langfiles.files = ../translations/*.qm
INSTALLS += langfiles

DISTFILES += \
    qml/harbour-fuoten.qml \
    harbour-fuoten.desktop \
    qml/phone/pages/MainPage.qml \
    qml/phone/cover/CoverPage.qml \
    qml/tablet/pages/MainPage.qml \
    qml/tablet/cover/CoverPage.qml \
    qml/common/dialogs/AccountSetup.qml \
    qml/common/pages/Settings.qml \
    qml/common/parts/ErrorItem.qml

