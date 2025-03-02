import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ColumnLayout {
    required property QtObject cppObject

    Label {
        Layout.fillWidth: true;
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("ARINC connection check");
    }



}
