#include "app/adagio_pch.h"
#include "MainUIController.h"

#include <QFileInfo>
#include <QtConcurrent>
#include <QXYSeries>

MainUIController::MainUIController(Adagio::Application* app, QObject* parent)
    : QObject{parent}, m_AdagioApp(app), m_openedFile(""), m_SampleRate(0.0f)
{
    QObject::connect(this, &MainUIController::openedFileChanged, this, &MainUIController::onOpenedFileChanged);
}

void MainUIController::updateWaveform(QAbstractSeries *series)
{
    if (series) {
        auto xySeries = static_cast<QXYSeries *>(series);
        xySeries->replace(m_WaveformDataArray);
        emit waveformUpdated(0, m_WaveformDataArray.size());
    }
}

void MainUIController::setOpenedFile(const QString &newOpenedFile)
{
    if (m_openedFile == newOpenedFile)
        return;
    m_openedFile = newOpenedFile == "" ? "" : newOpenedFile.last(newOpenedFile.length() - 8);
    emit openedFileChanged();
}

void MainUIController::onOpenedFileChanged()
{
    QFuture<void> status = QtConcurrent::run([=]
    {
        if (m_openedFile.toStdString() == "")
        {
            // Close audio file
            int status = m_AdagioApp->ClearAudio();
            m_WaveformDataArray.clear();
            m_openedFile = "";
            m_SampleRate = 0.0f;
            emit audioCleared(status);
        }
        else
        {
            // Open audio file
            int status = m_AdagioApp->LoadAudio(m_openedFile.toStdString());
            emit audioLoading("Constructing waveform...");
            std::vector<float> waveformDataVec = m_AdagioApp->ConstructWaveformData();

            m_SampleRate = m_AdagioApp->GetPlaybackSampleRate();
            for (int i = 0; i < waveformDataVec.size(); i++)
            {
                QPointF point(i, waveformDataVec[i]);
                m_WaveformDataArray.append(point);
            }

            emit audioLoaded(status);
        }
    });
    if (m_openedFile.toStdString() != "")
        emit audioLoading("Loading audio...");
}
