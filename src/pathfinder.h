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
     *  Iterates over the start to target assignments provided by the assignTargets function and finds the path
     *  for each of those mapping.
    **/
    std::vector<std::vector<std::pair<int, int>>> findAllPaths(const ParseResult& gridMap, int elevatedValue);

private:

    std::pair<int, int> findEmptyAdjacentCell(
        std::pair<int, int> targetPosition,
        const std::vector<std::vector<int>>& grid,
        int elevatedValue,
        const std::map<std::pair<int, int>, std::pair<int, int>>& alreadyAssigned
    );

    /**
     * Function to map all the starting positions to a target position on the map.
    **/
    std::map<std::pair<int, int>, std::pair<int, int>> assignTargets(
        const ParseResult& gridMap,
        const std::vector<std::pair<int, int>>& startPositions,
        const std::vector<std::pair<int, int>>& targetPositions,
        int elevatedValue
    );

    /**
     * Function to calculate the nearest Target from a list of target positions from the starting position
    **/
    std::pair<int, int> findNearestTarget(
        std::pair<int, int> startPosition,
        const std::vector<std::pair<int, int>>& targetPositions
    );

    /**
     *  Function to compute the most optimal path from the start position to the target position
     *  Uses A* algorithm with Manhatten distance heuristic. It returns an ordered list of positions
     *  from the start to the target or an empty vector if no path exists.
    **/
    std::vector<std::pair<int, int>> findPath(
        const ParseResult& grid,
        int elevatedValue,
        std::set<std::tuple<int, int, int> > &reservationTable,
        std::pair<int, int> startPosition,
        std::pair<int, int> targetPosition
    );

    /**
     *  Function to iterate over the parent map which contains the positions a start position would neec to traverse to reach the
     *  target position. This function is called for all the paths found for start -> target positions.
    **/
    std::vector<std::pair<int, int>> backtrackParent(const std::map<std::pair<int, int>, std::pair<int, int>>& parent, std::pair<int, int> currentNodePos);

    /**
     *  Helper function to calculare the manhatten distance between two positions
    **/
    int calculateManhattenDistance(std::pair<int, int> currentPos, std::pair<int, int> targetPos);

};

#endif // PATHFINDER_H
