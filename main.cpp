#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>
#include <QQuickWindow>
#include <QPalette>
#include <QStyleFactory>

#include "app/Core/Application.h"
#include "ui/controller/MainUIController.h"

int main(int argc, char *argv[])
{
    auto adagioApp = Adagio::Application();
    MainUIController mainUIController(&adagioApp);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QQmlContext* context(engine.rootContext());
    context->setContextProperty("uiController", &mainUIController);

    qmlRegisterSingletonType(QStringLiteral("qrc:/ui/components/Theme/AppStyle.qml"),"AppStyle",1,0,"AppStyle");
    qmlRegisterSingletonType(QUrl(QStringLiteral("qrc:/ui/components/Theme/FontStyle.qml")),"AppStyle",1,0,"FontStyle");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Adagio", "Main");
    return app.exec();
}
