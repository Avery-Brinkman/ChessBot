import QtQuick
import QtQuick.Controls.Universal

Column {
  readonly property var bitboards: model.bitboards
  readonly property var settingsPanel: model.settingsPanel

  spacing: 2

  Switch {
    checked: settingsPanel.showBitboards
    text: "Show Bitboards"
    width: parent.width

    onCheckedChanged: settingsPanel.showBitboards = checked
  }

  Item {
    height: 10
    width: parent.width

    Rectangle {
      anchors.centerIn: parent
      color: "black"
      height: 1
      width: parent.width
    }
  }

  Button {
    checkable: true
    text: "White Pawns"
    width: 150
  }

  Text {
    font.bold: true
    font.pointSize: 120
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
    minimumPointSize: 1
    text: bitboards.whitePawns
    width: parent.width
  }

  Button {
    checkable: true
    text: "White Knights"
    width: 150
  }

  Text {
    font.bold: true
    font.pointSize: 120
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
    minimumPointSize: 1
    text: bitboards.whiteKnights
    width: parent.width
  }

  Button {
    checkable: true
    text: "White Bishops"
    width: 150
  }

  Text {
    font.bold: true
    font.pointSize: 120
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
    minimumPointSize: 1
    text: bitboards.whiteBishops
    width: parent.width
  }

  Button {
    checkable: true
    text: "White Rooks"
    width: 150
  }

  Text {
    font.bold: true
    font.pointSize: 120
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
    minimumPointSize: 1
    text: bitboards.whiteRooks
    width: parent.width
  }

  Button {
    checkable: true
    text: "White Queens"
    width: 150
  }

  Text {
    font.bold: true
    font.pointSize: 120
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
    minimumPointSize: 1
    text: bitboards.whiteQueens
    width: parent.width
  }

  Button {
    checkable: true
    text: "White King"
    width: 150
  }

  Text {
    font.bold: true
    font.pointSize: 120
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
    minimumPointSize: 1
    text: bitboards.whiteKing
    width: parent.width
  }

  Button {
    checkable: true
    text: "Black Pawns"
    width: 150
  }

  Text {
    font.bold: true
    font.pointSize: 120
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
    minimumPointSize: 1
    text: bitboards.blackPawns
    width: parent.width
  }

  Button {
    checkable: true
    text: "Black Knights"
    width: 150
  }

  Text {
    font.bold: true
    font.pointSize: 120
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
    minimumPointSize: 1
    text: bitboards.blackKnights
    width: parent.width
  }

  Button {
    checkable: true
    text: "Black Bishops"
    width: 150
  }

  Text {
    font.bold: true
    font.pointSize: 120
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
    minimumPointSize: 1
    text: bitboards.blackBishops
    width: parent.width
  }

  Button {
    checkable: true
    text: "Black Rooks"
    width: 150
  }

  Text {
    font.bold: true
    font.pointSize: 120
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
    minimumPointSize: 1
    text: bitboards.blackRooks
    width: parent.width
  }

  Button {
    checkable: true
    text: "Black Queens"
    width: 150
  }

  Text {
    font.bold: true
    font.pointSize: 120
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
    minimumPointSize: 1
    text: bitboards.blackQueens
    width: parent.width
  }

  Button {
    checkable: true
    text: "Black King"
    width: 150
  }

  Text {
    font.bold: true
    font.pointSize: 120
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
    minimumPointSize: 1
    text: bitboards.blackKing
    width: parent.width
  }

  Button {
    checkable: true
    text: "White En Passant"
    width: 150
  }

  Text {
    font.bold: true
    font.pointSize: 120
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
    minimumPointSize: 1
    text: bitboards.whiteEnPassant
    width: parent.width
  }

  Button {
    checkable: true
    text: "Black En Passant"
    width: 150
  }

  Text {
    font.bold: true
    font.pointSize: 120
    fontSizeMode: Text.HorizontalFit
    height: contentHeight
    minimumPointSize: 1
    text: bitboards.blackEnPassant
    width: parent.width
  }
}
