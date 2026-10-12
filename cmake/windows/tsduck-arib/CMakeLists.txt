# SPDX-License-Identifier: GPL-2.0-or-later

# Build tsduck-arib on Windows.
#
# The upstream build system uses GNU make with shell commands for Linux and macOS, and MSBuild for
# Windows.  This file builds a static library equivalent to `make install-devel` with the
# following variables which mirakc-arib uses on other platforms:
#
#   ARIB=1 NOCURL=1 NODTAPI=1 NOPCSC=1 NOSRT=1 NOTEST=1

cmake_minimum_required(VERSION 3.13.0)

project(tsduck-arib CXX)

if(NOT TSDUCK_SOURCE_DIR)
  message(FATAL_ERROR "TSDUCK_SOURCE_DIR is required")
endif()
if(NOT ARIBB24_INCLUDE_DIR)
  message(FATAL_ERROR "ARIBB24_INCLUDE_DIR is required")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIBTSDUCK_DIR ${TSDUCK_SOURCE_DIR}/src/libtsduck)

# Collect source files and include directories except for other platforms.
file(GLOB_RECURSE TSDUCK_SOURCES ${LIBTSDUCK_DIR}/*.cpp)
file(GLOB_RECURSE TSDUCK_HEADERS ${LIBTSDUCK_DIR}/*.h)
set(TSDUCK_EXCLUDE_REGEX "/(linux|mac|unix)/")
list(FILTER TSDUCK_SOURCES EXCLUDE REGEX "${TSDUCK_EXCLUDE_REGEX}")

# The DirectShow tuner implementation cannot be built with MinGW-w64 because some headers are not
# provided.  mirakc-arib doesn't use tuners in TSDuck.  So, stub implementations are used.
list(FILTER TSDUCK_SOURCES EXCLUDE REGEX "/dtv/windows/")
list(APPEND TSDUCK_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/tsTunerGuts.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tsHiDesDevice.cpp
)
list(FILTER TSDUCK_HEADERS EXCLUDE REGEX "${TSDUCK_EXCLUDE_REGEX}")

set(TSDUCK_INCLUDE_DIRS)
foreach(header ${TSDUCK_HEADERS})
  get_filename_component(dir ${header} DIRECTORY)
  list(APPEND TSDUCK_INCLUDE_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES TSDUCK_INCLUDE_DIRS)

add_library(tsduck STATIC ${TSDUCK_SOURCES})

target_include_directories(tsduck PRIVATE ${TSDUCK_INCLUDE_DIRS} ${ARIBB24_INCLUDE_DIR})

target_compile_definitions(tsduck
  PUBLIC
    TSDUCK_STATIC=1
    TS_ARIB=1
    TS_NO_CURL=1
    TS_NO_DTAPI=1
    TS_NO_PCSC=1
    TS_NOSRT=1
    TS_NO_DIRECTSHOW=1
  PRIVATE
    UNICODE
    _UNICODE
)

# Public headers are installed into a single directory like `make install-devel`.
set(TSDUCK_PUBLIC_HEADERS ${TSDUCK_HEADERS})
list(FILTER TSDUCK_PUBLIC_HEADERS EXCLUDE REGEX "/private/")
list(FILTER TSDUCK_PUBLIC_HEADERS EXCLUDE REGEX "/dtv/windows/")
list(FILTER TSDUCK_PUBLIC_HEADERS INCLUDE REGEX "/ts[^/]*\\.h$")

install(TARGETS tsduck ARCHIVE DESTINATION lib)
install(FILES ${TSDUCK_PUBLIC_HEADERS} ${LIBTSDUCK_DIR}/tsduck.mk DESTINATION include/tsduck)
