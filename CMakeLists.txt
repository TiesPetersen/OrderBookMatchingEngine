cmake_minimum_required(VERSION 3.20)

project(HighPerformanceMarketEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization flags are critical for your resume metrics!
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG") 

# Tell CMake where to look for .hpp files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Create the library from the source files directly in src/
add_library(matching_engine 
    src/Order.cpp 
    src/OrderBook.cpp
)

# Benchmark executable
add_executable(bench_engine benchmarks/bench.cpp)
target_link_libraries(bench_engine matching_engine)