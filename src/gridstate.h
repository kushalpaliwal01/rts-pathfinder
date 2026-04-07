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
    Q_PROPERTY(int gridColumns READ getGridColumns NOTIFY cellsChanged)
    Q_PROPERTY(int gridRows READ getGridRows NOTIFY cellsChanged)

public:
    explicit GridState(QObject *parent = nullptr);
    QVariantList cells() const;
    int getGridRows() const;
    int getGridColumns() const;
    void buildCellList(const ParseResult& result, const std::vector<std::vector<std::pair<int,int>>>& allPaths);
    void advance();

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

    void buildBaseGrid();
};

#endif // GRIDSTATE_H
