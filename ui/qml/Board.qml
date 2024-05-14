import QtQuick
import QtQuick.Controls

TableView {
    property int gridSize

    delegate: Rectangle {
        implicitWidth: gridSize
        implicitHeight: gridSize
        color: (row + column) % 2 == 0 ? "#cfa08c" : "#7d4b36"

        Text {
            anchors {
                fill: parent
                margins: 2
            }
            visible: boardModel.debugPanel.showRankAndFile
            text: model.rankAndFile
        }

        Text {
            anchors {
                fill: parent
                margins: 2
            }
            horizontalAlignment: Text.AlignRight
            visible: boardModel.debugPanel.showIndex
            text: model.boardIndex
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
