set(UI_NAME ${PROJECT_NAME}_UI)

cmake_policy(SET CMP0071 NEW)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick)
qt_standard_project_setup(REQUIRES 6.5)

set(SRC_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/BitboardsModel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/BoardModel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/SettingsPanel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

set(INCLUDE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/BitboardsModel.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/BoardModel.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/SettingsPanel.h
)

set(QML_FILES
  qml/BitboardDisplays.qml
  qml/Board.qml
  qml/SettingsPanel.qml
  qml/main.qml
)

set(IMAGES
  images/blackBishop.png
  images/blackKing.png
  images/blackKnight.png
  images/blackPawn.png
  images/blackQueen.png
  images/blackRook.png
  images/whiteBishop.png
  images/whiteKing.png
  images/whiteKnight.png
  images/whitePawn.png
  images/whiteQueen.png
  images/whiteRook.png
)

set(ICONS
  images/copy-icon.svg
  images/paste-icon.svg
)

add_executable(${UI_NAME} ${SRC_FILES} ${INCLUDE_FILES})

target_link_libraries(${UI_NAME} PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Quick

  # Link the engine dll from the engine project
  ${ENGINE_NAME}
)

target_include_directories(${UI_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${ENGINE_INCLUDE_DIR}
)

qt_add_resources(${UI_NAME} "qml"
  FILES
    ${QML_FILES}
    ${IMAGES}
    ${ICONS}
)
