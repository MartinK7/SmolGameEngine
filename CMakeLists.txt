cmake_minimum_required(VERSION 3.10)
project(SGE_Demo)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# the `pkg_check_modules` function is created with this call
find_package(PkgConfig REQUIRED)

# Packages
pkg_check_modules(GLFW3 REQUIRED glfw3)

# System dependent
if (UNIX)
    # Threading
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    set(EXTRA_LIBS m dl Threads::Threads)
else()
    # ...
endif (UNIX)

# Obtain project sources
file(GLOB_RECURSE SOURCES
    third_party/*.c
    src/*.cpp
    demo/*.cpp
)
#file(GLOB_RECURSE REMOVE_SOURCES
#    # ...
#)
#list(REMOVE_ITEM SOURCES ${REMOVE_SOURCES})

# Set new executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Target
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
    # ...
)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    include/
    third_party/glad/
    third_party/stb/
    ${GLFW3_INCLUDE_DIRS}
)
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC
    ${GLFW3_CFLAGS_OTHER}
    -DCONFIG_ASSETS_PATH="../demo/assets/"
)
target_link_libraries(${CMAKE_PROJECT_NAME} ${EXTRA_LIBS}
    ${GLFW3_LIBRARIES}
)

# Custom output directory
add_custom_target(run COMMAND ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME})
