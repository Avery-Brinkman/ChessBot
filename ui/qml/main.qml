import QtQuick
import QtQuick.Controls
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

            TableView {
                id: board
                Layout.preferredWidth: gridSize * 8
                Layout.preferredHeight: gridSize * 8
                model: boardModel

                delegate: Rectangle {
                    implicitWidth: gridSize
                    implicitHeight: gridSize
                    color: (row + column) % 2 == 0 ? "#cfa08c" : "#7d4b36"

                    Text {
                        anchors.fill: parent
                        visible: boardModel.debugPanel.showRankAndFile
                        text: model.rankAndFile
                    }

                    Rectangle {
                        anchors {
                            fill: parent
                            margins: 2
                        }
                        radius: height / 2
                        color: {
                            if (model.validMove)
                                return "green";
                            if (boardModel.debugPanel.debugEnabled && model.debugInfo)
                                return "purple";
                            return "transparent";
                        }
                    }

                    Image {
                        anchors.fill: parent
                        source: model.pieceImage
                    }

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        onClicked: (mouse) => { 
                            if (tabBar.currentIndex == 2) {
                                if (mouse.button == Qt.RightButton)
                                    model.togglePiece = false;
                                else
                                    model.togglePiece = true;
                            }
                            else if (model.validMove)
                                model.validMove = true;
                            else
                                model.selected = true;
                        }

                    }
                }
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
