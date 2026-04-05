#ifndef GRIDSTATE_H
#define GRIDSTATE_H

#include <QObject>
#include "types.h"

class GridState : public QObject
{
    Q_OBJECT
public:
    explicit GridState(QObject *parent = nullptr);
};

#endif // GRIDSTATE_H
