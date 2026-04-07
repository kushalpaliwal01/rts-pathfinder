#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <utility>

/**
 * Holds the result of parsing a JSON map file
 * grid: 2D representation of the battlefield with tile values
 * startPositions: (row,col) position of the battle unit on the grid
 * targetPositions: (row,col) position of the target on the grid
**/
struct ParseResult {
    std::vector<std::vector<int>> grid;
    std::vector<std::pair<int, int>> startPositions;
    std::vector<std::pair<int, int>> targetPositions;
};

#endif // TYPES_H
