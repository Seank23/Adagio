import QtQuick 2.15
import QtCharts

ChartView {
    id: cvAudioViewer
    anchors {
        top: headerBar.bottom
        left: parent.left
        right: parent.right
        topMargin: 20
        leftMargin: 20
        rightMargin: 20
    }
    height: 300
    visible: false
    legend.visible: false

    Connections {
        target: uiController
        function onAudioLoaded(success) {
            uiController.updateWaveform(cvAudioViewer.series(0));
            cvAudioViewer.visible = true;
        }
        function onAudioCleared(success) {
            cvAudioViewer.visible = false;
        }
        function onWaveformUpdated(minSample, maxSample) {
            axisX.min = minSample;
            axisX.max = maxSample;
        }
    }

    property bool openGL: openGLSupported
    animationOptions: ChartView.NoAnimation

    onOpenGLChanged: {
        if (openGLSupported) {
            var series1 = series("audio");
            if (series1)
                series1.useOpenGL = openGL;
        }
    }

    ValueAxis {
        id: axisY
        min: -1
        max: 1
        visible: false
    }

    ValueAxis {
        id: axisX
        gridVisible: false
    }

    LineSeries {
        id: audioData
        name: "audio"
        axisX: axisX
        axisY: axisY
        useOpenGL: cvAudioViewer.openGL
    }
}
