import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import SerialPortModule

Item {
    id: root
    required property QtObject cppObject;

    Pane {
        anchors.centerIn: parent
        Material.elevation: 5
        implicitWidth: 300

        ColumnLayout {
            anchors.centerIn: parent
            width: parent.width

            Label {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Serial device selector")
            }

            SerialPort {
                Layout.fillWidth: true
                cppObject: root.cppObject
            }
        }
    }
}
