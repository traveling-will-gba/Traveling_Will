set (ENGINE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/engine/include")
set (ENGINE_SRC_DIR     "${PROJECT_SOURCE_DIR}/engine/src")

file(GLOB_RECURSE SOURCES "${ENGINE_SRC_DIR}/*.cpp")

include_directories("${ENGINE_INCLUDE_DIR}")
add_library(engine STATIC ${SOURCES})
target_compile_options(engine PRIVATE -shared -rdynamic -fPIC)
