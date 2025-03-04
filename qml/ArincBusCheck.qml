import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    property QtObject arincBusCheckPageObj: arincBusCheckPageGUI

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20

        Pane {
            implicitWidth: 800
            Layout.alignment: Qt.AlignHCenter
            Material.elevation: 6

            ArincBusCheckControls {
                arincBusCheckPageObj: arincBusCheckPageGUI
                anchors.fill: parent
            }
        }

        Pane {
            implicitWidth: 800
            Layout.minimumHeight: 300
            Layout.maximumHeight: 400
            Layout.alignment: Qt.AlignHCenter
            Material.elevation: 6

            ColumnLayout {
                anchors.fill: parent

                Label {
                    text: qsTr("Logs")
                    Layout.fillWidth: true
                    font.capitalization: Font.AllUppercase
                    font.bold: true
                    wrapMode: Label.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }

                BoardExchangeLogs {
                    Layout.fillWidth: true;
                    Layout.fillHeight: true;
                    logsCtrlObj: arincBusCheckPageGUI.getLogsCtrl();
                }
            }
        }
    }
}
