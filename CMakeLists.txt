cmake_minimum_required(VERSION 3.0.2)
project(traveling-will LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (ENGINE_DIR "${PROJECT_SOURCE_DIR}/engine")
set (KERNEL_DIR "${PROJECT_SOURCE_DIR}/engine/kernel/sdl2")
set (INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set (SRC_DIR     "${PROJECT_SOURCE_DIR}/src")
set (ASSETS_DIR  "${PROJECT_SOURCE_DIR}/res")

file(MAKE_DIRECTORY "libs")
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

include_directories("${INCLUDE_DIR}")

add_subdirectory(${ENGINE_DIR})
include_directories(${ENGINE_INCLUDE_DIR})

add_subdirectory(${KERNEL_DIR})
include_directories(${KERNEL_DIR})

# The CMAKE_DL_LIBS is to prevent C++11 errors

add_executable(traveling-will ${SOURCES})
set_property(TARGET traveling-will PROPERTY CXX_STANDARD 11)
target_link_libraries(traveling-will
                        engine
                        kernel
                        SDL2 SDL2_image SDL2_ttf SDL2_mixer
                        ${CMAKE_DL_LIBS})

add_custom_command(TARGET traveling-will COMMAND ${CMAKE_COMMAND} -E copy traveling-will ${PROJECT_SOURCE_DIR})

install(TARGETS traveling-will DESTINATION "/bin")

install(DIRECTORY res DESTINATION "/opt/traveling-will" DIRECTORY_PERMISSIONS
        OWNER_WRITE OWNER_READ OWNER_EXECUTE
        GROUP_WRITE GROUP_READ GROUP_EXECUTE
        WORLD_WRITE WORLD_READ WORLD_EXECUTE
        FILE_PERMISSIONS
        OWNER_WRITE OWNER_READ OWNER_EXECUTE
        GROUP_WRITE GROUP_READ GROUP_EXECUTE
        WORLD_WRITE WORLD_READ WORLD_EXECUTE
)

install(DIRECTORY libs DESTINATION "/opt/traveling-will" DIRECTORY_PERMISSIONS
        OWNER_WRITE OWNER_READ OWNER_EXECUTE
        GROUP_WRITE GROUP_READ GROUP_EXECUTE
        WORLD_WRITE WORLD_READ WORLD_EXECUTE
        FILE_PERMISSIONS
        OWNER_WRITE OWNER_READ OWNER_EXECUTE
        GROUP_WRITE GROUP_READ GROUP_EXECUTE
        WORLD_WRITE WORLD_READ WORLD_EXECUTE
)

SET(CPACK_GENERATOR "DEB")

SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Victor Navarro and Vitor Barbosa")

SET(CPACK_PACKAGE_NAME "traveling-will")
SET(CPACK_PACKAGE_DESCRIPTION "2D musical platform game made with C++ and SDL2")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Traveling Will is a musical platform game which involves time travel. Will is the main characther, who travels to different times, each one with a single musical style. Each level happens in a different time and place, reflecting both in the design and the soundtrack.")
SET(CPACK_PACKAGE_VENDOR "Victor Navarro and Vitor Barbosa")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-2.0-0, libsdl2-image-2.0-0, libsdl2-mixer-2.0-0, libsdl2-ttf-2.0-0")
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_SECTION "misc")
SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

# RPM only
SET(CPACK_RPM_PACKAGE_SUMMARY "Traveling Will is a musical platform game which involves time travel. Will is the main characther, who travels to different times, each one with a single musical style. Each level happens in a different time and place, reflecting both in the design and the soundtrack.")
SET(CPACK_RPM_PACKAGE_NAME "traveling-will")
SET(CPACK_RPM_PACKAGE_VERSION "1.0.0")
SET(CPACK_RPM_PACKAGE_RELEASE "1")
SET(CPACK_RPM_PACKAGE_LICENSE "MIT")
SET(CPACK_RPM_PACKAGE_GROUP)
SET(CPACK_RPM_PACKAGE_VENDOR "Victor Navarro and Vitor Barbosa")
SET(CPACK_RPM_PACKAGE_DESCRIPTION "2D musical platform game made with C++ and SDL2")

INCLUDE(CPack)
