 

HEADERS += \
    $$PWD/globals.h \
    $$PWD/configuration.h \
    $$PWD/languagemodel.h \
    $$PWD/enums.h \
    $$PWD/contextconfig.h

SOURCES += \
    $$PWD/configuration.cpp \
    $$PWD/languagemodel.cpp \
    $$PWD/contextconfig.cpp

DISTFILES += \
    $$PWD/qml/common/models/PrivacyPolicyModel.qml

commonQmlFiles.files = $$PWD/qml/*

sfos {
    commonQmlFiles.path = /usr/share/harbour-fuoten/qml
}

INSTALLS += commonQmlFiles
