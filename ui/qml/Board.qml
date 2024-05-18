import QtQuick
import QtQuick.Controls.Universal

TableView {
  property int gridSize

  delegate: Rectangle {
    color: (row + column) % 2 == 0 ? "#cfa08c" : "#7d4b36"
    implicitHeight: gridSize
    implicitWidth: gridSize

    Text {
      text: model.rankAndFile
      visible: boardModel.settingsPanel.showRankAndFile

      anchors {
        fill: parent
        margins: 2
      }
    }

    Text {
      horizontalAlignment: Text.AlignRight
      text: model.boardIndex
      visible: boardModel.settingsPanel.showIndex

      anchors {
        fill: parent
        margins: 2
      }
    }

    Rectangle {
      color: {
        if (model.validMove)
          return "green";
        if (boardModel.settingsPanel.debugEnabled && model.debugInfo)
          return "purple";
        return "transparent";
      }
      radius: height / 2

      anchors {
        fill: parent
        margins: 2
      }
    }

    Image {
      anchors.fill: parent
      source: model.pieceImage
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
