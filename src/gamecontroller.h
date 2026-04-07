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
    Q_PROPERTY(int gridColumns READ gridColumns NOTIFY gridChanged)
    Q_PROPERTY(int gridRows READ gridRows NOTIFY gridChanged)


public:
    explicit GameController(QObject *parent = nullptr);
    QVariantList cells() const;

    /**
     * Loads a battlefield map from the RiskyLab compatible Json file provided using JsonParser
     * and computes the collision free paths for all starting positions of the battleunits
     * Emits gridChanged signal to update the grid dimensions in the UI
    **/
    Q_INVOKABLE void loadMap(const QString& filePath);

    /**
     * Advances all units one step forward in their pre computed paths and this advancement is calculated
     * in the advance function of the GridState. Emits cellsChanged to trigger the re render of the grid
     * in the UI
    **/
    Q_INVOKABLE void nextStep();
    Q_INVOKABLE void previousStep();

    int gridColumns() const;
    int gridRows() const;

signals:
    void cellsChanged();
    void gridChanged();

private:
    GridState m_gridState;

};

#endif