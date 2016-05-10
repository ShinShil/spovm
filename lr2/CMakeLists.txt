cmake_minimum_required(VERSION 2.8)

project(lr2)

include_directories(headers)

add_executable(main main.cpp)
add_executable(printer printer.cpp)
add_executable(input input.cpp)

add_subdirectory(lib)

target_link_libraries(main lib)
target_link_libraries(printer lib)
target_link_libraries(input lib)