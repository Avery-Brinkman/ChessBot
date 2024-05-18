import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts

Rectangle {
  id: debugPanel

  property alias currentIndex: stackLayout.currentIndex
  property var model

  color: "grey"
  implicitHeight: 400
  implicitWidth: 400

  StackLayout {
    id: stackLayout

    anchors.fill: parent

    Column {
      Layout.fillWidth: true

      Switch {
        checked: model.debugPanel.showRankAndFile
        text: "Show Rank and File"
        width: parent.width

        onCheckedChanged: model.debugPanel.showRankAndFile = checked
      }

      Switch {
        checked: model.debugPanel.showIndex
        text: "Show Index"
        width: parent.width

        onCheckedChanged: model.debugPanel.showIndex = checked
      }
    }

    Column {
      Layout.fillWidth: true

      Switch {
        checked: model.debugPanel.debugEnabled
        text: "Show debug info"
        width: parent.width

        onCheckedChanged: model.debugPanel.debugEnabled = checked
      }

      Switch {
        checked: model.debugPanel.showBits
        text: "Show bits"
        width: parent.width

        onCheckedChanged: model.debugPanel.showBits = checked
      }

      Switch {
        checked: model.debugPanel.showWhite
        text: "Show white"
        width: parent.width

        onCheckedChanged: model.debugPanel.showWhite = checked
      }

      Switch {
        checked: model.debugPanel.showBlack
        text: "Show black"
        width: parent.width

        onCheckedChanged: model.debugPanel.showBlack = checked
      }

      Switch {
        checked: model.debugPanel.enPassant
        text: "En passant"
        width: parent.width

        onCheckedChanged: model.debugPanel.enPassant = checked
      }
    }

    Column {
      Layout.fillWidth: true

      Text {
        text: "Click to toggle white, right click to toggle black"
        width: parent.width
        wrapMode: Text.WordWrap
      }

      Repeater {
        model: ["Pawn", "Knight", "Bishop", "Rook", "Queen", "King"]

        RadioDelegate {
          required property int index
          required property var modelData

          checked: model.debugPanel.pieceType == index + 1
          text: `<font color='black'>${modelData}</font>`
          width: parent.width

          background: Rectangle {
            color: checked ? "lightgrey" : "transparent"
          }

          onCheckedChanged: {
            if (checked)
              model.debugPanel.pieceType = index + 1;
          }
        }
      }
    }
  }
}
