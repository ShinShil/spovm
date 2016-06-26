cmake_minimum_required(VERSION 2.8)

project(lib)

set(SOURCE_LIB fileSystemUser.cpp fileCommands.cpp)

add_library(lib STATIC ${SOURCE_LIB})