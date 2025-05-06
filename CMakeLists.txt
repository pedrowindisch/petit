cmake_minimum_required(VERSION 3.10)
project(petit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Define the executable
add_executable(petit ${SOURCES})

# Add include directories for this target only
target_include_directories(petit PRIVATE ${CMAKE_SOURCE_DIR}/src)
