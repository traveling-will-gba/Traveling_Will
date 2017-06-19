set (KERNEL_SOURCE_DIR "${PROJECT_SOURCE_DIR}/engine/kernel/sdl2")

file(GLOB_RECURSE SOURCES "${KERNEL_SOURCE_DIR}/*.cpp")

include_directories("${KERNEL_SOURCE_DIR}")
add_library(kernel SHARED ${SOURCES})

set (ENGINE_DIR "${PROJECT_SOURCE_DIR}/engine")
set(ENGINE_INCLUDE_DIR "${ENGINE_DIR}/include")
include_directories(${ENGINE_INCLUDE_DIR})

include_directories(${ENGINE_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -shared -rdynamic")

add_executable(kernel.0.1.0.so ${SOURCES})
set_property(TARGET kernel.0.1.0.so PROPERTY CXX_STANDARD 11)
target_link_libraries(kernel.0.1.0.so engine SDL2 SDL2_image SDL2_ttf SDL2_mixer dl)
target_compile_options(kernel.0.1.0.so PRIVATE -shared -rdynamic -fPIC)