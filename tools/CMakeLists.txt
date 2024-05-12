set(TOOLS_NAME ${PROJECT_NAME}_Tools)

set(TOOL_INCLUDES
  # ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

set(TOOL_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

add_executable(${TOOLS_NAME} ${TOOL_SOURCES} ${TOOL_INCLUDES})
target_link_libraries(${TOOLS_NAME} ${ENGINE_NAME})
target_include_directories(${TOOLS_NAME} PRIVATE ${ENGINE_INCLUDE_DIR})
