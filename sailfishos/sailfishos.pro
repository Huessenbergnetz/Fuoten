# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
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

OTHER_FILES +=

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

TRANSLATIONS_INSTALL_DIR = /usr/share/harbour-fuoten/l10n

langfiles.path = $$TRANSLATIONS_INSTALL_DIR
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

