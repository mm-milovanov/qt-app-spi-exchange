import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ColumnLayout {
    Label {
        text: qsTr("Controls")
        Layout.fillWidth: true
        font.capitalization: Font.AllUppercase
        font.bold: true
        wrapMode: Label.WordWrap
        horizontalAlignment: Text.AlignHCenter
    }

    RowLayout {
        spacing: 10

        Button {
            Layout.fillWidth: true
            text: qsTr("Start")
            onClicked: {
                var opCode = parseInt(opCodeField.text, 16);
                cppObject.writeRequest(opCode, dataStr);
            }
        }

        Button {
            Layout.fillWidth: true
            text: qsTr("Pause")
            onClicked: {
                var opCode = parseInt(opCodeField.text, 16);
                cppObject.writeRequest(opCode, dataStr);
            }
        }

        TextField {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom

            readOnly: true
            placeholderText: qsTr("Data");
            // text: (isHexDataView) ? dataStr : hex2bin(dataStr);

            wrapMode: TextEdit.WrapAnywhere
        }
    }
}
