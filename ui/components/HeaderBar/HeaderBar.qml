import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Dialogs

Rectangle {
    id: headerBarContainer

    Connections {
        target: uiController
        function onAudioLoaded(success) {
            btnOpenFile.text = 'Close' 
        }
        function onAudioCleared(success) {
            btnOpenFile.text = 'Open'
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
