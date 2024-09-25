#ifndef MAINUICONTROLLER_H
#define MAINUICONTROLLER_H

#include "app/Core/Application.h"
#include "app/API/Utils.h"

#include <QObject>
#include <QtQml>
#include <QPointF>
#include <QXYSeries>
#include <QTimer>

QT_FORWARD_DECLARE_CLASS(QAbstractSeries)

class MainUIController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString openedFile READ openedFile WRITE setOpenedFile NOTIFY openedFileChanged FINAL)
public:
    explicit MainUIController(Adagio::Application* app, QObject* parent = nullptr);

    QString openedFile() const { return m_openedFile; }
    Q_INVOKABLE const float getSampleRate() const { return m_SampleRate; }
    Q_INVOKABLE const float getWaveformSize() const { return m_WaveformDataArray.size(); }

    Q_INVOKABLE void initialiseWaveformSeries(QAbstractSeries* seriesMax, QAbstractSeries* seriesMin);
    Q_INVOKABLE void updateWaveformScale(int componentWidth, float boundingMin, float boundingMax);
    Q_INVOKABLE void updateWaveformPosition(float boundingChange);

    Q_INVOKABLE void playAudio();
    Q_INVOKABLE void pauseAudio();
    Q_INVOKABLE void stopAudio();
    Q_INVOKABLE QString getAudioState() { return m_AudioState; }
    void setAudioState(Adagio::PlayState playState);

    Q_INVOKABLE float getPlaybackPosition() { return m_AdagioApp->GetAudioCurrentSample() / m_AdagioApp->GetAudioSampleCount(); }
    Q_INVOKABLE void setPlaybackPosition(float newPos);


public slots:
    void setOpenedFile(const QString &newOpenedFile);
    void onOpenedFileChanged();

signals:
    void openedFileChanged();
    void loading(QString statusMessage);
    void loaded(QString statusMessage);
    void audioLoaded(int sampleCount, float sampleRate);
    void audioCleared(int success);
    void waveformUpdated(float boundingMin, float boundingMax);
    void playbackPositionUpdate(float position);

private:
    Adagio::Application* m_AdagioApp;
    QString m_openedFile;
    std::vector<float> m_WaveformDataArray;
    float m_SampleRate;
    QString m_AudioState;

    QXYSeries* m_SeriesMax;
    QXYSeries* m_SeriesMin;
    int m_BlockCount, m_MinSample, m_MaxSample;

    int m_PlaybackTimerId;

protected:
    void timerEvent(QTimerEvent *event);
};

#endif // MAINUICONTROLLER_H
