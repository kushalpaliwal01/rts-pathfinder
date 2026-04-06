#include "gamecontroller.h"
#include "iconset.h"
#include <set>
#include <tuple>

GameController::GameController(QObject *parent) : QObject(parent) {}

QVariantList GameController::cells() const {
    return m_cells;
}

void GameController::loadMap(const QString& filePath) {
    JsonParser parser;
    ParseResult result = parser.parseJson(filePath.toStdString());
    PathFinder finder;

    std::vector<std::vector<std::pair<int,int>>> allPaths = finder.findAllPaths(result, IconSet::ELEVATED);
    buildCellList(result, allPaths);
    emit cellsChanged();
}

void GameController::buildCellList(const ParseResult& result, const std::vector<std::vector<std::pair<int,int>>>& allPaths) {
    m_cells.clear();

    std::map<std::pair<int,int>, int> pathMap;
    for (int p = 0; p < allPaths.size(); p++) {
        int colorCode = (p == 0) ? 2 : 5;  // first path = blue, second = yellow
        for (const auto& cell : allPaths[p]) {
            pathMap[cell] = colorCode;
        }
    }

    std::set<std::pair<int,int>> startSet(result.startPositions.begin(), result.startPositions.end());
    std::set<std::pair<int,int>> targetSet(result.targetPositions.begin(), result.targetPositions.end());

    for (int i = 0; i < result.grid.size(); i++) {
        for (int j = 0; j < result.grid[i].size(); j++) {
            int val = result.grid[i][j];
            auto pos = std::make_pair(i, j);
            if (startSet.count(pos)) m_cells.append(3);
            else if (targetSet.count(pos)) m_cells.append(4);
            else if (pathMap.count(pos)) m_cells.append(pathMap[pos]);
            else if (val == IconSet::ELEVATED) m_cells.append(1);
            else m_cells.append(0);
        }
    }
}
