#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "src/jsonparser.h"

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

    // Debugging Code
    JsonParser parser;
    ParseResult result = parser.parseJson("/Users/kushal/Projects/PathFinder/sample.json");
    qDebug() << "Rows:" << result.grid.size();
    qDebug() << "Cols:" << result.grid[0].size();
    qDebug() << "Start:" << result.startPosition.first << result.startPosition.second;
    qDebug() << "Target:" << result.targetPosition.first << result.targetPosition.second;

    engine.loadFromModule("PathFinder", "Main");

    return QCoreApplication::exec();
}
