import QtQuick

Rectangle {
    width: 800
    height: 640
    color: "black"

    Rectangle {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: board.right
            right: parent.right
        }
        color: "grey"
        Column {
            anchors.fill: parent
        }
    }

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
            implicitWidth: 640 / 8
            implicitHeight: 640 / 8
            color: (row + column) % 2 == 0 ? "#cfa08c" : "#7d4b36"

            Rectangle {
                anchors {
                    fill: parent
                    margins: 2
                }
                radius: height / 2
                color: model.hasValidMove ? "red" : "transparent"
            }

            Image {
                anchors.fill: parent
                source: model.pieceImage
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (model.hasValidMove) 
                        model.moveHere = true
                    else
                        model.selected = true
                }
            }
        }
    }
}
