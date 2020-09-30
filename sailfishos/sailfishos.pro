TARGET = harbour-fuoten

CONFIG += sailfishapp
CONFIG += c++11
CONFIG += c++14

QT += sql network dbus

VER_MAJ = 0
VER_MIN = 6
VER_PAT = 6

VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

DEFINES += VERSION_STRING=\"\\\"$${VERSION}\\\"\"

CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

isEmpty(AES256_KEY) {
    error("You need to define a 32 byte AES256 encryption key with AES256_KEY")
}
DEFINES += AES256_KEY=\"\\\"$${AES256_KEY}\\\"\"

contains(CONFIG, asan) {
    QMAKE_CXXFLAGS += "-fsanitize=address -fno-omit-frame-pointer -Wformat -Werror=format-security -Werror=array-bounds -g -ggdb"
    QMAKE_LFLAGS += "-fsanitize=address"
}

include(../common/common.pri)

LIBS += -L$$OUT_PWD/../libfuoten -lfuoten
INCLUDEPATH += $$PWD/../libfuoten

PKGCONFIG += openssl
PKGCONFIG += nemonotifications-qt5
INCLUDEPATH += /usr/include/nemonotifications-qt5

SOURCES += \
    src/main.cpp \
    src/sfosuseravatar.cpp \
    src/sharing/sharingmethod.cpp \
    src/sharing/sharingmethodsmodel.cpp \
    src/namfactory.cpp \
    src/coverconnector.cpp \
    src/useragentmodel.cpp \
    src/sfosconfig.cpp \
    src/sfosnotificator.cpp \
    src/dbus/fuotendbusproxy.cpp \
    src/dbus/fuotendbusadaptor.cpp \
    src/languagesmodel.cpp \
    src/licensesmodel.cpp

SAILFISHAPP_ICONS = 86x86 108x108 128x128 150x150 172x172

isEmpty(INSTALL_TRANSLATIONS_DIR): INSTALL_TRANSLATIONS_DIR = /usr/share/$$TARGET/translations

DEFINES += TRANSLATIONS_DIR=\"\\\"$${INSTALL_TRANSLATIONS_DIR}\\\"\"

langfiles.path = $$INSTALL_TRANSLATIONS_DIR
langfiles.files = ../translations/*.qm
INSTALLS += langfiles

contimgs.path = /usr/share/$$TARGET/images/contributors
contimgs.files = images/contributors/*
INSTALLS += contimgs

icons.path = /usr/share/$$TARGET/icons
icons.files = icons/z*
INSTALLS += icons

DISTFILES += \
    qml/common/wizard/LoginManual.qml \
    qml/common/wizard/LoginManualCheck.qml \
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
    qml/common/models/ChangelogModel.qml \
    qml/common/models/ContributorsModel.qml \
    qml/common/pages/PrivacyPolicy.qml \
    qml/common/wizard/Welcome.qml \
    qml/common/wizard/LoginFlowSetup.qml \
    qml/common/wizard/LoginFlowCheck.qml \
    qml/common/wizard/LoginFlowPolling.qml \
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
    qml/common/pages/Sharing.qml \
    qml/common/parts/FontSizeSlider.qml

HEADERS += \
    src/sfosuseravatar.h \
    src/sharing/sharingmethod.h \
    src/sharing/sharingmethod_p.h \
    src/sharing/sharingmethodsmodel.h \
    src/namfactory.h \
    src/coverconnector.h \
    src/useragentmodel.h \
    src/sfosconfig.h \
    ../ADVobfuscator/Lib/Indexes.h \
    ../ADVobfuscator/Lib/Inline.h \
    ../ADVobfuscator/Lib/Log.h \
    ../ADVobfuscator/Lib/MetaRandom.h \
    ../ADVobfuscator/Lib/MetaString.h \
    src/sfosnotificator.h \
    src/dbus/fuotendbusproxy.h \
    src/dbus/fuotendbusadaptor.h \
    src/languagesmodel.h \
    src/licensesmodel.h

include(../HBN_SFOS_Components/HBN_SFOS_Components.pri)
