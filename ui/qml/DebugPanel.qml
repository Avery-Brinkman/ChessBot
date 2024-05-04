import QtQuick
import QtQuick.Controls

Column {
    id: column
    property var model

    Switch {
        width: parent.width
        text: "Show debug info"
        checked: model.debugEnabled
        onCheckedChanged: model.debugEnabled = checked
    }

    Switch {
        width: parent.width
        text: "Show white"
        checked: model.showWhite
        onCheckedChanged: model.showWhite = checked
    }

    Switch {
        width: parent.width
        text: "Show black"
        checked: model.showBlack
        onCheckedChanged: model.showBlack = checked
    }

    Switch {
        width: parent.width
        text: "En passant"
        checked: model.enPassant
        onCheckedChanged: model.enPassant = checked
    }
}