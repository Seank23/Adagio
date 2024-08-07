import QtQuick

import "ui/components/HeaderBar"

Window {
    width: 1280
    height: 720
    visible: true
    title: qsTr("Adagio")

    HeaderBar {
        id: headerBar
    }
}
