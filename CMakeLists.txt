cmake_minimum_required(VERSION 2.8)
project(skip_map)

SET(CMAKE_CXX_COMPILER g++-5)
SET(CLANG_COMPILE_FLAGS "-Wall -Wextra -Wfatal-errors --std=c++14 -g")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CLANG_COMPILE_FLAGS}" )

#Find all cpp files recursively
file(GLOB_RECURSE SRC "src" "*.cpp")

add_executable(skip_map ${SRC})

find_package(GTest REQUIRED)
find_package (Threads)

include_directories( ${GTEST_INCLUDE_DIRS} )
target_link_libraries(skip_map ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
