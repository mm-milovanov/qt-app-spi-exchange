import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width:   1280
    height:  720
    title:   qsTr("SPI exchange")

    property bool isDarkMode:    true;
    property int  currPageIndex: 0;
    property bool showLogs:      false

    Material.theme:     (isDarkMode) ? Material.Dark   : Material.Light;
    Material.primary:   (isDarkMode) ? Material.Indigo : Material.Blue;

    Connections {
        target: connectionPageGUI
        function onConnectionChanged(flag) {
            if (!flag)
                currPageIndex = 0;
        }
    }

    ColumnLayout {
        anchors.fill: parent;

        ToolBar {
            Layout.fillWidth: true;

            RowLayout {
                anchors.fill: parent;
                anchors.leftMargin: 15
                anchors.rightMargin: 15

                Status {
                    cppObject: serialPortGUI
                }

                ToolSeparator {}

                Repeater {
                    model: [
                        qsTr("Connection"),
                        qsTr("SPI control"),
                        qsTr("ARINC bus check")
                    ]

                    ToolButton {
                        text: modelData
                        highlighted: currPageIndex == index;
                        // enabled: connectionPageGUI.connected
                        onClicked: currPageIndex = index;
                        Rectangle {
                            height: 2
                            width: parent.width
                            anchors.bottom: parent.bottom
                            color: Material.accent
                            visible: currPageIndex == index
                        }
                    }
                }

                ToolSeparator {}

                Item {
                    Layout.fillWidth: true
                }

                ToolButton {
                    text: qsTr("Theme")
                    onClicked: isDarkMode = !isDarkMode;
                }

                ToolSeparator {}

                ColumnLayout {
                    Label {
                        text: NVG_VERSION_STRING
                    }
                    Label {
                        text: NVG_VERSION_BUILD_TIME + ' '
                              + NVG_VERSION_BUILD_DATE
                    }
                }
            }
        }

        Loader {
            Layout.fillHeight: true;
            Layout.fillWidth: true;

            sourceComponent: {
                switch (currPageIndex) {
                case 0: return connectionPage;
                case 1: return spiPage;
                case 2: return arincBusCheck;
                }
            }
        }
    }

    Component {
        id: connectionPage
        ConnectionPage {
            cppObject: serialPortGUI
        }
    }

    Component {
        id: spiPage
        SpiPage { }
    }

    Component {
        id: arincBusCheck
        ArincBusCheck { }
    }
}
