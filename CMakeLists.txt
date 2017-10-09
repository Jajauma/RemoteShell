cmake_minimum_required(VERSION 3.0.0)
project(RemoteShell C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CheckIncludeFile)
check_include_file(sys/socket.h HAVE_SYS_SOCKET_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(netdb.h HAVE_NETDB_H)

include(CheckSymbolExists)
check_symbol_exists(getaddrinfo netdb.h HAVE_GETADDRINFO)
check_symbol_exists(getnameinfo netdb.h HAVE_GETNAMEINFO)
check_symbol_exists(gai_strerror netdb.h HAVE_GAI_STRERROR)

option(BUILD_UNIT_TESTS "Build unit tests" OFF)
include_directories(${CMAKE_CURRENT_LIST_DIR})

add_library(${PROJECT_NAME}.Classes OBJECT
    Cxx/Assert.cpp
    System/FileDescriptor.cpp
    System/GetAddrInfo.cpp
    System/GetNameInfo.cpp
    System/IO.cpp
    System/Shutdown.cpp
    System/Socket.cpp)
add_executable(ShellClient
    ShellClient/Connect.cpp
    ShellClient/Main.cpp
    $<TARGET_OBJECTS:${PROJECT_NAME}.Classes>)
add_executable(ShellServer
    ShellServer/Main.cpp
    $<TARGET_OBJECTS:${PROJECT_NAME}.Classes>)

if (BUILD_UNIT_TESTS)
    find_package(GTest REQUIRED)
    target_compile_definitions(${PROJECT_NAME}.Classes PRIVATE
        "BUILD_UNIT_TESTS")
    add_executable(${PROJECT_NAME}.Tests
        $<TARGET_OBJECTS:${PROJECT_NAME}.Classes>)
    target_link_libraries(${PROJECT_NAME}.Tests GTest::Main)
    target_link_libraries(ShellClient GTest::GTest)
    target_link_libraries(ShellServer GTest::GTest)
    enable_testing()
    add_test(Tests ${PROJECT_NAME}.Tests)
endif()
