#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <utility>

/**
 * Holds the result of parsing a JSON map file
 * grid: 2D representation of the battlefield with tile values
 * startPosition: (row,col) position of the battle unit on the grid
 * targetPosition: (row,col) position of the target on the grid
**/
struct ParseResult {
    std::vector<std::vector<int>> grid;
    std::pair<int, int> startPosition;
    std::pair<int, int> targetPosition;
};

#endif // TYPES_H
