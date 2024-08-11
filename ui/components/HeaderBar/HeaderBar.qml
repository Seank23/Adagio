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
        function onAudioLoading(message) {
            root.infoText.text = message
            root.loadingSpinner.running = true
        }
        function onAudioLoaded(success) {
            root.infoText.text = success ? "Audio loaded successfully" : "Audio failed to load"
            root.loadingSpinner.running = false
            btnOpenFile.text = 'Close'
            timer.setTimeout(() => root.infoText.text = '', 2000);
        }
        function onAudioCleared(success) {
            root.infoText.text = success ? "Audio closed" : "An error occuring while closing the audio file"
            root.loadingSpinner.running = false
            btnOpenFile.text = 'Open'
            timer.setTimeout(() => root.infoText.text = '', 2000);
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
}
