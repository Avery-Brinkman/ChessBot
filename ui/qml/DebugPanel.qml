import QtQuick
import QtQuick.Controls

Column {
    id: column
    property var model

    Switch {
        id: builderSwitch
        width: parent.width
        text: "Bitboard builder"
        checked: model.debugPanel.bitboardBuilder
        onCheckedChanged: model.debugPanel.bitboardBuilder = checked
    }

    Switch {
        width: parent.width
        visible: !builderSwitch.checked
        text: "Show debug info"
        checked: model.debugPanel.debugEnabled
        onCheckedChanged: model.debugPanel.debugEnabled = checked
    }

    Switch {
        width: parent.width
        visible: !builderSwitch.checked
        text: "Show white"
        checked: model.debugPanel.showWhite
        onCheckedChanged: model.debugPanel.showWhite = checked
    }

    Switch {
        width: parent.width
        visible: !builderSwitch.checked
        text: "Show black"
        checked: model.debugPanel.showBlack
        onCheckedChanged: model.debugPanel.showBlack = checked
    }

    Switch {
        width: parent.width
        visible: !builderSwitch.checked
        text: "En passant"
        checked: model.debugPanel.enPassant
        onCheckedChanged: model.debugPanel.enPassant = checked
    }
}