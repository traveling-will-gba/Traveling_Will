cmake_minimum_required(VERSION 3.0.2)
project(traveling_will)

# Creating variables

set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(LIB_DIR "${PROJECT_SOURCE_DIR}/libs")
set(IJENGINE_DIR "/usr/local/include/ijengine")
file(GLOB SOURCES "${SRC_DIR}/*.cpp")

# Setting COMPILE_FLAGS (this is from CMAKE)

set(COMPILE_FLAGS "-std=c++11 -W -std=c++0x -Wall -pedantic -Wshadow -O2 -g")


# Including header files

include_directories(${LIB_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${IJENGINE_DIR})

# Creating executable and adding libraries to CMake

add_executable(traveling_will ${SOURCES} ${LIB_DIR})
set_property(TARGET traveling_will PROPERTY CXX_STANDARD 11)
target_link_libraries(traveling_will  SDL2 SDL2_image SDL2_ttf SDL2_mixer ijengine dl)
