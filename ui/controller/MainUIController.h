#ifndef MAINUICONTROLLER_H
#define MAINUICONTROLLER_H

#include "app/Core/Application.h"

#include <QObject>

class MainUIController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString openedFile READ openedFile WRITE setOpenedFile NOTIFY openedFileChanged FINAL)
public:
    explicit MainUIController(Adagio::Application* app, QObject* parent = nullptr);

    QString openedFile() const;

public slots:
    void setOpenedFile(const QString &newOpenedFile);
    void onOpenedFileChanged();

signals:
    void openedFileChanged();
    void audioLoading();
    void audioLoaded(int success);
    void audioCleared(int success);

private:
    Adagio::Application* m_AdagioApp;
    QString m_openedFile;
};

#endif // MAINUICONTROLLER_H
