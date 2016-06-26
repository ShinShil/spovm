cmake_minimum_required (VERSION 2.8)

project(lr5)

include_directories(headers)

add_executable(main main.cpp)

add_subdirectory(dll)

target_link_libraries(main _dll)