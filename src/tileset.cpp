#include "tileset.h"

TileSet::TileSet(int elevatedValue, int targetValue) : ELEVATED(elevatedValue), TARGET(targetValue) {}

int TileSet::getGroundValue() const {
    return GROUND;
}

int TileSet::getStartValue() const {
    return START;
}

int TileSet::getElevatedValue() const {
    return ELEVATED;
}

int TileSet::getTargetValue() const {
    return TARGET;
}