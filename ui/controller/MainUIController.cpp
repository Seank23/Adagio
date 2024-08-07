#include "app/adagio_pch.h"
#include "MainUIController.h"

MainUIController::MainUIController(QObject *parent)
    : QObject{parent}
{}

QString MainUIController::openedFile() const
{
    return m_openedFile;
}

void MainUIController::setOpenedFile(const QString &newOpenedFile)
{
    if (m_openedFile == newOpenedFile)
        return;
    m_openedFile = newOpenedFile;
    emit openedFileChanged();
}
