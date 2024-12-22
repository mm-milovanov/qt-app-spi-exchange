import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    property QtObject spiPageGuiObj: spiPageGUI

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20

        Pane {
            implicitWidth: 800
            Layout.alignment: Qt.AlignHCenter
            Material.elevation: 6

            SpiControls {
                anchors.fill: parent
                cppObject: spiPageGuiObj
            }
        }
    }
}
