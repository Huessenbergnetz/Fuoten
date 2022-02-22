include(FindPkgConfig)
pkg_search_module(Sailfish sailfishapp REQUIRED IMPORTED_TARGET)
pkg_search_module(NemoNotifications nemonotifications-qt5 REQUIRED IMPORTED_TARGET)
find_package(OpenSSL COMPONENTS SSL REQUIRED)

set(harbour_fuoten_SRCS
    main.cpp
    sfosuseravatar.cpp
    sharing/sharingmethod.cpp
    sharing/sharingmethodsmodel.cpp
    namfactory.cpp
    coverconnector.cpp
    useragentmodel.cpp
    sfosconfig.cpp
    sfosnotificator.cpp
    dbus/fuotendbusproxy.cpp
    dbus/fuotendbusadaptor.cpp
    languagesmodel.cpp
    licensesmodel.cpp
    ../../common/configuration.cpp
    ../../common/contextconfig.cpp
    ../../common/updateintervalmodel.cpp
    ../../common/useravatar.cpp
)

set(harbour_fuoten_HEADERS
    sfosuseravatar.h
    sharing/sharingmethod.h
    sharing/sharingmethod_p.h
    sharing/sharingmethodsmodel.h
    namfactory.h
    coverconnector.h
    useragentmodel.h
    sfosconfig.h
    ../../ADVobfuscator/Lib/Indexes.h
    ../../ADVobfuscator/Lib/Inline.h
    ../../ADVobfuscator/Lib/Log.h
    ../../ADVobfuscator/Lib/MetaRandom.h
    ../../ADVobfuscator/Lib/MetaString.h
    sfosnotificator.h
    dbus/fuotendbusproxy.h
    dbus/fuotendbusadaptor.h
    languagesmodel.h
    licensesmodel.h
    ../../common/globals.h
    ../../common/configuration.h
    ../../common/enums.h
    ../../common/contextconfig.h
    ../../common/updateintervalmodel.h
    ../../common/useravatar.h
)

add_executable(harbour-fuoten
    ${harbour_fuoten_HEADERS}
    ${harbour_fuoten_SRCS}
)

set_property(TARGET harbour-fuoten
    APPEND PROPERTY INSTALL_RPATH "/usr/share/harbour-fuoten/lib"
)

set(CMAKE_SKIP_RPATH OFF CACHE BOOL "" FORCE)
set(AES256_KEY "" CACHE STRING "32 charachter (32 byte) long encryption key used for AES256 encryption of the password, if you change this key, passwords stored with old keys are not readable anymore")

if(NOT ${AES256_KEY} MATCHES "[a-zA-Z0-9][a-zA-Z0-9][a-zA-Z0-9]+")
    message(FATAL_ERROR "Invalid AES256_KEY variable")
endif()

target_compile_definitions(harbour-fuoten PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    $<$<NOT:$<CONFIG:Debug>>:QT_NO_DEBUG_OUTPUT>
    VERSION_STRING="${PROJECT_VERSION}"
    TRANSLATIONS_DIR="/usr/share/harbour-fuoten/translations"
    AES256_KEY="${AES256_KEY}"
)

target_link_libraries(harbour-fuoten
    PRIVATE
        Qt5::Core
        Qt5::Quick
        Qt5::Sql
        Qt5::Network
        Qt5::DBus
        OpenSSL::SSL
        PkgConfig::Sailfish
        PkgConfig::NemoNotifications
        HbnSfosComponentsQt5::Core
        FuotenQt5::Core
)

install(TARGETS harbour-fuoten
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
