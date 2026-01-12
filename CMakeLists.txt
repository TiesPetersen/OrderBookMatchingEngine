cmake_minimum_required(VERSION 3.20)

project(HighPerformanceMarketEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "No build type specified. Defaulting to Release.")
endif()
message(STATUS "BUILD TYPE: ${CMAKE_BUILD_TYPE}")

if(MSVC)
    # --- WINDOWS (Visual Studio) ---
    # /O2   = Maximize Speed (Equivalent to -O3)
    # /Ot   = Favor Code Speed
    # /Ob2  = Aggressive Inlining
    # /Oi   = Enable Intrinsic Functions
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ot /Ob2 /Oi /DNDEBUG")
    
    # Debug flags
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Ob0 /Od /RTC1")
else()
    # --- LINUX / MAC (Clang / GCC) ---
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(matching_engine_lib 
    src/matching_engine/Order.cpp
    src/matching_engine/OrderBook.cpp
)

add_executable(matching_engine src/main.cpp)
target_link_libraries(matching_engine matching_engine_lib)

add_executable(benchmark_engine benchmarks/bench_matching_engine.cpp)
target_link_libraries(benchmark_engine matching_engine_lib)

enable_testing()

add_executable(unit_tests
    tests/test_order_book.cpp
)
target_link_libraries(unit_tests matching_engine_lib)
add_test(NAME MatchingEngineTests COMMAND unit_tests)