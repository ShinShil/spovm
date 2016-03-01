cmake_minimum_required(VERSION 2.8)

project(lr1)

set(SOURCE_EXE main.cpp)
set(SOURCE_EXE2 process.cpp)
include_directories(headers)

add_executable(main ${SOURCE_EXE})
add_executable(process ${SOURCE_EXE2})

add_subdirectory(lib)

target_link_libraries(main lib)
target_link_libraries(process lib)

