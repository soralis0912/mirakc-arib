# SPDX-License-Identifier: GPL-2.0-or-later

# Cross-compile for Windows (x86_64, MinGW-w64 ABI) by using Zig as a C/C++ compiler.
#
#   cmake -S . -B build -D CMAKE_BUILD_TYPE=Release \
#     -D CMAKE_TOOLCHAIN_FILE=$(pwd)/toolchain.cmake.d/windows-x86_64-zig.cmake
#
# `zig` must be in PATH.

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(MIRAKC_ARIB_HOST_TRIPLE x86_64-w64-mingw32)

set(CMAKE_C_COMPILER ${CMAKE_CURRENT_LIST_DIR}/zig/zig-cc)
set(CMAKE_CXX_COMPILER ${CMAKE_CURRENT_LIST_DIR}/zig/zig-c++)
set(CMAKE_AR ${CMAKE_CURRENT_LIST_DIR}/zig/zig-ar CACHE FILEPATH "")
set(CMAKE_RANLIB ${CMAKE_CURRENT_LIST_DIR}/zig/zig-ranlib CACHE FILEPATH "")
set(CMAKE_RC_COMPILER ${CMAKE_CURRENT_LIST_DIR}/zig/zig-rc CACHE FILEPATH "")
