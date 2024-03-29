# SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

include(FindPkgConfig)
pkg_search_module(Sailfish sailfishapp REQUIRED IMPORTED_TARGET)
pkg_search_module(NemoNotifications nemonotifications-qt5 REQUIRED IMPORTED_TARGET)
find_package(OpenSSL COMPONENTS SSL REQUIRED)

add_executable(fuoten_exe)

target_sources(fuoten_exe
    PRIVATE
        main.cpp
        sfosconfig.h
        sfosconfig.cpp
        sfosnotificator.h
        sfosnotificator.cpp
        sfosuseravatar.h
        sfosuseravatar.cpp
        sfosmigrator.h
        sfosmigrator.cpp
        sfoscontextconfig.h
        sfoscontextconfig.cpp
#        useragentmodel.h
#        useragentmodel.cpp
        languagesmodel.h
        languagesmodel.cpp
        licensesmodel.h
        licensesmodel.cpp
        coverconnector.h
        coverconnector.cpp
        namfactory.h
        namfactory.cpp
        ../../ADVobfuscator/Lib/Indexes.h
        ../../ADVobfuscator/Lib/Inline.h
        ../../ADVobfuscator/Lib/Log.h
        ../../ADVobfuscator/Lib/MetaRandom.h
        ../../ADVobfuscator/Lib/MetaString.h
)

GNUInstallDirs_get_absolute_install_dir(FUOTEN_EXE_RPATH CMAKE_INSTALL_LIBDIR)

set_property(TARGET fuoten_exe
    APPEND PROPERTY INSTALL_RPATH "${FUOTEN_EXE_RPATH}"
)

set_property(TARGET fuoten_exe
    PROPERTY OUTPUT_NAME "${FUOTEN_EXE_OUTPUT_NAME}"
)

set(CMAKE_SKIP_RPATH OFF CACHE BOOL "" FORCE)


GNUInstallDirs_get_absolute_install_dir(FUOTEN_ABS_I18NDIR FUOTEN_I18NDIR)

target_compile_definitions(fuoten_exe PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    $<$<NOT:$<CONFIG:Debug>>:QT_NO_DEBUG_OUTPUT>
    VERSION_STRING="${PROJECT_VERSION}"
    FUOTEN_I18NDIR="${FUOTEN_ABS_I18NDIR}"
    AES256_KEY="${FUOTEN_AES256_KEY}"
)

target_link_libraries(fuoten_exe
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

install(TARGETS fuoten_exe
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

add_subdirectory(../../common common)
add_subdirectory(dbus)
