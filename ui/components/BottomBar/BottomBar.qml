import QtQuick 2.15
import QtQuick.Controls

Rectangle {
    id: bottomBar
    anchors {
        left: parent.left
        right: parent.right
        bottom: parent.bottom
    }
    height: 50

    BusyIndicator {
        id: indLoading
        anchors {
            right: parent.right
            rightMargin: 20
            verticalCenter: parent.verticalCenter
        }
        Component.onCompleted: root.loadingSpinner = indLoading
        running: false
    }

    Text {
        id: lblInfo
        anchors {
            right: indLoading.left
            rightMargin: 20
            verticalCenter: parent.verticalCenter
        }
        Component.onCompleted: root.infoText = lblInfo
        text: ""
    }
}
