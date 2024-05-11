cmake_minimum_required(VERSION 3.8)
project(ChessBot LANGUAGES CXX)

option(SKIP_UI "SKIP_UI" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Create the variables that will be used between projects
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/engine/include)
set(ENGINE_NAME ${PROJECT_NAME}_Engine)

message(STATUS "Building Engine")
add_subdirectory(engine)

if(${SKIP_UI})
  message(STATUS "Skipping UI")
else()
  message(STATUS "Building UI")
  add_subdirectory(ui)
endif()
