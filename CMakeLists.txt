cmake_minimum_required(VERSION 3.0.0)
project(RemoteShell C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_UNIT_TESTS "Build unit tests" OFF)
include_directories(${CMAKE_CURRENT_LIST_DIR})

add_library(${PROJECT_NAME}.Classes OBJECT
    Cxx/Assert.cpp
    System/FileDescriptor.cpp
    System/GetAddrInfo.cpp
    System/GetAddrInfoErrorCategory.cpp
    System/GetNameInfo.cpp
    System/IO.cpp
    System/Listen.cpp
    System/Shutdown.cpp
    System/Socket.cpp)
add_executable(ShellClient
    ShellClient/Connect.cpp
    ShellClient/Main.cpp
    ShellClient/MultiplexIOLoop.cpp
    $<TARGET_OBJECTS:${PROJECT_NAME}.Classes>)
add_executable(ShellServer
    ShellServer/AcceptClientsLoop.cpp
    ShellServer/Bind.cpp
    ShellServer/Main.cpp
    ShellServer/ProcessClientAsync.cpp
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
