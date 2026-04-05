#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <utility>

struct ParseResult {
    std::vector<std::vector<int>> grid;
    std::pair<int, int> startPosition;
    std::pair<int, int> targetPosition;
};

#endif // TYPES_H
