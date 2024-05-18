import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts

Rectangle {
  id: root

  property int gridSize: Math.min(root.width, root.height - tabBar.height) / 8

  color: "black"

  TabBar {
    id: tabBar

    width: parent.width

    Repeater {
      model: ["General", "Add/Remove Pieces"]

      TabButton {
        text: modelData
      }
    }
  }

  ColumnLayout {
    spacing: 0

    anchors {
      bottom: parent.bottom
      left: parent.left
      right: parent.right
      top: tabBar.bottom
    }

    RowLayout {
      Layout.fillHeight: true
      Layout.fillWidth: true
      spacing: 0

      Board {
        id: board

        Layout.preferredHeight: gridSize * 8
        Layout.preferredWidth: gridSize * 8
        gridSize: root.gridSize
        model: boardModel
      }

      SettingsPanel {
        Layout.fillHeight: true
        Layout.fillWidth: true
        currentIndex: tabBar.currentIndex
        model: boardModel
      }
    }
  }
}
