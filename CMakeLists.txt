#
# Project details
#
cmake_minimum_required(VERSION 3.27)

project(cpp_project_template)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For clangd

# Build type settings
if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
  set(CMAKE_C_FLAGS "-fsanitize=address -DDEBUG -g -Wall -O0")
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
  set(CMAKE_C_FLAGS "-DNDEBUG -O3")
endif()

message("Compiler: ${CMAKE_CXX_COMPILER}")
message("Build type: ${CMAKE_BUILD_TYPE}")
message("Flags: ${CMAKE_C_FLAGS}")

#
# Building and including path
#
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/static) # STATIC
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/shared) # SHARED
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) # EXE

include_directories(${PROJECT_SOURCE_DIR}/src/include)

#
# Subdirectories
#
add_subdirectory(src)
add_subdirectory(test)
