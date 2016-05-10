cmake_minimum_required(VERSION 2.8)

project(lib)

set(SOURCE_DLL FileAIO.cpp)

add_library(_dll shared ${SOURCE_DLL})