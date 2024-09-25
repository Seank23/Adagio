#include "app/adagio_pch.h"
#include "MainUIController.h"

#include <QFileInfo>
#include <QtConcurrent>

MainUIController::MainUIController(Adagio::Application* app, QObject* parent)
    : QObject{parent}, m_AdagioApp(app), m_openedFile(""), m_SampleRate(0.0f)
{
    QObject::connect(this, &MainUIController::openedFileChanged, this, &MainUIController::onOpenedFileChanged);
    setAudioState(m_AdagioApp->GetAudioState());
}

void MainUIController::updateWaveformScale(int componentWidth, float boundingMin, float boundingMax)
{
    if (m_SeriesMax && m_SeriesMin)
    {
        QList<QPointF> newWaveformMax;
        QList<QPointF> newWaveformMin;
        m_MinSample = m_WaveformDataArray.size() * boundingMin;
        m_MaxSample = m_WaveformDataArray.size() * boundingMax;
        m_BlockCount = std::max((int)((m_MaxSample - m_MinSample) / (float)(componentWidth - 1)), 1);
        for (int i = m_MinSample; i < m_MaxSample; i += m_BlockCount)
        {
            float maxY = *std::max_element(&m_WaveformDataArray[i], &m_WaveformDataArray[i] + m_BlockCount);
            newWaveformMax.append(QPointF(newWaveformMax.size(), maxY + 0.01f));
            newWaveformMin.append(QPointF(newWaveformMin.size(), -maxY - 0.01f));
        }
        m_SeriesMax->replace(newWaveformMax);
        m_SeriesMin->replace(newWaveformMin);
        emit waveformUpdated(boundingMin, boundingMax);
    }
}

void MainUIController::updateWaveformPosition(float boundingChange)
{
    if (m_SeriesMax && m_SeriesMin && boundingChange != 0.0f)
    {
        bool isScrollForward = boundingChange > 0.0f;
        int pointsToChange = m_SeriesMax->count() * std::abs(boundingChange);
        int sampleCount = m_WaveformDataArray.size();
        if (isScrollForward && m_MaxSample + pointsToChange * m_BlockCount >= sampleCount || !isScrollForward && m_MinSample - pointsToChange * m_BlockCount < 0)
           return;
        QList<QPointF> newWaveformMax = m_SeriesMax->points();
        QList<QPointF> newWaveformMin = m_SeriesMin->points();
        int removeIndex = !isScrollForward ? newWaveformMax.size() - pointsToChange : 0;
        newWaveformMax.remove(removeIndex, pointsToChange);
        newWaveformMin.remove(removeIndex, pointsToChange);
        int startSampleIndex = isScrollForward ? m_MaxSample : m_MinSample - (pointsToChange * m_BlockCount);
        int endSampleIndex = startSampleIndex + (m_BlockCount * pointsToChange);
        if (isScrollForward)
        {
            for (int i = 0; i < newWaveformMax.count(); i++)
            {
                newWaveformMax[i].setX(newWaveformMax[i].x() - pointsToChange);
                newWaveformMin[i].setX(newWaveformMin[i].x() - pointsToChange);
            }
            int appendX = newWaveformMax.count();
            if (endSampleIndex > sampleCount)
                endSampleIndex = sampleCount;
            for (int i = startSampleIndex; i < endSampleIndex; i += m_BlockCount)
            {
                float maxY = *std::max_element(&m_WaveformDataArray[i], &m_WaveformDataArray[i] + m_BlockCount);
                newWaveformMax.append(QPointF(appendX, maxY + 0.01f));
                newWaveformMin.append(QPointF(appendX, -maxY - 0.01f));
                appendX++;
            }
        }
        else
        {
            for (int i = 0; i < newWaveformMax.count(); i++)
            {
                newWaveformMax[i].setX(newWaveformMax[i].x() + pointsToChange);
                newWaveformMin[i].setX(newWaveformMin[i].x() + pointsToChange);
            }
            int appendX = pointsToChange;
            if (startSampleIndex < 0)
                startSampleIndex = 0;
            for (int i = endSampleIndex; i > startSampleIndex; i -= m_BlockCount)
            {
                float maxY = *std::max_element(&m_WaveformDataArray[i] - m_BlockCount, &m_WaveformDataArray[i]);
                newWaveformMax.push_front(QPointF(appendX, maxY + 0.01f));
                newWaveformMin.push_front(QPointF(appendX, -maxY - 0.01f));
                appendX--;
            }
        }
        m_MinSample += isScrollForward ? pointsToChange * m_BlockCount : -pointsToChange * m_BlockCount;
        m_MaxSample += isScrollForward ? pointsToChange * m_BlockCount : -pointsToChange * m_BlockCount;
        m_SeriesMax->replace(newWaveformMax);
        m_SeriesMin->replace(newWaveformMin);
        emit waveformUpdated(m_MinSample / (float)sampleCount, m_MaxSample / (float)sampleCount);
    }
}

void MainUIController::playAudio()
{
    m_AdagioApp->UpdateAudioState(Adagio::PlayState::PLAYING);
    setAudioState(m_AdagioApp->GetAudioState());
}

void MainUIController::pauseAudio()
{
    m_AdagioApp->UpdateAudioState(Adagio::PlayState::PAUSED);
    setAudioState(m_AdagioApp->GetAudioState());
}

void MainUIController::stopAudio()
{
    m_AdagioApp->UpdateAudioState(Adagio::PlayState::STOPPED);
    setAudioState(m_AdagioApp->GetAudioState());
}

void MainUIController::setAudioState(Adagio::PlayState playState)
{
    switch (playState)
    {
    case Adagio::PlayState::NOT_STARTED:
        m_AudioState = "NOT_STARTED";
        break;
    case Adagio::PlayState::PLAYING:
        m_AudioState = "PLAYING";
        m_PlaybackTimerId = startTimer(1.0f / 60.0f);
        break;
    case Adagio::PlayState::PAUSED:
        m_AudioState = "PAUSED";
        killTimer(m_PlaybackTimerId);
        break;
    case Adagio::PlayState::STOPPED:
        m_AudioState = "STOPPED";
        emit playbackPositionUpdate(0.0f);
        killTimer(m_PlaybackTimerId);
        break;
    }
}

void MainUIController::setPlaybackPosition(float newPos)
{
    int sampleIndex = newPos * m_AdagioApp->GetAudioSampleCount();
    m_AdagioApp->SetPlaybackPosition(sampleIndex);
}

void MainUIController::initialiseWaveformSeries(QAbstractSeries *seriesMax, QAbstractSeries *seriesMin)
{
    m_SeriesMax = static_cast<QXYSeries *>(seriesMax);
    m_SeriesMin = static_cast<QXYSeries *>(seriesMin);
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
            stopAudio();
            int status = m_AdagioApp->ClearAudio();
            m_WaveformDataArray.clear();
            m_openedFile = "";
            m_SampleRate = 0.0f;
            emit audioCleared(status);
            emit loaded(status ? "Audio closed" : "An error occuring while closing the audio file");
        }
        else
        {
            // Open audio file
            int status = m_AdagioApp->LoadAudio(m_openedFile.toStdString());
            emit loading("Constructing waveform...");
            m_WaveformDataArray = m_AdagioApp->ConstructWaveformData();
            m_SampleRate = m_AdagioApp->GetPlaybackSampleRate();
            if (status)
                emit audioLoaded(m_WaveformDataArray.size(), m_SampleRate);
            emit loaded(status ? "Audio loaded successfully" : "Audio failed to load");
        }
    });
    if (m_openedFile.toStdString() != "")
        emit loading("Loading audio...");
}

void MainUIController::timerEvent(QTimerEvent *event)
{
    float playbackPosition = (float)m_AdagioApp->GetAudioCurrentSample() / (float)m_AdagioApp->GetAudioSampleCount();
    emit playbackPositionUpdate(playbackPosition);
}
