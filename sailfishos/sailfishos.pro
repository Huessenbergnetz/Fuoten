TARGET = harbour-fuoten

CONFIG += sailfishapp
CONFIG += c++11

QT += sql network dbus

VER_MAJ = 0
VER_MIN = 5
VER_PAT = 2

VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

DEFINES += VERSION_STRING=\"\\\"$${VERSION}\\\"\"

CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

contains(CONFIG, clazy) {
    DEFINES+=CLAZY
    QMAKE_CXXFLAGS += "-Xclang -load -Xclang ClangLazy.so -Xclang -add-plugin -Xclang clang-lazy"
    QT += qml quick
}

contains(CONFIG, asan) {
    QMAKE_CXXFLAGS += "-fsanitize=address -fno-omit-frame-pointer -Wformat -Werror=format-security -Werror=array-bounds -g -ggdb"
    QMAKE_LFLAGS += "-fsanitize=address"
}

include(../common/common.pri)

LIBS += -L$$OUT_PWD/../libfuoten -lfuoten
INCLUDEPATH += $$PWD/../libfuoten

SOURCES += \
    src/main.cpp \
    src/sharing/sharingmethod.cpp \
    src/sharing/sharingmethodsmodel.cpp \
    src/namfactory.cpp \
    src/coverconnector.cpp \
    src/useragentmodel.cpp

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

isEmpty(INSTALL_TRANSLATIONS_DIR): INSTALL_TRANSLATIONS_DIR = /usr/share/harbour-fuoten/l10n

langfiles.path = $$INSTALL_TRANSLATIONS_DIR
langfiles.files = ../translations/*.qm
INSTALLS += langfiles

contimgs.path = /usr/share/harbour-fuoten/images/contributors
contimgs.files = images/contributors/*
INSTALLS += contimgs

icons.path = /usr/share/harbour-fuoten/images
icons.files = images/*.png
INSTALLS += icons

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
    qml/common/dialogs/MoveFeedDialog.qml \
    qml/phone/pages/FeedListDelegate.qml \
    qml/phone/pages/ArticlesListView.qml \
    qml/phone/pages/ArticlesListPage.qml \
    qml/common/parts/RescalingRichText.qml \
    qml/phone/pages/ArticlePage.qml \
    qml/phone/pages/WebViewPage.qml \
    qml/phone/pages/BaseListView.qml \
    qml/common/parts/SyncPanel.qml \
    qml/common/pages/Sharing.qml

HEADERS += \
    src/fuoteniconprovider.h \
    src/sharing/sharingmethod.h \
    src/sharing/sharingmethod_p.h \
    src/sharing/sharingmethodsmodel.h \
    src/namfactory.h \
    src/coverconnector.h \
    src/useragentmodel.h

