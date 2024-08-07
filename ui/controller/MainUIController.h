#ifndef MAINUICONTROLLER_H
#define MAINUICONTROLLER_H

#include <QObject>

class MainUIController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString openedFile READ openedFile WRITE setOpenedFile NOTIFY openedFileChanged FINAL)
public:
    explicit MainUIController(QObject *parent = nullptr);

    QString openedFile() const;

public slots:
    void setOpenedFile(const QString &newOpenedFile);

signals:
    void openedFileChanged();

private:
    QString m_openedFile;
};

#endif // MAINUICONTROLLER_H
