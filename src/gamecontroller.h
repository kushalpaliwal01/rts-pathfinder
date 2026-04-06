#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QVariantList>
#include "jsonparser.h"
#include "pathfinder.h"

class GameController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList cells READ cells NOTIFY cellsChanged)

public:
    explicit GameController(QObject *parent = nullptr);
    QVariantList cells() const;
    void loadMap(const QString& filePath);

signals:
    void cellsChanged();

private:
    QVariantList m_cells;
    void buildCellList(const ParseResult& result, const std::vector<std::vector<std::pair<int,int>>>& allPaths);
};

#endif