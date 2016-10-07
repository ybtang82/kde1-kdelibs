#=============================================================================
# Copyright 2016 Helio Chissini de Castro <helio@kde.org>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

set(KDE1_FOUND 0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(X11 REQUIRED)
find_package(Qt1 REQUIRED)
find_package(Gettext REQUIRED)

include(CMakePackageConfigHelpers)
include(KDE11Macros)
include(KDE1InstallDirs)

if(NOT ${BOOTSTRAP})
    if(UNIX)
        message(STATUS "Looking for KDE1 headers and libraries")
        find_path(KDE1_INCLUDE_DIR kapp.h ${KDE1_INCLUDES})
        find_library(KDE_LIBRARIES kdecore ${KDE1_LIBRARIES})
    endif()

    # handle the QUIETLY and REQUIRED arguments and set MOTIF_FOUND to TRUE if
    # all listed variables are TRUE
    include(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(KDE1 DEFAULT_MSG KDE1_LIBRARIES KDE1_INCLUDE_DIR)

    mark_as_advanced(
        KDE1_INCLUDE_DIR
        KDE1_LIBRARIES
    )
endif()
