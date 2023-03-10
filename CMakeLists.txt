cmake_minimum_required(VERSION 3.10)

project(CONAN_TEST)

cmake_policy(SET CMP0079 NEW)

set(PROJECT_EXE ${PROJECT_NAME}_EXE)
set(PROJECT_LIB ${PROJECT_NAME}_LIB)
set(PROJECT_TEST ${PROJECT_NAME}_TEST)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_CXX_COMPILER MSVC CACHE INTERNAL "")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(OPTION_BUILD_DOCS "Build documentation." OFF)
option(OPTION_TEST_ALL "Execute all test" OFF)
option(CMAKE_TOOLCHAIN_FILE "VCPKG path" "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
MESSAGE(STATUS "CMAKE_TOOLCHAIN_FILE: " ${CMAKE_TOOLCHAIN_FILE})

if (OPTION_TEST_ALL)
add_compile_definitions(TEST_ALL)
endif(OPTION_TEST_ALL)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MTd")

if(OPTION_BUILD_DOCS)
message(STATUS "Build only documentation")
add_subdirectory(docs)
else()
message(STATUS "Build source and test files")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
set(pybind11_DIR C:/dev/vcpkg/installed/x86-windows/share/pybind11)
find_package(pybind11 CONFIG REQUIRED)
find_package(Pangolin CONFIG REQUIRED)

add_subdirectory(src)
add_subdirectory(test)

add_dependencies(${PROJECT_EXE} ${PROJECT_LIB})
add_dependencies(${PROJECT_TEST} ${PROJECT_LIB})
endif()

find_program(CLANGTIDY clang-tidy)
if(CLANGTIDY)
message(STATUS "activate clang-tidy")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY "${CLANGTIDY};-extra-arg=-Wno-unknown-warning-option,-std=c++17;-fix")
else()
message(SEND_ERROR "clang-tidy requested but executable not found")
endif()