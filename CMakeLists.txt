cmake_minimum_required(VERSION 3.0.2)
project(traveling_will LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (ENGINE_DIR "${PROJECT_SOURCE_DIR}/engine")
set (KERNEL_DIR "${PROJECT_SOURCE_DIR}/engine/kernel/sdl2")
set (INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set (SRC_DIR     "${PROJECT_SOURCE_DIR}/src")
set (ASSETS_DIR  "${PROJECT_SOURCE_DIR}/res")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

include_directories("${INCLUDE_DIR}")

add_subdirectory(${ENGINE_DIR})
include_directories(${ENGINE_INCLUDE_DIR})

add_subdirectory(${KERNEL_DIR})
include_directories(${KERNEL_DIR})

# The CMAKE_DL_LIBS is to prevent C++11 errors

add_executable(traveling_will ${SOURCES})
set_property(TARGET traveling_will PROPERTY CXX_STANDARD 11)
target_link_libraries(traveling_will
                        engine
                        kernel
                        SDL2 SDL2_image SDL2_ttf SDL2_mixer
                        ${CMAKE_DL_LIBS})
