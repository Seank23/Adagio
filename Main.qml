import QtQuick

import "ui/components/HeaderBar"
import "ui/components/BottomBar"
import "ui/components/WaveformChart"

Window {
    id: root
    width: 1600
    height: 900
    visible: true
    title: qsTr("Adagio")

    HeaderBar {
        id: headerBar
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        height: 100
    }

    BottomBar {
        id: bottomBar
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        height: 50
    }

    WaveformChart {
        id: waveformChart
        anchors {
            top: headerBar.bottom
            left: parent.left
            right: parent.right
            topMargin: 20
            leftMargin: 20
            rightMargin: 20
        }
        height: parent.height / 4
    }

    Timer {
        id: timer
        function setTimeout(cb, delayTime, repeat) {
            timer.interval = delayTime;
            timer.repeat = repeat || false;
            timer.triggered.connect(cb);
            timer.triggered.connect(function release () {
                timer.triggered.disconnect(cb); // This is important
                timer.triggered.disconnect(release); // This is important as well
            });
            timer.start();
        }
    }
}
