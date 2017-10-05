 

HEADERS += \
    $$PWD/globals.h \
    $$PWD/configuration.h \
    $$PWD/languagemodel.h \
    $$PWD/enums.h \
    $$PWD/contextconfig.h \
    $$PWD/updateintervalmodel.h

SOURCES += \
    $$PWD/configuration.cpp \
    $$PWD/languagemodel.cpp \
    $$PWD/contextconfig.cpp \
    $$PWD/updateintervalmodel.cpp

DISTFILES += \
    $$PWD/qml/common/models/PrivacyPolicyModel.qml

sfos {
    commonQmlFiles.files = $$PWD/qml/*
    commonQmlFiles.path = /usr/share/harbour-fuoten/qml
    INSTALLS += commonQmlFiles
}
