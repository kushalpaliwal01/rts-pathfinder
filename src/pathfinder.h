#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "types.h"
#include <vector>
#include <utility>
#include <map>

class PathFinder
{
public:
    PathFinder();

    /**
     *  Function to compute the most optimal path from the start position to the target position
     *  Uses A* algorithm with Manhatten distance heuristic. It returns an ordered list of positions
     *  from the start to the target or an empty vector if no path exists.
    **/
    std::vector<std::pair<int, int>> findPath(ParseResult grid, int elevatedValue);

private:
    int calculateManhattenDistance(std::pair<int, int> currentPos, std::pair<int, int> targetPos);
    std::vector<std::pair<int, int>> backtrackParent(const std::map<std::pair<int, int>, std::pair<int, int>> parent, std::pair<int, int> currentNodePos);
};

#endif // PATHFINDER_H
