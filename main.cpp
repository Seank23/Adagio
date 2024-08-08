#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "app/Core/Application.h"
#include "ui/controller/MainUIController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    auto adagioApp = Adagio::Application();

    MainUIController mainUIController(&adagioApp);

    QQmlContext* context(engine.rootContext());
    context->setContextProperty("uiController", &mainUIController);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Adagio", "Main");
    return app.exec();
}
