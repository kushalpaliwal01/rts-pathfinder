#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QVariantList>
#include "jsonparser.h"
#include "pathfinder.h"
#include "gridstate.h"

class GameController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList cells READ cells NOTIFY cellsChanged)
    Q_PROPERTY(int gridColumns READ gridColumns NOTIFY cellsChanged)
    Q_PROPERTY(int gridRows READ gridRows NOTIFY cellsChanged)
    Q_PROPERTY(int gridColumns READ gridColumns NOTIFY gridChanged)
    Q_PROPERTY(int gridRows READ gridRows NOTIFY gridChanged)


public:
    explicit GameController(QObject *parent = nullptr);
    QVariantList cells() const;
    Q_INVOKABLE void loadMap(const QString& filePath);
    Q_INVOKABLE void nextStep();

    int gridColumns() const;
    int gridRows() const;

signals:
    void cellsChanged();
    void gridChanged();

private:
    GridState m_gridState;

};

#endif