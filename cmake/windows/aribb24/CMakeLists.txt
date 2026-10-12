# SPDX-License-Identifier: GPL-2.0-or-later

# Build aribb24 on Windows.
#
# The upstream build system uses autotools which is not available on Windows in most cases.  This
# file builds a static library in the same way as:
#
#   ./configure --disable-shared --with-pic --without-libpng

cmake_minimum_required(VERSION 3.13.0)

project(aribb24 C)

if(NOT ARIBB24_SOURCE_DIR)
  message(FATAL_ERROR "ARIBB24_SOURCE_DIR is required")
endif()

add_library(aribb24 STATIC
  ${ARIBB24_SOURCE_DIR}/src/aribb24.c
  ${ARIBB24_SOURCE_DIR}/src/decoder.c
  ${ARIBB24_SOURCE_DIR}/src/drcs.c
  ${ARIBB24_SOURCE_DIR}/src/md5.c
  ${ARIBB24_SOURCE_DIR}/src/parser.c
)

target_include_directories(aribb24 PRIVATE ${ARIBB24_SOURCE_DIR}/src)

# asprintf() and vasprintf() are provided by MinGW-w64 with the following definitions.
target_compile_definitions(aribb24
  PRIVATE
    _GNU_SOURCE
    __USE_MINGW_ANSI_STDIO=1
    HAVE_VASPRINTF=1
)

install(TARGETS aribb24 ARCHIVE DESTINATION lib)
install(DIRECTORY ${ARIBB24_SOURCE_DIR}/src/aribb24 DESTINATION include)
