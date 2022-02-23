# SPDX-FileCopyrightText: (C) 2016-2021 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)
cmake_policy(SET CMP0048 NEW)

project(fuoten
        VERSION 0.7.1
        DESCRIPTION "Qt based client for the Nextcloud News Application"
        HOMEPAGE_URL "https://github.com/Huessenbergnetz/Fuoten"
        LANGUAGES CXX)

# Auto generate moc files
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Forbid in-tree building
if(${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
      message(STATUS "Please do an out-of-tree build:")
      message(STATUS "rm -f CMakeCache.txt && mkdir build && cd build; cmake .. && make")
      message(FATAL_ERROR "In-tree-build detected!")
endif(${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})

include(GNUInstallDirs)
include(FindPkgConfig)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "libfuoten default install prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

option(ENABLE_MAINTAINER_FLAGS "Enables some build flags used for development" OFF)
set(TARGETSYSTEM "sfos" CACHE STRING "Target system to build the application for, currently supported: sfos, kirigami")
set(FUOTEN_TRANSDIR "${CMAKE_INSTALL_DATADIR}/nazzida/translations" CACHE PATH "Installation destination for translation files.")

if(${TARGETSYSTEM} STREQUAL "sfos")
    message(STATUS "Building Fuoten for SailfishOS")

    find_package(Qt5 COMPONENTS Core Quick Network Sql DBus LinguistTools REQUIRED)

    set(HBNSC_APPNAME harbour-fuoten)
    set(FUOTEN_EXE_OUTPUT_NAME harbour-fuoten)
    set(CMAKE_INSTALL_LIBDIR share/${HBNSC_APPNAME}/lib)
    set(HBNSC_WITH_FIRFUORIDA OFF)
    set(FUOTEN_TRANSDIR share/${FUOTEN_EXE_OUTPUT_NAME}/translations)
    set(LIBFUOTEN_I18NDIR ${FUOTEN_TRANSDIR})

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/HBN_SFOS_Cmake)

    add_subdirectory(libfuoten)
    add_subdirectory(HBN_SFOS_Cmake)
    add_subdirectory(HBN_SFOS_Components)
    add_subdirectory(sailfishos)

    add_custom_target(specfiles
        SOURCES
            rpm/harbour-fuoten.changes
            rpm/harbour-fuoten.spec
            rpm/harbour-fuoten.yaml
    )
endif(${TARGETSYSTEM} STREQUAL "sfos")