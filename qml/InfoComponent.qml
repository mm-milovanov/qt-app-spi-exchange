import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout{
    property alias label: componentLabel.text
    spacing: 10

    Label {
        id: componentLabel
        text: label
        color: "white"
        wrapMode: Label.WordWrap
    }
}
