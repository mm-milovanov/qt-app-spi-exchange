import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

RowLayout {
    required property QtObject cppObject

    property bool isEcho: false

    function getSecondCircleColor() {
        if (!cppObject.connected)
            return Material.color(Material.Red);

        if (!connectionPageGUI.connected)
            return Material.color(Material.Red);

        return Material.color(Material.Green);
    }

    Label {
        Layout.minimumWidth: 100
        text: {
            if (!cppObject.connected)
                return qsTr("Disconnected");

            if (!connectionPageGUI.connected)
                return qsTr("Pending");

            return qsTr("Connected");
        }
    }

    RowLayout {
        spacing: 0

        Rectangle {
            width: 15
            height: width
            color: (cppObject.connected) ?
                    Material.color(Material.Green) :
                    Material.color(Material.Red)
            radius: width*0.5
        }

        Rectangle {
            width: 5
            height: 4
            color: getSecondCircleColor();
        }

        Rectangle {
            id: secondCircle
            width: 15
            height: width
            color: (isEcho) ? Material.color(Material.Yellow) : getSecondCircleColor();
            radius: width*0.5
        }
    }

    Timer {
        id: timer
        interval: 100;
        repeat: false
        onTriggered: {
            isEcho = false;
        }
    }

    Connections {
        target: connectionPageGUI
        function onGetEchoResponse() {
            isEcho = true;
            timer.running = true
        }
    }
}

