import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    required property QtObject logsCtrlObj

    function getColor(type) {
        switch(type) {
        case 0: // DEBUG
            return Material.color(Material.Grey);
        case 1: // INFO
            return Material.color(Material.Cyan);
        case 2: // OK
            return Material.color(Material.Green);
        case 3: // WARNING
            return Material.color(Material.Orange);
        case 4: // ERROR
            return Material.color(Material.Red);
        default:// UNKNOWN
            return Material.color(Material.Grey);
        }
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

        RowLayout {
            Layout.fillWidth: true

            Label {
                Layout.preferredWidth: 125
                text: qsTr("Time")
            }

            Label {
                Layout.preferredWidth: 75
                text: qsTr("Tag")
            }

            Label {
                text: qsTr("Message")
            }
        }

        ListView {
            id: logList
            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true
            model: logsCtrlObj.model
            highlightMoveDuration: 1
            boundsBehavior: Flickable.StopAtBounds

            ScrollBar.vertical: scrollBar

            delegate: RowLayout {
                width: logList.width

                Label {
                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                    Layout.preferredWidth: 125
                    text: model.time
                    textFormat: TextEdit.RichText
                    font.family: "Monospace"
                    color: getColor(model.type)
                }

                Label {
                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                    Layout.preferredWidth: 75
                    text: model.tag
                    textFormat: TextEdit.RichText
                    font.family: "Monospace"
                    color: getColor(model.type)
                }

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

            Timer {
                id: positionTimer
                interval: 200
                repeat: false
                onTriggered: logList.positionViewAtIndex(logList.count-1, ListView.Visible)
            }

            onCountChanged: {
                positionTimer.start()
            }

            // onCountChanged: {
            //     var newIndex = logList.count - 1 // last index
            //     logList.positionViewAtEnd()
            //     logList.currentIndex = newIndex
            // }
        }
    }
}
