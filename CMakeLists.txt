cmake_minimum_required(VERSION 3.15)
project(afxdp-telemetry)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

add_executable(telemetry src/main.cpp)
