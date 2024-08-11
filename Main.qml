import QtQuick

import "ui/components/HeaderBar"
import "ui/components/BottomBar"
import "ui/components/WaveformChart"

Window {
    id: root
    width: 1920
    height: 1080
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

    WaveformChart {
        id: waveformChart
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
