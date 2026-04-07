#ifndef GRIDSTATE_H
#define GRIDSTATE_H

#include <QObject>
#include "types.h"
#include <QVariantList>
#include <vector>
#include <set>

class GridState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList cells READ cells NOTIFY cellsChanged)

public:
    explicit GridState(QObject *parent = nullptr);
    QVariantList cells() const;

    int getGridRows() const;
    int getGridColumns() const;

    /**
     * Initializes the grid state from a parsed map and computed paths.
     * Stores the grid, start/target positions and all unit paths, then builds the initial cell list.
     * Resets the current step to 0 when a new map is loaded.
    **/
    void buildCellList(const ParseResult& result, const std::vector<std::vector<std::pair<int,int>>>& allPaths);

    /**
     * Function to advance all units to their target positions by one step using their set paths.
     * At each step change it emits cells changed to update the grid.
     * Units that have already reached their target position remain at their final position.
    **/
    void advance();

    /**
     * Function to iterate back to the start position
    **/
    void retreat();

signals:
    void cellsChanged();

private:
    int m_rows = 0;
    int m_columns = 0;
    std::vector<std::vector<std::pair<int,int>>> m_allPaths;
    std::vector<std::vector<int>> m_grid;
    std::set<std::pair<int,int>> m_startSet;
    std::set<std::pair<int,int>> m_targetSet;
    int m_currentStep = 0;
    QVariantList m_cells;

    void overlayUnits();
    void buildBaseGrid();
};

#endif // GRIDSTATE_H
