cmake_minimum_required(VERSION 2.8)
project(skip_map)

IF(APPLE)
    SET(CMAKE_CXX_COMPILER clang++)
ELSE()
    SET(CMAKE_CXX_COMPILER g++-5)
ENDIF()

SET(CXX_COMPILE_FLAGS "-Wall -Wextra -Wfatal-errors -pedantic --std=c++14 -g")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}" )

set(TEST_SOURCES src/tests.cpp)
set(BENCH_SOURCES src/bench.cpp)

find_package(GTest REQUIRED)
find_package (Threads REQUIRED)
find_package(benchmark REQUIRED)

include_directories(${benchmark_INCLUDE_DIR} ${GTEST_INCLUDE_DIRS} )

add_executable(bench ${BENCH_SOURCES})
target_link_libraries(bench benchmark)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
