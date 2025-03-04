import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import "./extra"
import FastbootModule

Item {
    required property QtObject deviceObj

    function getColor(type) {
        switch(type) {
        case 0: // DEBUG
            return Material.color(Material.Grey);
        case 1: // INFO
            return Material.color(Material.Cyan);
        case 2: // OK
            return Material.color(Material.Gren);
        case 3: // WARNING
            return Material.color(Material.Orange);
        case 4: // ERROR
            return Material.color(Material.Red);
        default:
            return Material.color(Material.Grey);
        }
    }

    ClientLogsCtrl {
        id: clientLogsCtrl
        Component.onCompleted: {
            clientLogsCtrl.setDevice(deviceObj);
        }
    }

    Background {
        color: Material.background
        anchors.fill: parent
    }

    ScrollBar {
        id: scrollBar
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        policy: ScrollBar.AsNeeded
    }

    ColumnLayout {
        anchors.fill: parent;
        anchors.margins: 20

        Label {
            text: qsTr("Логи обмена")
            Layout.fillWidth: true
            font.capitalization: Font.AllUppercase
            font.bold: true
            wrapMode: Label.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }

        RowLayout {
            Layout.fillWidth: true

            Label {
                Layout.preferredWidth: 100
                text: qsTr("Время")
            }

            ToolSeparator {}

            Label {
                Layout.preferredWidth: 100
                text: qsTr("Тип")
            }

            ToolSeparator {}

            Label {
                text: qsTr("Сообщение")
            }
        }

        ListView {
            id: logList
            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true
            model: clientLogsCtrl.model
            highlightMoveDuration: 1
            boundsBehavior: Flickable.StopAtBounds

            ScrollBar.vertical: scrollBar

            delegate: RowLayout {
                width: logList.width

                Label {
                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                    Layout.preferredWidth: 100
                    text: model.time
                    textFormat: TextEdit.RichText
                    font.family: "Monospace"
                    color: getColor(model.type)
                }

                ToolSeparator {}

                Label {
                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                    Layout.preferredWidth: 100
                    text: model.tag
                    textFormat: TextEdit.RichText
                    font.family: "Monospace"
                    color: getColor(model.type)
                }

                ToolSeparator {}

                Label {
                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                    Layout.fillWidth: true;
                    text: model.msg
                    wrapMode: Text.WrapAnywhere
                    textFormat: TextEdit.RichText
                    font.family: "Monospace"
                    color: getColor(model.type)
                }
            }

            onCountChanged: {
                if (true) {
                    var newIndex = count - 1 // last index
                    positionViewAtEnd()
                    currentIndex = newIndex
                }
            }
        }
    }
}
