cmake_minimum_required(VERSION 2.8)
project(skip_map)

IF(APPLE)
    SET(CMAKE_CXX_COMPILER clang++)
ELSE()
    SET(CMAKE_CXX_COMPILER g++-5)
ENDIF()

SET(CXX_COMPILE_FLAGS "-Wall -Wextra -Wfatal-errors -pedantic --std=c++14 -g")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}" )

#Find all cpp files recursively
set(SOURCES src/tests.cpp)

add_executable(skip_map ${SOURCES})

find_package(GTest REQUIRED)
find_package (Threads)

include_directories( ${GTEST_INCLUDE_DIRS} )
target_link_libraries(skip_map ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
