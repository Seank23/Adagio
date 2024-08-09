import QtQuick

import "ui/components/HeaderBar"
import "ui/components/BottomBar"

Window {
    id: root
    width: 1280
    height: 720
    visible: true
    title: qsTr("Adagio")

    property var infoText
    property var loadingSpinner

    HeaderBar {
        id: headerBar
    }

    BottomBar {
        id: bottomBar
    }

    //property alias infoText: bottomBar.lblInfo.text
    //property alias loadingSpinner: bottomBar.indLoading.running

    Timer {
        id: timer
        function setTimeout(cb, delayTime) {
            timer.interval = delayTime;
            timer.repeat = false;
            timer.triggered.connect(cb);
            timer.triggered.connect(function release () {
                timer.triggered.disconnect(cb); // This is important
                timer.triggered.disconnect(release); // This is important as well
            });
            timer.start();
        }
    }
}
