#ifndef MAINUICONTROLLER_H
#define MAINUICONTROLLER_H

#include "app/Core/Application.h"

#include <QObject>
#include <QtQml>
#include <QPointF>

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
    Q_INVOKABLE void updateWaveform(QAbstractSeries* series);


public slots:
    void setOpenedFile(const QString &newOpenedFile);
    void onOpenedFileChanged();

signals:
    void openedFileChanged();
    void audioLoading(QString statusMessage);
    void audioLoaded(int success);
    void audioCleared(int success);
    void waveformUpdated(int minSample, int maxSample);

private:
    Adagio::Application* m_AdagioApp;
    QString m_openedFile;
    QList<QPointF> m_WaveformDataArray;
    float m_SampleRate;
};

#endif // MAINUICONTROLLER_H
