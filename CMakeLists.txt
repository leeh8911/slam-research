cmake_minimum_required(VERSION 3.10)

project(CONAN_TEST)

cmake_policy(SET CMP0079 NEW)

set(PROJECT_EXE ${PROJECT_NAME}_EXE)
set(PROJECT_LIB ${PROJECT_NAME}_LIB)
set(PROJECT_TEST ${PROJECT_NAME}_TEST)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(src)
add_subdirectory(test)

find_program(CLANGTIDY clang-tidy)
if(CLANGTIDY)
message(STATUS "activate clang-tidy")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY "${CLANGTIDY};-extra-arg=-Wno-unknown-warning-option;-fix")
else()
message(SEND_ERROR "clang-tidy requested but executable not found")
endif()