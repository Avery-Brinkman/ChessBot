import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    property var model

    implicitWidth: 400
    implicitHeight: 400

    color: "grey"

    TabBar {
        id: tabBar
        width: parent.width

        Repeater {
            model: ["General", "Bitboards", "Builders"]
            TabButton {
                text: modelData
            }
        }
    }

    StackLayout {
        anchors {
            top: tabBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        currentIndex: tabBar.currentIndex

        Column {
            Layout.fillWidth: true

            Switch {
                width: parent.width
                text: "Show Rank and File"
                checked: model.debugPanel.showRankAndFile
                onCheckedChanged: model.debugPanel.showRankAndFile = checked
            }
        }

        Column {
            Layout.fillWidth: true

            Switch {
                width: parent.width
                text: "Show debug info"
                checked: model.debugPanel.debugEnabled
                onCheckedChanged: model.debugPanel.debugEnabled = checked
            }

            Switch  {
                width: parent.width
                text: "Show white"
                checked: model.debugPanel.showWhite
                onCheckedChanged: model.debugPanel.showWhite = checked
            }

            Switch {
                width: parent.width
                text: "Show black"
                checked: model.debugPanel.showBlack
                onCheckedChanged: model.debugPanel.showBlack = checked
            }

            Switch {
                width: parent.width
                text: "En passant"
                checked: model.debugPanel.enPassant
                onCheckedChanged: model.debugPanel.enPassant = checked
            }
        }

        Column {
            Layout.fillWidth: true
        }
    }
}