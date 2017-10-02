cmake_minimum_required(VERSION 3.0.0)
project(RemoteShell CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(Sources)

add_executable(ShellClient
    Sources/ShellClient.cpp)
add_executable(ShellServer
    Sources/ShellServer.cpp)
