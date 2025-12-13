cmake_minimum_required(VERSION 4.2)

project(HighPerformanceMarketEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Clang-Tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    message(STATUS "Clang-Tidy found: ${CLANG_TIDY_EXE}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}") 
else()
    message(WARNING "Clang-Tidy not found! Skipping linting.")
endif()

file(GLOB PROTOCOL_SRCS "src/protocol_layer/*.cpp")

file(GLOB ENGINE_SRCS "src/matching_engine/*.cpp")

add_executable(market_engine 
    ${ENGINE_SRCS} 
    ${PROTOCOL_SRCS}
)

file(GLOB CLIENT_SRCS "src/trading_client/*.cpp")

add_executable(trading_client 
    ${CLIENT_SRCS} 
    ${PROTOCOL_SRCS}
)