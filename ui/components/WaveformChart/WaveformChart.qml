import QtQuick 2.15
import QtCharts

Rectangle {

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

    Connections {
        target: uiController
        function onAudioLoaded(_sampleCount, _sampleRate) {
            sampleCount = _sampleCount;
            sampleRate = _sampleRate;
            uiController.initialiseWaveformSeries(cvAudioViewer.series(0).upperSeries, cvAudioViewer.series(0).lowerSeries);
            uiController.updateWaveformScale(resolution, boundingMin, boundingMax);
            cvAudioViewer.visible = true;
            // durationAxis.visible = true;
        }
        function onAudioCleared(success) {
            cvAudioViewer.visible = false;
            boundingMin = 0.0;
            boundingMax = 1.0;
            // durationAxis.visible = false;
        }
        function onWaveformUpdated(min, max) {
            boundingMin = min;
            boundingMax = max;
            minTime = min * sampleCount / sampleRate;
            maxTime = max * sampleCount / sampleRate;
        }
        function onPlaybackPositionUpdate(position) {
            updateCursorPosition(position);
        }
    }

    function updateCursorPosition(position) {
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
        const minutesStr = minutes < 10 ? `0${minutes}` : minutes.toString();
        const secondsStr = seconds < 10 ? `0${seconds}` : seconds.toString();
        const msStr = (milliseconds.toFixed(3) * 1000).toString();
        return `${minutesStr}:${secondsStr}.${msStr}`;
    }

    function onClick(event) {
        switch (event.button) {
        case Qt.LeftButton:
            const newCursorPos = boundingMin + ((boundingMax - boundingMin) * ((event.x - indicatorMarginLeft) / (cvAudioViewer.width - indicatorMarginLeft - indicatorMarginRight)));
            uiController.setPlaybackPosition(newCursorPos);
            updateCursorPosition(newCursorPos);
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
            updateCursorPosition(cursorPos);
            break;
        };
    }

    function onScroll(event) {
        updateBoundingsForZoom(event);
        updateCursorPosition(cursorPos);
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
            color: '#429ef5'
            upperSeries: LineSeries {}
            lowerSeries: LineSeries {}
            useOpenGL: true
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
            color: '#ffffff'
        }
    }

    // Rectangle {
    //     id: durationAxis
    //     anchors {
    //         top: cvAudioViewer.bottom
    //         left: parent.left
    //         right: parent.right
    //         leftMargin: 40
    //         rightMargin: 40
    //     }
    //     height: 20
    //     color: '#ededed'
    //     visible: false

    //     Text {
    //         anchors.left: parent.left
    //         text: floatToTimeString(minTime)
    //     }

    //     Text {
    //         anchors.left: parent.right
    //         text: floatToTimeString(maxTime)
    //     }
    // }
}
