import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Dialogs

Rectangle {
    id: headerBarContainer

    property bool audioOpen: false

    Connections {
        target: uiController
        function onAudioLoaded(success) {
            btnOpenFile.text = 'Close';
            audioOpen = true;
        }
        function onAudioCleared(success) {
            btnOpenFile.text = 'Open';
            audioOpen = false;
        }
    }

    function onPlayClicked() {
        uiController.getAudioState() === 'PLAYING' ? uiController.pauseAudio() : uiController.playAudio();
        btnPlay.text = uiController.getAudioState() === 'PLAYING' ? 'Pause' : 'Play';
    }

    function onStopClicked() {
        uiController.stopAudio();
        btnPlay.text = 'Play';
    }

    Button {
        id: btnOpenFile
        text: 'Open'
        anchors {
            left: parent.left
            leftMargin: 50
            verticalCenter: parent.verticalCenter
        }
        width: 100
        height: 30
        onClicked: uiController.openedFile === '' ? fileDialog.open() : uiController.setOpenedFile('')
    }

    Rectangle {
        id: playbackButtonContainer
        anchors {
            left: btnOpenFile.right
            leftMargin: 30
            verticalCenter: parent.verticalCenter
        }
        visible: audioOpen

        Button {
            id: btnPlay
            text: 'Play'
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            width: 100
            height: 30
            onClicked: () => onPlayClicked()
        }

        Button {
            id: btnStop
            text: 'Stop'
            anchors {
                left: btnPlay.right
                leftMargin: 30
                verticalCenter: parent.verticalCenter
            }
            width: 100
            height: 30
            onClicked: () => onStopClicked()
        }
    }

    FileDialog {
        id: fileDialog
        onAccepted: uiController.setOpenedFile(selectedFile)
    }
}
