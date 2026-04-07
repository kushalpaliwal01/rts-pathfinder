#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "types.h"
#include <vector>
#include <utility>
#include <set>
#include <tuple>
#include <map>


class PathFinder
{
public:
    PathFinder();

    /**
    *  Find all collision free paths for all start positions to a target position using Cooperative A* with position and timestep reservation
    *  Uses a priority order where the earlier units reserve cells and the later units route around them
    **/
    std::vector<std::vector<std::pair<int, int>>> findAllPaths(const ParseResult& gridMap, int elevatedValue);

private:

    std::pair<int, int> findEmptyAdjacentCell(
        std::pair<int, int> targetPosition,
        const std::vector<std::vector<int>>& grid,
        int elevatedValue,
        const std::map<std::pair<int, int>, std::pair<int, int>>& alreadyAssigned
    );

    std::map<std::pair<int, int>, std::pair<int, int>> assignTargets(
        const ParseResult& gridMap,
        const std::vector<std::pair<int, int>>& startPositions,
        const std::vector<std::pair<int, int>>& targetPositions,
        int elevatedValue
    );

    std::pair<int, int> findNearestTarget(
        std::pair<int, int> startPosition,
        const std::vector<std::pair<int, int>>& targetPositions
    );

    std::vector<std::pair<int, int>> findPath(
        const ParseResult& grid,
        int elevatedValue,
        std::set<std::tuple<int, int, int> > &reservationTable,
        std::pair<int, int> startPosition,
        std::pair<int, int> targetPosition
    );

    std::vector<std::pair<int, int>> backtrackParent(const std::map<std::pair<int, int>, std::pair<int, int>>& parent, std::pair<int, int> currentNodePos);

    int calculateManhattenDistance(std::pair<int, int> currentPos, std::pair<int, int> targetPos);

};

#endif // PATHFINDER_H
