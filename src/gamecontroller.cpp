#include "gamecontroller.h"
#include "iconset.h"

GameController::GameController(QObject *parent) : QObject(parent) {}

QVariantList GameController::cells() const {
    return m_cells;
}

void GameController::loadMap(const QString& filePath) {
    JsonParser parser;
    ParseResult result = parser.parseJson(filePath.toStdString());
    PathFinder finder;
    std::vector<std::pair<int,int>> path = finder.findPath(result, IconSet::ELEVATED);
    buildCellList(result, path);
    emit cellsChanged();
}

void GameController::buildCellList(const ParseResult& result, const std::vector<std::pair<int,int>>& path) {
    m_cells.clear();
    std::set<std::pair<int,int>> pathSet(path.begin(), path.end());
    for (int i = 0; i < result.grid.size(); i++) {
        for (int j = 0; j < result.grid[i].size(); j++) {
            int val = result.grid[i][j];
            if (std::make_pair(i,j) == result.startPosition) m_cells.append(3);
            else if (std::make_pair(i,j) == result.targetPosition) m_cells.append(4);
            else if (pathSet.count({i,j})) m_cells.append(2);
            else if (val == IconSet::ELEVATED) m_cells.append(1);
            else m_cells.append(0);
        }
    }
}