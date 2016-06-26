cmake_minimum_required (VERSION 2.8)

project(lr4)

include_directories(headers headers/fileSystemUser)
add_subdirectory(lib)

add_executable(main main.cpp)

target_link_libraries(main lib)

if(UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif()

if(LINUX)
	message(STATUS ">>> Linux")
	TARGET_LINK_LIBRARIES(main pthread)
endif()