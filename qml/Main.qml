import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 640
    height: 200
    visible: true
    title: qsTr("arinc429-spi")

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10

        Page {
            Layout.fillWidth: true
            header: Label {
                text: qsTr("OP CODE")
                color: "white"
                wrapMode: Label.WordWrap
            }
            TextField {
                anchors.fill: parent
                anchors.margins: 10
            }
        }

        Page {
            Layout.fillWidth: true
            header: Label {
                text: qsTr("DATA")
                color: "white"
                wrapMode: Label.WordWrap
            }
            TextField {
                anchors.fill: parent
                anchors.margins: 10
            }
        }

        Button {
            text: qsTr("WRITE")
        }

        Button {
            text: qsTr("READ")
        }

        Button {
            text: qsTr("RESET")
        }
    }
}
