#include "gridstate.h"
#include "iconset.h"
#include <set>

GridState::GridState (QObject *parent): QObject(parent) {
    m_rows = 32;
    m_columns = 32;
    m_cells = QVariantList(32 * 32, 0);
}

int GridState::getGridRows() const {
    return m_rows;
}

int GridState::getGridColumns() const {
    return m_columns;
}

QVariantList GridState::cells() const {
    return m_cells;
}

void GridState::buildCellList(const ParseResult& result, const std::vector<std::vector<std::pair<int,int>>>& allPaths) {
    m_currentStep = 0;
    m_cells.clear();
    m_rows = result.grid.size();
    m_columns = result.grid[0].size();
    m_grid = result.grid;
    m_allPaths = allPaths;

    m_startSet = std::set<std::pair<int,int>>(result.startPositions.begin(), result.startPositions.end());
    m_targetSet = std::set<std::pair<int,int>>(result.targetPositions.begin(), result.targetPositions.end());

    buildBaseGrid();

    emit cellsChanged();
}

void GridState::advance() {
    int maxSteps = 0;
    for (const auto& path: m_allPaths) {
        maxSteps = std::max(maxSteps, (int)path.size() - 1);
    }

    if (m_currentStep >= maxSteps) return;

    m_currentStep++;
    buildBaseGrid();

    for (int p = 0; p < m_allPaths.size(); p++) {
        if (m_allPaths[p].empty()) continue;
        int step = std::min(m_currentStep, (int)m_allPaths[p].size() - 1);
        auto pos = m_allPaths[p][step];
        int index = pos.first * m_columns + pos.second;
        m_cells[index] = (p == 0) ? 3 : 6;
    }
    emit cellsChanged();
}

void GridState::buildBaseGrid() {
    m_cells.clear();
    for (int i = 0; i < m_grid.size(); i++) {
        for (int j = 0; j < m_grid[i].size(); j++) {
            int val = m_grid[i][j];
            auto pos = std::make_pair(i, j);
            if (m_startSet.count(pos)) m_cells.append(3);
            else if (m_targetSet.count(pos)) m_cells.append(4);
            else if (val == IconSet::ELEVATED) m_cells.append(1);
            else m_cells.append(0);
        }
    }
}