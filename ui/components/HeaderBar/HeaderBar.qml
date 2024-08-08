import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Dialogs

Rectangle {
    id: headerBarContainer
    anchors {
        left: parent.left
        right: parent.right
        top: parent.top
    }
    height: 100

    Connections {
        target: uiController
        function onAudioLoading() {
            lblInfo.text = ''
            indLoading.running = true
        }
        function onAudioLoaded(success) {
            lblInfo.text = success ? "Audio loaded successfully" : "Audio failed to load"
            indLoading.running = false
            btnOpenFile.text = 'Close'
            timer.setTimeout(() => lblInfo.text = '', 5000);
        }
        function onAudioCleared(success) {
            //lblInfo.text = success ? "Audio closed successfully" : "An error occuring when closing the audio file"
            indLoading.running = false
            btnOpenFile.text = 'Open'
            //timer.setTimeout(() => lblInfo.text = '', 5000);
        }
    }

    Button {
        id: btnOpenFile
        text: "Open"
        anchors {
            left: parent.left
            leftMargin: 50
            verticalCenter: parent.verticalCenter
        }
        width: 100
        height: 30
        onClicked: uiController.openedFile === '' ? fileDialog.open() : uiController.setOpenedFile('')
    }

    FileDialog {
        id: fileDialog
        onAccepted: uiController.setOpenedFile(selectedFile)
    }

    BusyIndicator {
        id: indLoading
        anchors {
            left: btnOpenFile.right
            leftMargin: 20
            verticalCenter: parent.verticalCenter
        }
        running: false
    }

    Text {
        id: lblInfo
        anchors {
            left: btnOpenFile.right
            leftMargin: 20
            verticalCenter: parent.verticalCenter
        }
        text: ""
    }
}
