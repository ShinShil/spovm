cmake_minimum_required(VERSION 2.8)

project(lr3)

include_directories(headers)

add_executable(client client.cpp)
add_executable(server server.cpp)

add_subdirectory(lib)
target_link_libraries(client lib)
target_link_libraries(server lib)
