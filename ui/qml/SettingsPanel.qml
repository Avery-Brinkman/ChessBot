import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts

Rectangle {
  id: settingsPanel

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
        checked: model.settingsPanel.showRankAndFile
        text: "Show Rank and File"
        width: parent.width

        onCheckedChanged: model.settingsPanel.showRankAndFile = checked
      }

      Switch {
        checked: model.settingsPanel.showIndex
        text: "Show Index"
        width: parent.width

        onCheckedChanged: model.settingsPanel.showIndex = checked
      }
    }

    Column {
      Layout.fillWidth: true

      Switch {
        checked: model.settingsPanel.debugEnabled
        text: "Show debug info"
        width: parent.width

        onCheckedChanged: model.settingsPanel.debugEnabled = checked
      }

      Switch {
        checked: model.settingsPanel.showBits
        text: "Show bits"
        width: parent.width

        onCheckedChanged: model.settingsPanel.showBits = checked
      }

      Switch {
        checked: model.settingsPanel.showWhite
        text: "Show white"
        width: parent.width

        onCheckedChanged: model.settingsPanel.showWhite = checked
      }

      Switch {
        checked: model.settingsPanel.showBlack
        text: "Show black"
        width: parent.width

        onCheckedChanged: model.settingsPanel.showBlack = checked
      }

      Switch {
        checked: model.settingsPanel.enPassant
        text: "En passant"
        width: parent.width

        onCheckedChanged: model.settingsPanel.enPassant = checked
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

          checked: model.settingsPanel.pieceType == index + 1
          text: `<font color='black'>${modelData}</font>`
          width: parent.width

          background: Rectangle {
            color: checked ? "lightgrey" : "transparent"
          }

          onCheckedChanged: {
            if (checked)
              model.settingsPanel.pieceType = index + 1;
          }
        }
      }
    }
  }
}
