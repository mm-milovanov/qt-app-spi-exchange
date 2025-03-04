import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ColumnLayout {
    required property QtObject arincBusCheckPageObj

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

        TextField {
            Layout.alignment: Qt.AlignBottom

            readOnly: true
            placeholderText: qsTr("n");
            text: arincBusCheckPageObj.pkgNum
            font.family: "Monospace"
        }

        TextField {
            Layout.alignment: Qt.AlignBottom

            readOnly: true
            placeholderText: qsTr("SR");
            text: "0x" + arincBusCheckPageObj.SR.toString(16)
            font.family: "Monospace"
        }

        TextField {
            Layout.fillWidth: true

            readOnly: true
            placeholderText: qsTr("Data");
            text: arincBusCheckPageObj.data
            font.family: "Monospace"

            wrapMode: TextEdit.WrapAnywhere
        }
    }

    RowLayout {
        spacing: 10

        Button {
            Layout.fillWidth: true
            text: (arincBusCheckPageObj.status === 0 || arincBusCheckPageObj.status === 3)
                    ? qsTr("Start")
                    : qsTr("Stop");
            onClicked: {
                if (arincBusCheckPageObj.status === 0 || arincBusCheckPageObj.status === 3)
                    arincBusCheckPageObj.start();
                else
                    arincBusCheckPageObj.stop();
            }
        }

        Button {
            Layout.fillWidth: true
            // m_status is ACTIVE or is PAUSE
            enabled: arincBusCheckPageObj.status === 1 || arincBusCheckPageObj.status === 2;
            text: (arincBusCheckPageObj.status === 0 || arincBusCheckPageObj.status === 1)
                  ? qsTr("Pause")
                  : qsTr("Resume");
            onClicked: {
                if (arincBusCheckPageObj.status === 1)
                    arincBusCheckPageObj.pause();
                else if (arincBusCheckPageObj.status === 2)
                    arincBusCheckPageObj.start();
            }
        }
    }
}
