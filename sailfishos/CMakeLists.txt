# SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

add_subdirectory(src)
add_subdirectory(qml)
add_subdirectory(icons)
add_subdirectory(translations)

install(FILES harbour-fuoten.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
)

# Get the other files reachable from the project tree in Qt Creator
add_custom_target(distfiles
    SOURCES
        harbour-fuoten.desktop
)

# Tell Qt Creator where the application executable(s) would be located on the
# device.
#
# It is not necessary to list other deployables than executables (runtime
# targets) here. The deployment process of Sailfish OS projects is opaque to
# Qt Creator and the information contained in QtCreatorDeployment.txt is only
# used to locate the executable associated with the active run configuration
# on the device in order to run it.
#
# Search the Qt Creator Manual to learn about the QtCreatorDeployment.txt file
# format.
file(WRITE "${CMAKE_BINARY_DIR}/QtCreatorDeployment.txt"
    "${CMAKE_INSTALL_PREFIX}
${CMAKE_BINARY_DIR}/sailfishos/src/harbour-fuoten:bin
")
