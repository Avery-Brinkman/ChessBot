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
      acceptedButtons: Qt.LeftButton | Qt.RightButton
      anchors.fill: parent

      onClicked: mouse => {
        if (tabBar.currentIndex == 2) {
          if (mouse.button == Qt.RightButton)
            model.togglePiece = false;
          else
            model.togglePiece = true;
        } else if (model.validMove)
          model.validMove = true;
        else
          model.selected = true;
      }
    }
  }
}
