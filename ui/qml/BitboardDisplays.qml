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

  Switch {
    id: customValueSwitch

    Layout.fillWidth: true
    checked: bitboards.useCustomValue
    text: "Use Custom Value"

    onCheckedChanged: bitboards.useCustomValue = checked
  }

  Text {
    Layout.fillWidth: true
    font.pointSize: 12
    text: "Number Mode"
  }

  RadioButton {
    Layout.fillWidth: true
    checked: bitboards.showAsBin
    text: "Binary"

    onCheckedChanged: bitboards.showAsBin = checked
  }

  RadioButton {
    Layout.fillWidth: true
    checked: bitboards.showAsDec
    text: "Decimal"

    onCheckedChanged: bitboards.showAsDec = checked
  }

  RadioButton {
    Layout.fillWidth: true
    checked: bitboards.showAsHex
    text: "Hexadecimal"

    onCheckedChanged: bitboards.showAsHex = checked
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

  TextField {
    Layout.fillWidth: true
    text: bitboards.customValue
    visible: customValueSwitch.checked

    onEditingFinished: {
      bitboards.customValue = text;
      focus = false;
    }
  }

  ScrollView {
    Layout.fillHeight: true
    Layout.fillWidth: true
    clip: true
    visible: !customValueSwitch.checked

    ListView {
      model: bitboards
      width: parent.width

      delegate: Item {
        height: childrenRect.height
        width: parent.width

        Button {
          id: enableButton

          checkable: true
          checked: isEnabled
          text: name
          width: 150

          onCheckedChanged: isEnabled = checked
        }

        Text {
          id: bitboardText

          anchors.top: enableButton.bottom
          color: "white"
          font.bold: true
          font.pointSize: !bitboards.showAsBin ? 12 : 120
          fontSizeMode: !bitboards.showAsBin ? Text.FixedSize : Text.HorizontalFit
          height: contentHeight
          minimumPointSize: 1
          text: bits
          width: parent.width
        }
      }
    }
  }
}