cmake_minimum_required(VERSION 3.8)
project(ChessBot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Create the variables that will be used between projects
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/engine/include)
set(ENGINE_NAME ${PROJECT_NAME}_Engine)

add_subdirectory(engine)
add_subdirectory(ui)
