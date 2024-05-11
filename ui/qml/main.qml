import QtQuick

Rectangle {
    id: root

    color: "black"

    TableView {
        id: board
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
        }
        width: height
        model: boardModel

        delegate: Rectangle {
            implicitWidth: board.width / 8
            implicitHeight: board.height / 8
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
                visible: !boardModel.debugPanel.bitboardBuilder
                source: model.pieceImage
            }


            Text {
                anchors.fill: parent
                fontSizeMode: Text.Fit
                font.pixelSize: height

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                visible: boardModel.debugPanel.bitboardBuilder
                text: model.bitBoard ? "1" : "0"
                
                Rectangle {
                    anchors.fill: parent
                    z: -1
                    color: "grey"
                    opacity: 0.5
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (boardModel.debugPanel.bitboardBuilder) {
                        model.bitBoard = true;
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
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: board.right
            right: parent.right
        }
        model: boardModel
    }
}
