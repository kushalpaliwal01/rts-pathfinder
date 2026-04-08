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

    // Overlay the units on the starting position
    for (const auto& path : m_allPaths) {
        if (path.empty()) continue;
        auto pos = path[0];
        m_cells[pos.first * m_columns + pos.second] = 3;
    }

    emit cellsChanged();
}

void GridState::advance() {
    int maxSteps = 0;
    for (const auto& path: m_allPaths) {
        maxSteps = std::max(maxSteps, (int)path.size() - 1);
    }

    // return if all battle units have reached their target positions
    if (m_currentStep >= maxSteps) return;

    m_currentStep++;

    /**
     *  Rebuild the base grid to reset unit positions from the previous step.
     *  Comment this out to show the full path trail instead of step-by-step movement.
    **/
    // buildBaseGrid();
    overlayUnits();
    emit cellsChanged();
}

void GridState::retreat() {
    if (m_currentStep <= 0) return;
    m_currentStep--;
    buildBaseGrid();

    overlayUnits();
    emit cellsChanged();


}

/**
* Overlay the current position of the units at the current step on the map.
* If one of the units has reached its target position the step is clamped to its final step
* and not incremented to the next step since it has already reached the target position
**/
void GridState::overlayUnits() {
    for (int p = 0; p < (int)m_allPaths.size(); p++) {
        if (m_allPaths[p].empty()) continue;
        int step = std::min(m_currentStep, (int)m_allPaths[p].size() - 1);
        auto pos = m_allPaths[p][step];
        int index = pos.first * m_columns + pos.second;     // Convert 2d position to flat list index
        m_cells[index] = 3;
    }
}
/**
 * Rebuilds the m_cells from scratch using the static map data -> grid, start positions, target positions
 * Called when loading the map to show the initial state and then in advance to get a cleaner state by
 * erasing previous steps markers.
**/
void GridState::buildBaseGrid() {
    m_cells.clear();
    for (int i = 0; i < m_grid.size(); i++) {
        for (int j = 0; j < m_grid[i].size(); j++) {
            int val = m_grid[i][j];
            auto pos = std::make_pair(i, j);
            if (m_startSet.count(pos)) m_cells.append(IconSet::OPEN);
            else if (m_targetSet.count(pos)) m_cells.append(IconSet::TARGET_DISPLAY);
            else if (val == IconSet::ELEVATED) m_cells.append(IconSet::ELEVATED_DISPLAY);
            else m_cells.append(IconSet::OPEN);
        }
    }
}