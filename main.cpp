#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>
#include <QQuickWindow>
#include <QQuickView>

#include "app/Core/Application.h"
#include "ui/controller/MainUIController.h"

int main(int argc, char *argv[])
{
    auto adagioApp = Adagio::Application();
    MainUIController mainUIController(&adagioApp);

    QApplication app(argc, argv);

    bool openGLSupported = QQuickWindow::graphicsApi() == QSGRendererInterface::OpenGLRhi;
    if (!openGLSupported) {
        qWarning() << "OpenGL is not set as the graphics backend, so AbstractSeries.useOpenGL will not work.";
        qWarning() << "Set QSG_RHI_BACKEND=opengl environment variable to force the OpenGL backend to be used.";
    }

    QQmlApplicationEngine engine;

    QQmlContext* context(engine.rootContext());
    context->setContextProperty("uiController", &mainUIController);
    context->setContextProperty("openGLSupported", openGLSupported);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Adagio", "Main");
    return app.exec();
}
