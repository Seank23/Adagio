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
