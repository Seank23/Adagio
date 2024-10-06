import QtQuick 2.15
import QtQuick.Controls

Frame {
    id: bottomBar

    Connections {
        target: uiController
        function onLoading(message) {
            lblInfo.text = message;
            indLoading.running = true;
        }
        function onLoaded(message) {
            lblInfo.text = message;
            indLoading.running = false;
            timer.setTimeout(() => lblInfo.text = '', 2000);
        }
    }

    BusyIndicator {
        id: indLoading
        anchors {
            right: parent.right
            rightMargin: 20
            verticalCenter: parent.verticalCenter
        }
        running: false
    }

    Label {
        id: lblInfo
        anchors {
            right: indLoading.left
            rightMargin: 20
            verticalCenter: parent.verticalCenter
        }
        text: ""
    }
}
