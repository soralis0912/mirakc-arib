# SPDX-License-Identifier: GPL-2.0-or-later

# Build LibISDB on Windows.
#
# The upstream CMakeLists.txt is for non-Windows platforms.  On Windows, FileStreamWindows.cpp is
# added, and LIBISDB_FORCE_UTF8 is defined in order to output UTF-8 strings like other platforms.

cmake_minimum_required(VERSION 3.13.0)

project(LibISDB-windows CXX)

if(NOT LIBISDB_SOURCE_DIR)
  message(FATAL_ERROR "LIBISDB_SOURCE_DIR is required")
endif()

add_compile_definitions(LIBISDB_FORCE_UTF8)

# See libcxx_char_traits.hpp.
add_compile_options(-include ${CMAKE_CURRENT_SOURCE_DIR}/../compat/libcxx_char_traits.hpp)

add_subdirectory(${LIBISDB_SOURCE_DIR}/LibISDB LibISDB)

target_sources(LibISDB PRIVATE ${LIBISDB_SOURCE_DIR}/LibISDB/Base/FileStreamWindows.cpp)

# Use the same library name as other platforms.
set_target_properties(LibISDB PROPERTIES OUTPUT_NAME "isdb")
