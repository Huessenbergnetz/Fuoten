TARGET = harbour-fuoten

CONFIG += sailfishapp
CONFIG += c++11

QT += sql network

VER_MAJ = 0
VER_MIN = 1
VER_PAT = 0

VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

DEFINES += VERSION_STRING=\"\\\"$${VERSION}\\\"\"

include(../common/common.pri)

LIBS += -L$$OUT_PWD/../libfuoten -lfuoten
INCLUDEPATH += $$PWD/../libfuoten

SOURCES += \
    src/main.cpp

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

isEmpty(INSTALL_TRANSLATIONS_DIR): INSTALL_TRANSLATIONS_DIR = /usr/share/harbour-fuoten/l10n

langfiles.path = $$INSTALL_TRANSLATIONS_DIR
langfiles.files = ../translations/*.qm
INSTALLS += langfiles

imgfiles.path = /usr/share/harbour-fuoten/images
imgfiles.files = images/*
INSTALLS += imgfiles

DISTFILES += \
    qml/harbour-fuoten.qml \
    harbour-fuoten.desktop \
    qml/phone/pages/MainPage.qml \
    qml/phone/cover/CoverPage.qml \
    qml/tablet/pages/MainPage.qml \
    qml/tablet/cover/CoverPage.qml \
    qml/common/dialogs/AccountSetup.qml \
    qml/common/pages/Settings.qml \
    qml/common/parts/ErrorItem.qml \
    qml/common/pages/About.qml \
    qml/common/models/LicensesModel.qml \
    qml/common/models/ChangelogModel.qml \
    qml/common/models/ContributorsModel.qml \
    qml/common/pages/PrivacyPolicy.qml \
    qml/common/wizard/Welcome.qml \
    qml/common/wizard/Account.qml \
    qml/common/parts/AccountItem.qml \
    qml/phone/pages/FolderListView.qml \
    qml/common/parts/ListPageHeader.qml \
    qml/phone/pages/FeedsListView.qml \
    qml/common/parts/CountBubble.qml \
    qml/common/dialogs/RenameFolderDialog.qml \
    qml/common/dialogs/CreateFolderDialog.qml \
    qml/common/pages/ContextConfigPage.qml \
    qml/phone/pages/FeedsListPage.qml \
    qml/common/dialogs/RenameFeedDialog.qml \
    qml/common/parts/FoldersComboBox.qml \
    qml/common/dialogs/CreateFeedDialog.qml \
    qml/common/dialogs/MoveFeedDialog.qml

