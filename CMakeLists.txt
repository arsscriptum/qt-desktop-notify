cmake_minimum_required(VERSION 3.14)
project(qt-desktop-notify)

set(CMAKE_CXX_STANDARD 17)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(qt-desktop-notify src/main.cpp)
target_link_libraries(qt-desktop-notify Qt6::Widgets)
