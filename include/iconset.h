#ifndef ICONSET_H
#define ICONSET_H

class IconSet
{
public:
    // Raw tile values from JSON
    static constexpr int GROUND = -1;
    static constexpr int START = 0;
    static constexpr int ELEVATED = 3;
    static constexpr int TARGET = 8;

    // Display Code used in QVariant List
    static constexpr int OPEN = 0;
    static constexpr int ELEVATED_DISPLAY = 1;
    static constexpr int PATH = 2;
    static constexpr int UNIT = 3;
    static constexpr int TARGET_DISPLAY = 4;

};

#endif // ICONSET_H
