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

# eBPF program compile
find_program(CLANG clang REQUIRED)

set(XDP_SOURCE ${CMAKE_SOURCE_DIR}/xdp/pass.c)
set(XDP_OBJECT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/xdp_pass.o)

add_custom_command(
  OUTPUT ${XDP_OBJECT}
  COMMAND ${CLANG}
    -O2
    -g
    -target bpf
    -I/usr/include/x86_64-linux-gnu
    -c ${XDP_SOURCE}
    -o ${XDP_OBJECT}
  DEPENDS ${XDP_SOURCE}
)

add_custom_target(xdp_pass ALL
  DEPENDS ${XDP_OBJECT}
)
