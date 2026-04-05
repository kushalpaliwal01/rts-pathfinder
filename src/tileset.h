#ifndef TILESET_H
#define TILESET_H

class TileSet
{
public:
    TileSet();
    int getGroundValue() const;
    int getStartValue() const;

    virtual ~TileSet() = default;
    virtual int getElevatedValue() const = 0;
    virtual int getTargetValue() const = 0;

private:
    const int GROUND = -1;
    const int START = 0;
};

#endif // TILESET_H
