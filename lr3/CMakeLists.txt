cmake_minimum_required(VERSION 2.8)

project(lr3)

include_directories(headers)

add_executable(main main.cpp)
add_executable(process process.cpp)
add_executable(server server.cpp)
add_executable(client client.cpp)

if(UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif()

if(LINUX)
	message(STATUS ">>> Linux")
	TARGET_LINK_LIBRARIES(server semaphore)
	TARGET_LINK_LIBRARIES(client semaphore)
endif()