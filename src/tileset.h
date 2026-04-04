#ifndef TILESET_H
#define TILESET_H

class TileSet
{
public:
    TileSet(int elevatedValue, int targetValue);
    int getGroundValue() const;
    int getStartValue() const;
    int getElevatedValue() const;
    int getTargetValue() const;

private:
    const int GROUND = -1;
    const int START = 0;
    const int ELEVATED;
    const int TARGET;
};

#endif // TILESET_H
