import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: debugPanel
    property var model
    property alias currentIndex: stackLayout.currentIndex

    implicitWidth: 400
    implicitHeight: 400

    color: "grey"

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        Column {
            Layout.fillWidth: true

            Switch {
                width: parent.width
                text: "Show Rank and File"
                checked: model.debugPanel.showRankAndFile
                onCheckedChanged: model.debugPanel.showRankAndFile = checked
            }

            Switch {
                width: parent.width
                text: "Show Index"
                checked: model.debugPanel.showIndex
                onCheckedChanged: model.debugPanel.showIndex = checked
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

            Switch {
                width: parent.width
                text: "Show bits"
                checked: model.debugPanel.showBits
                onCheckedChanged: model.debugPanel.showBits = checked
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

            Text {
                width: parent.width
                text: "Click to toggle white, right click to toggle black"
                wrapMode: Text.WordWrap
            }

            ComboBox {
                width: parent.width
                model: ["Pawn", "Knight", "Bishop", "Rook", "Queen", "King"]
                currentIndex: debugPanel.model.debugPanel.pieceType - 1
                onCurrentIndexChanged: debugPanel.model.debugPanel.pieceType = currentIndex + 1
            }
        }
    }
}