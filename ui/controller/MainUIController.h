#ifndef MAINUICONTROLLER_H
#define MAINUICONTROLLER_H

#include "app/Core/Application.h"

#include <QObject>
#include <QtQml>
#include <QPointF>
#include <QXYSeries>

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


public slots:
    void setOpenedFile(const QString &newOpenedFile);
    void onOpenedFileChanged();

signals:
    void openedFileChanged();
    void loading(QString statusMessage);
    void loaded(QString statusMessage);
    void audioLoaded(int success);
    void audioCleared(int success);
    void waveformUpdated(float boundingMin, float boundingMax);

private:
    Adagio::Application* m_AdagioApp;
    QString m_openedFile;
    std::vector<float> m_WaveformDataArray;
    float m_SampleRate;

    QXYSeries* m_SeriesMax;
    QXYSeries* m_SeriesMin;
    int m_BlockCount, m_MinSample, m_MaxSample;
};

#endif // MAINUICONTROLLER_H
