cmake_minimum_required(VERSION 3.15)
project(afxdp-telemetry)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

file(GLOB_RECURSE TELEMETRY_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(telemetry ${TELEMETRY_SOURCES})
