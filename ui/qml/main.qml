import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts

Rectangle {
    id: root

    property int gridSize: Math.min(root.width, root.height - tabBar.height - bits.height) / 8

    color: "black"

    TabBar {
        id: tabBar
        width: parent.width
        Repeater {
            model: ["General", "Bitboards", "Add/Remove Pieces"]
            TabButton {
                text: modelData
            }
        }
    }

    ColumnLayout {
        anchors {
            top: tabBar.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        spacing: 0

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            Board {
                id: board
                gridSize: root.gridSize
                Layout.preferredWidth: gridSize * 8
                Layout.preferredHeight: gridSize * 8
                model: boardModel
            }

            DebugPanel {
                Layout.fillWidth: true
                Layout.fillHeight: true

                currentIndex: tabBar.currentIndex
                model: boardModel
            }
        }

        Text {
            id: bits

            Layout.fillWidth: true
            Layout.preferredHeight: boardModel.debugPanel.showBits ? implicitHeight : 0
            text: "Hello"
            color: "white"
        }
    }
}
