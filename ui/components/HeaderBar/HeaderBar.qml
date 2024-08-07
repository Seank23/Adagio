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
    color: "black"

    Button {
        id: btnOpenFile
        text: "Open"
        anchors {
            left: parent.left
            leftMargin: 50
            verticalCenter: parent.verticalCenter
        }
        width: 100
        onClicked: fileDialog.open()
    }

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: uiController.setOpenedFile(selectedFile)
    }
}
