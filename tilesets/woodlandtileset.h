#ifndef WOODLANDTILESET_H
#define WOODLANDTILESET_H

#include "tileset.h"

class WoodLandTileSet : public TileSet
{
public:
    WoodLandTileSet();
    int getElevatedValue() const override;
    int getTargetValue() const override;

private:
    const int ELEVATED = 3;
    const int TARGET = 8;
};

#endif // WOODLANDTILESET_H
