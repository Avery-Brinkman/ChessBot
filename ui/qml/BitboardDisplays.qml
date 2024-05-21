import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts

ColumnLayout {
  readonly property var bitboards: model.bitboards
  readonly property var settingsPanel: model.settingsPanel

  spacing: 2

  Switch {
    Layout.fillWidth: true
    checked: settingsPanel.showBitboards
    text: "Show Bitboards"

    onCheckedChanged: settingsPanel.showBitboards = checked
  }

  Item {
    Layout.fillWidth: true
    Layout.preferredHeight: 10

    Rectangle {
      anchors.centerIn: parent
      color: "black"
      height: 1
      width: parent.width
    }
  }

  ScrollView {
    Layout.fillHeight: true
    Layout.fillWidth: true
    clip: true

    ListView {
      model: bitboards

      delegate: Item {
        height: childrenRect.height
        width: parent.width

        Button {
          id: enableButton

          checkable: true
          text: name
          width: 150
        }

        Text {
          id: bitboardText

          anchors.top: enableButton.bottom
          font.bold: true
          font.pointSize: 120
          fontSizeMode: Text.HorizontalFit
          height: contentHeight
          minimumPointSize: 1
          text: bits
          width: parent.width
        }
      }
    }
  }
}
