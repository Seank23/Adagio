#include "app/adagio_pch.h"
#include "MainUIController.h"

#include <QFileInfo>
#include <QtConcurrent>

MainUIController::MainUIController(Adagio::Application* app, QObject* parent)
    : QObject{parent}, m_AdagioApp(app)
{
    QObject::connect(this, &MainUIController::openedFileChanged, this, &MainUIController::onOpenedFileChanged);
}

QString MainUIController::openedFile() const
{
    return m_openedFile;
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
            int status = m_AdagioApp->ClearAudio();
            emit audioCleared(status);
        }
        else
        {
            int status = m_AdagioApp->LoadAudio(m_openedFile.toStdString());
            emit audioLoaded(status);
        }
    });
    emit audioLoading();
}
