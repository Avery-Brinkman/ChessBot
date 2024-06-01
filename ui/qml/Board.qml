import QtQuick
import QtQuick.Controls.Universal

TableView {
  property int gridSize

  delegate: Rectangle {
    color: (row + column) % 2 == 0 ? "#cfa08c" : "#7d4b36"
    implicitHeight: gridSize
    implicitWidth: gridSize

    // Rank and File
    Text {
      text: model.rankAndFile
      visible: boardModel.settingsPanel.showRankAndFile

      anchors {
        fill: parent
        margins: 2
      }
    }

    // Index
    Text {
      horizontalAlignment: Text.AlignRight
      text: model.boardIndex
      visible: boardModel.settingsPanel.showIndex

      anchors {
        fill: parent
        margins: 2
      }
    }

    // Move indicator
    Rectangle {
      color: {
        if (model.validMove)
          return "green";
        return "transparent";
      }
      radius: height / 2

      anchors {
        fill: parent
        margins: 2
      }
    }

    // Image
    Image {
      anchors.fill: parent
      source: model.pieceImage
    }

    // Debug Bits
    Text {
      anchors.fill: parent
      color: "white"
      fontSizeMode: Text.Fit
      horizontalAlignment: Text.AlignHCenter
      text: model.bitboard ? "1" : "0"
      verticalAlignment: Text.AlignVCenter
      visible: boardModel.settingsPanel.showBitboards

      font {
        bold: true
        pointSize: 1200
      }

      Rectangle {
        anchors.fill: parent
        color: model.bitboard ? "blue" : "grey"
        opacity: 0.5
        z: -1
      }
    }

    MouseArea {
      id: mouseArea

      acceptedButtons: Qt.LeftButton | Qt.RightButton
      anchors.fill: parent
      hoverEnabled: true

      states: [
        // Add/Remove Pieces
        State {
          when: tabBar.currentIndex == 2

          PropertyChanges {
            target: mouseArea

            onClicked: mouse => {
              if (mouse.button == Qt.RightButton)
                model.togglePiece = false;
              else
                model.togglePiece = true;
            }
          }

          PropertyChanges {
            target: toolTip
            text: "Add or Remove Piece"
          }
        },

        // Set Custom Bits
        State {
          when: tabBar.currentIndex == 1 && boardModel.settingsPanel.showBitboards && boardModel.bitboards.useCustomValue

          PropertyChanges {
            target: mouseArea

            onClicked: mouse => {
              model.toggleCustomBit = true;
            }
          }

          PropertyChanges {
            target: toolTip
            text: "Toggle Custom Bit"
          }
        },

        // Valid Move
        State {
          when: model.validMove

          PropertyChanges {
            target: mouseArea

            onClicked: mouse => {
              model.validMove = true;
            }
          }

          PropertyChanges {
            delay: 1000
            target: toolTip
            text: "Move to this Square"
          }
        },

        // Select Piece
        State {
          when: model.hasPiece

          PropertyChanges {
            target: mouseArea

            onClicked: mouse => {
              model.selected = true;
            }
          }

          PropertyChanges {
            delay: 1000
            target: toolTip
            text: "Select Piece"
          }
        }
      ]

      ToolTip {
        id: toolTip

        delay: 600
        opacity: visible ? 1 : 0
        visible: mouseArea.containsMouse && text !== ""

        Behavior on opacity {
          NumberAnimation {
            duration: 100
          }
        }
      }
    }
  }
}
