import QtQuick 2.15
import QtQuick.Controls
import QtCharts
import AppStyle

Item {
    property int resolution: cvAudioViewer.width / 3
    property real boundingMin: 0.0
    property real boundingMax: 1.0
    property real mousePosX: 0.0
    property real regionWidth: 1.0

    property int sampleCount: 0
    property real sampleRate: 0.0

    property real minTime: 0.0
    property real maxTime: 0.0

    property int indicatorMarginLeft: 57
    property int indicatorMarginRight: 32

    property real cursorPos: 0.0

    property string audioDurationStr: ''

    Connections {
        target: uiController
        function onAudioLoaded(_sampleCount, _sampleRate) {
            sampleCount = _sampleCount;
            sampleRate = _sampleRate;
            uiController.initialiseWaveformSeries(cvAudioViewer.series(0).upperSeries, cvAudioViewer.series(0).lowerSeries);
            uiController.updateWaveformScale(resolution, boundingMin, boundingMax);
            audioDurationStr = floatToTimeString(sampleCount / sampleRate);
            txtPlaybackTime.text = `0:00:000 / ${audioDurationStr}`;
            txtPlaybackTime.visible = true;
            cvAudioViewer.visible = true;
        }
        function onAudioCleared(success) {
            cvAudioViewer.visible = false;
            txtPlaybackTime.visible = false;
            boundingMin = 0.0;
            boundingMax = 1.0;
            audioDurationStr = ''
        }
        function onWaveformUpdated(min, max) {
            boundingMin = min;
            boundingMax = max;
            minTime = min * sampleCount / sampleRate;
            maxTime = max * sampleCount / sampleRate;
        }
        function onPlaybackPositionUpdate(position, time) {
            updateCursorPosition(position, time);
        }
    }

    function updateCursorPosition(position, time) {
        txtPlaybackTime.text = `${floatToTimeString(time)} / ${audioDurationStr}`
        cursorPos = position;
        const scale = (cvAudioViewer.width - indicatorMarginLeft - indicatorMarginRight) / (boundingMax - boundingMin);
        if (position < boundingMin)
            playbackIndicator.width = 0;
        else if (position > boundingMax)
            playbackIndicator.width = scale;
        else
            playbackIndicator.width = (position - boundingMin) * scale;
    }

    function updateBoundingsForZoom(wheelEvent) {
        const zoomStep = 0.02;
        const zoomFactor = zoomStep * -wheelEvent.angleDelta.y / 120;
        regionWidth = Math.max((boundingMax - boundingMin) + zoomFactor, 0.03);
        const relPosition = wheelEvent.x / resolution;
        const boundingCenter = (boundingMin + boundingMax) / 2
        boundingMin = Math.max(boundingCenter - regionWidth / 2, 0.0);
        boundingMax = Math.min(boundingCenter + regionWidth / 2, 1.0);
        uiController.updateWaveformScale(resolution, boundingMin, boundingMax);
    }

    function updateBoundingsForMove(mouseEvent) {
        const boundingChange = (mouseEvent.x - mousePosX) * 0.01;
        mousePosX = mouseEvent.x;
        if (Math.abs(boundingChange) < 0.02) {
            const scrollAmount = -boundingChange / regionWidth;
            uiController.updateWaveformPosition(scrollAmount);
        }
    }

    function floatToTimeString(timeFloat) {
        const minutes = Math.floor(timeFloat / 60);
        const seconds = Math.floor(timeFloat % 60);
        const milliseconds = timeFloat % 1;
        const minutesStr = minutes.toString();
        const secondsStr = seconds < 10 ? `0${seconds}` : seconds.toString();
        let msStr = (milliseconds.toFixed(3) * 1000).toString();
        if (parseInt(msStr) < 10)
            msStr = `00${msStr}`;
        else if (parseInt(msStr) < 100)
            msStr = `0${msStr}`;
        return `${minutesStr}:${secondsStr}.${msStr}`;
    }

    function onClick(event) {
        switch (event.button) {
        case Qt.LeftButton:
            const newCursorPos = boundingMin + ((boundingMax - boundingMin) * ((event.x - indicatorMarginLeft) / (cvAudioViewer.width - indicatorMarginLeft - indicatorMarginRight)));
            uiController.setPlaybackPosition(newCursorPos);
            updateCursorPosition(newCursorPos, uiController.getPlaybackTime());
            break;
        case Qt.RightButton:
            mousePosX = event.x;
            break;
        };
    }

    function onPositionChange(event) {
        switch (event.buttons) {
        case Qt.LeftButton:
            break;
        case Qt.RightButton:
            updateBoundingsForMove(event);
            updateCursorPosition(cursorPos, uiController.getPlaybackTime());
            break;
        };
    }

    function onScroll(event) {
        updateBoundingsForZoom(event);
        updateCursorPosition(cursorPos, uiController.getPlaybackTime());
    }

    ChartView {
        id: cvAudioViewer
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            leftMargin: -15
            rightMargin: 5
        }
        height: parent.height
        visible: true
        legend.visible: false
        antialiasing: true
        backgroundColor: AppStyle.colorSurface_a20

        ValueAxis {
            id: axisY
            min: -1
            max: 1
            visible: false
        }

        ValueAxis {
            id: axisX
            min: 0
            max: resolution
            visible: false
        }

        AreaSeries {
            id: plot
            axisX: axisX
            axisY: axisY
            color: AppStyle.colorPrimary_a0
            upperSeries: LineSeries {}
            lowerSeries: LineSeries {}
            useOpenGL: true
            borderWidth: 0
        }

        MouseArea {
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            anchors.fill: parent
            onWheel: (event) => onScroll(event)
            onPositionChanged: (event) => onPositionChange(event)
            onClicked: (event) => onClick(event)
        }

        Rectangle {
            id: playbackIndicator
            anchors {
                left: cvAudioViewer.left
                top: cvAudioViewer.top
                bottom: cvAudioViewer.bottom
                leftMargin: indicatorMarginLeft
            }
            width: 0
            opacity: 0.5
            color: AppStyle.dark.window
        }

        Label {
            id: txtPlaybackTime
            visible: false
            anchors {
                left: cvAudioViewer.left
                top: cvAudioViewer.bottom
                leftMargin: indicatorMarginLeft
            }
            font.pixelSize: 20
            text: '0:00:000 / 0:00:000'
        }
    }
}
