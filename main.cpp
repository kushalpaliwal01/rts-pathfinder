#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "src/jsonparser.h"
#include "src/pathfinder.h"
#include <src/gamecontroller.h>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    GameController controller;
    engine.rootContext()->setContextProperty("gameController", &controller);
    engine.loadFromModule("PathFinder", "Main");

    return QCoreApplication::exec();
}
