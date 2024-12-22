import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ColumnLayout {
    required property QtObject cppObject

    property int opCode: 0;
    property string dataStr: "";
    property int dataByteSize: 0;
    property bool isHexDataView: true
    property bool isSetValidOpCode: false
    property bool isReadable: false
    property bool isWritable: false

    function hex2bin(str) {
        let result = "";
        if (str.length !== 0)
            result = parseInt(str, 16).toString(2);
        return result;
    }

    function bin2hex(str) {
        let result = "";
        if (str.length !== 0)
            result = parseInt(str, 2).toString(16);
        return result;
    }

    spacing: 10

    Label {
        Layout.fillWidth: true;
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("SPI controls");
    }

    RowLayout {
        spacing: 10

        TextField {
            id: opCodeField

            Layout.alignment: Qt.AlignBottom

            horizontalAlignment: Qt.AlignHCenter;
            text: "0x" + opCode.toString(16);
            onEditingFinished: {
                opCode = (text.length !== 0) ? parseInt(text, 16) : 0;

                var dataSize = cppObject.getDataSize(opCode);
                if (dataSize === -1) {
                    isSetValidOpCode = false;
                    warningRect.visible = true;
                    return;
                }

                if (dataSize > 8)
                    isHexDataView = true;

                dataByteSize = dataSize;
                warningRect.visible = false;
                var wr = cppObject.getRwPermision(opCode);
                isWritable = ((wr & 0b01) !== 0) ? true : false;
                isReadable = ((wr & 0b10) !== 0) ? true : false;

                isSetValidOpCode = true;
            }

            onTextEdited: {
                isSetValidOpCode = false;
            }

            validator: RegularExpressionValidator {
                regularExpression: /0[xX][0-9a-fA-F]{1,2}/;
            }

            Rectangle {
                radius: 10
                id: warningRect;
                border.color: Material.color(Material.Red);
                border.width: 2
                color: "transparent"
                anchors.fill: parent;
                visible: false
            }
        }

        TextField {
            id: dataField

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom

            enabled: isSetValidOpCode
            readOnly: !isWritable
            maximumLength: (isHexDataView) ? 2*dataByteSize : 8*dataByteSize;
            text: (isHexDataView) ? dataStr : hex2bin(dataStr);

            onEditingFinished: {
                dataStr = (isHexDataView) ? text : bin2hex(text);
            }

            validator: RegularExpressionValidator {
                regularExpression: (isHexDataView) ? /[0-9a-fA-F]+/ : /[0-1]+/;
            }
            wrapMode: TextEdit.WrapAnywhere
        }

        Connections {
            target: cppObject
            function onReceivedData(data) {
                dataStr = data;
            }
        }

        ComboBox {
            Layout.alignment: Qt.AlignBottom

            enabled: isSetValidOpCode && (dataByteSize <= 8);
            currentIndex: isHexDataView
            model: [
                qsTr("BIN"),
                qsTr("HEX")

            ];
            onActivated: {
                switch (currentIndex) {
                case 0:
                    isHexDataView = false;
                    break;
                case 1:
                    isHexDataView = true;
                    break;
                }
            }
        }
    }

    RowLayout {
        spacing: 10

        Button {
            Layout.fillWidth: true
            enabled: isSetValidOpCode && isWritable
            text: qsTr("Write")
            onClicked: {
                var opCode = parseInt(opCodeField.text, 16);
                cppObject.writeRequest(opCode, dataStr);
            }
        }

        Button {
            Layout.fillWidth: true
            enabled: isSetValidOpCode && isReadable
            text: qsTr("Read")
            onClicked: {
                var opCode = parseInt(opCodeField.text, 16);
                cppObject.readRequest(opCode, dataByteSize);
            }
        }

        Button {
            Layout.fillWidth: true
            enabled: isSetValidOpCode
            text: qsTr("Reset")
            onClicked: {
                dataStr = "";
            }
        }
    }
}
