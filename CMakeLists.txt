cmake_minimum_required(VERSION 3.20)

project(HighPerformanceMarketEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "No build type specified. Defaulting to Release.")
endif()
message(STATUS "BUILD TYPE: ${CMAKE_BUILD_TYPE}")

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ot /Ob2 /Oi /DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Ob0 /Od /RTC1")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -funroll-loops -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(matching_engine_lib 
    src/matching_engine/Order.cpp
    src/matching_engine/OrderBook.cpp
)

add_executable(run_engine src/main.cpp)
target_link_libraries(run_engine matching_engine_lib)

add_executable(benchmark_engine benchmarks/bench_matching_engine.cpp)
target_link_libraries(benchmark_engine matching_engine_lib)

enable_testing()

add_executable(test_engine
    tests/test_order_book.cpp
    tests/TestUtils.cpp
    tests/TestCases.cpp
)
target_link_libraries(test_engine matching_engine_lib)
add_test(NAME MatchingEngineTests COMMAND test_engine)