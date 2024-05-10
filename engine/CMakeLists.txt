set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(ENGINE_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/BitBoard.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Board.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Bot.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Engine.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Move.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Piece.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/SlidingPieceBitBoards.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/UCICommands.h
)

set(ENGINE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/BitBoard.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Board.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Bot.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Move.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Piece.cpp
)

set(ENGINE_CLI_INCLUDES
)

set(ENGINE_CLI_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

# Create the engine dll
add_library(${ENGINE_NAME} SHARED ${ENGINE_INCLUDES} ${ENGINE_SOURCES})
target_include_directories(${ENGINE_NAME} PRIVATE ${ENGINE_INCLUDE_DIR})

# Create a CLI executable
add_executable(${PROJECT_NAME} ${ENGINE_CLI_SOURCES} ${ENGINE_CLI_INCLUDES})
target_link_libraries(${PROJECT_NAME} ${ENGINE_NAME})
target_include_directories(${PROJECT_NAME} PRIVATE ${ENGINE_INCLUDE_DIR})
