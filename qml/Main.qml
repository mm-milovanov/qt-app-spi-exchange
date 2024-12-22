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
            currPageIndex = (flag) ? 1 : 0;
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

                ToolButton {
                    text: qsTr("Connection")
                    highlighted: currPageIndex == 0;
                    onClicked: currPageIndex = 0;
                }

                ToolButton {
                    text: qsTr("SPI control")
                    highlighted: currPageIndex == 1;
                    onClicked: currPageIndex = 1;
                    enabled: connectionPageGUI.connected
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
}
