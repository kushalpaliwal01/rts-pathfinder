#include "gamecontroller.h"
#include "iconset.h"
#include <QUrl>

GameController::GameController(QObject *parent) : QObject(parent) {
    // Forward GridState's cellChanged signal to QML via GameControllers own cellsChanged signal
    connect(&m_gridState, &GridState::cellsChanged, this, &GameController::cellsChanged);
}

QVariantList GameController::cells() const {
    return m_gridState.cells();
}

void GameController::loadMap(const QString& filePath) {
    QUrl url(filePath);
    QString localPath = url.toLocalFile();

    JsonParser parser;
    ParseResult result = parser.parseJson(localPath.toStdString());
    if (result.grid.empty()) {
        qWarning() << "Failed to load map: Invalid or Incompatible JSON file";
        return;
    }

    PathFinder finder;
    std::vector<std::vector<std::pair<int,int>>> allPaths = finder.findAllPaths(result, IconSet::ELEVATED);

    m_gridState.buildCellList(result, allPaths);
    emit gridChanged();
}

void GameController::nextStep() {
    m_gridState.advance();
}

void GameController::previousStep() {
    m_gridState.retreat();
}

int GameController::gridColumns() const {
    return m_gridState.getGridColumns();
}

int GameController::gridRows() const {
    return m_gridState.getGridRows();
}

