#include "pathfinder.h"
#include <queue>
#include <set>
#include <algorithm>
#include <climits>
#include <QDebug>

/**
 *  Represents a cell in the A* approach with the cell position and the cost values
 *  gCost: number of steps taken from the start, fCost: gCost + Manhatten distance to target
**/
struct Node{
    std::pair<int, int> cellPosition;
    int gCost;
    int fCost;

    /** Overrides the > operator for Node to enable min heap ordering in priority queue.
     *  It compares two nodes by their fCost values and keeps the node with the lowest
     *  fCost value at the top
    **/
    bool operator>(const Node& other) const {
        return fCost > other.fCost;
    }
};

PathFinder::PathFinder() {}

std::vector<std::vector<std::pair<int, int>>> PathFinder::findAllPaths(const ParseResult& gridMap, int elevatedValue) {

    std::vector<std::vector<std::pair<int, int>>> result;
    std::set<std::tuple<int,int,int>> reservationTable;
    std::map<std::pair<int, int>, std::pair<int, int>> startToTargetMapping = assignTargets(gridMap, gridMap.startPositions, gridMap.targetPositions, elevatedValue);

    for (auto& [start, target] : startToTargetMapping) {
        auto path = findPath(gridMap, elevatedValue, reservationTable, start, target);
        if (!path.empty()) {
            result.push_back(path);
        }
    }

    return result;

}

/**
* Function to map all the starting positions to a target position on the map.
**/
std::map<std::pair<int, int>, std::pair<int, int>> PathFinder::assignTargets(
    const ParseResult& gridMap,
    const std::vector<std::pair<int, int>>& startPositions,
    const std::vector<std::pair<int, int>>& targetPositions,
    int elevatedValue)
{
    std::map<std::pair<int, int>, std::pair<int, int>> result;
    std::vector<std::pair<int, int>> unassignedTargets = targetPositions;

    for (int i=0; i<startPositions.size(); i++) {
        std::pair<int, int> assigned = {-1, -1};
        if(!unassignedTargets.empty()) {
            assigned = findNearestTarget(startPositions[i], unassignedTargets);
            unassignedTargets.erase(
                std::remove(unassignedTargets.begin(), unassignedTargets.end(), assigned),
                unassignedTargets.end());
        }

        if (assigned.first == -1) {
            assigned = findNearestTarget(startPositions[i], targetPositions);
            assigned = findEmptyAdjacentCell(assigned, gridMap.grid, elevatedValue, result);

            if (assigned.first == -1) {
                qWarning() << "No free cell found for unit at start position: " << startPositions[i].first << ", " << startPositions[i].second;
                continue;
            }
        }

        result[startPositions[i]] = assigned;
    }

    return result;

}

/**
 * Function to find the nearest adjacent cell to a target position when that target position is acquired by
 * some other battleunit.
**/
std::pair<int, int> PathFinder::findEmptyAdjacentCell(
    std::pair<int, int> targetPosition,
    const std::vector<std::vector<int>>& grid,
    int elevatedValue,
    const std::map<std::pair<int, int>, std::pair<int, int>>& alreadyAssigned)
{
    const int deltaR[] = {-1,1,0,0};
    const int deltaC[] = {0,0,-1,1};

    for (int i=0; i<4; i++) {
        int row = targetPosition.first + deltaR[i];
        int col = targetPosition.second + deltaC[i];

        if (row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size()) continue;
        if (grid[row][col] == elevatedValue) continue;

        bool taken = false;
        for (auto& [start, dest] : alreadyAssigned) {
            if (dest == std::make_pair(row, col)) {
                taken = true;
                break;
            }
        }

        if (!taken) return {row, col};

    }

    return {-1, -1};
}

/**
* Function to calculate the nearest Target from a list of target positions from the starting position
**/
std::pair<int, int> PathFinder::findNearestTarget(std::pair<int, int> startPosition, const std::vector<std::pair<int, int>>& targetPositions){
    std::pair<int, int> currentMin = {-1, -1};
    int minCost = INT_MAX;
    for (int j=0; j < targetPositions.size(); j++){
        int cost = calculateManhattenDistance(startPosition, targetPositions[j]);
        if (cost < minCost) {
            minCost = cost;
            currentMin = targetPositions[j];
        }
    }

    return currentMin;
}

/**
*  Function to compute the most optimal path from the start position to the target position
*  Uses A* algorithm with Manhatten distance heuristic. It returns an ordered list of positions
*  from the start to the target or an empty vector if no path exists.
**/
std::vector<std::pair<int, int>> PathFinder::findPath (
    const ParseResult& gridMap,
    int elevatedValue,
    std::set<std::tuple<int, int, int>> &reservationTable,
    std::pair<int, int> startPosition,
    std::pair<int, int> targetPosition)
{

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
    std::set<std::pair<int, int>> closedSet;
    std::map<std::pair<int, int>, std::pair<int, int>> parentMap;
    std::vector<std::pair<int, int>> result;

    // To find the up, down, left, right cell corresponding to the current cell
    const int deltaR[] = {-1,1,0,0};
    const int deltaC[] = {0,0,-1,1};

    int rows = gridMap.grid.size() - 1;
    int columns = gridMap.grid[0].size() - 1;

    Node start;
    start.cellPosition = startPosition;
    start.gCost = 0;

    int manhattenDist = calculateManhattenDistance(startPosition, targetPosition);
    start.fCost = start.gCost + manhattenDist;

    openSet.push(start);


    while(!openSet.empty()){
        Node currentNode = openSet.top();
        openSet.pop();

        // gCost is equal to timestep since every step costs exactly 1
        int timestep = currentNode.gCost + 1;

        // Check for duplicate entry
        if (closedSet.count(currentNode.cellPosition)){
            continue;
        }

        if (currentNode.cellPosition == targetPosition){
            result = backtrackParent(parentMap, currentNode.cellPosition);

            for (int i=0; i<result.size(); i++) {
                reservationTable.insert({result[i].first, result[i].second, i});
            }

            return result;
        }

        closedSet.insert(currentNode.cellPosition);



        for (int i=0; i<4; i++){
            int rowVal = currentNode.cellPosition.first + deltaR[i];
            int colVal = currentNode.cellPosition.second + deltaC[i];

            // Check if the cell position is within the grid else skip
            if (rowVal < 0 || rowVal > rows || colVal < 0 || colVal > columns) continue;

            // Check if the cell position on the map is an elevated terrain or not if yes skip
            if (gridMap.grid[rowVal][colVal] == elevatedValue) continue;

            if (closedSet.count({rowVal, colVal})) continue;

            // Check if the cell position is already reserved by another unit at this timestep and if yes skip
            if (reservationTable.count({rowVal, colVal, timestep})) continue;

            // Skip if current cell is reserved at next timestep - prevents units swapping positions
            if (reservationTable.count({currentNode.cellPosition.first, currentNode.cellPosition.second, timestep})) continue;

            // Create the node and add it to the queue
            Node newNode;
            newNode.gCost = currentNode.gCost + 1;
            manhattenDist = calculateManhattenDistance({rowVal, colVal}, targetPosition);
            newNode.fCost = newNode.gCost + manhattenDist;
            newNode.cellPosition = {rowVal, colVal};

            parentMap[std::make_pair(rowVal, colVal)] = currentNode.cellPosition;

            openSet.push(newNode);

        }





    }
    return {};
}


/**
*  Helper function to calculare the manhatten distance between two positions
**/
int PathFinder::calculateManhattenDistance(std::pair<int, int> currentPos, std::pair<int, int> targetPos){
    return abs(currentPos.first - targetPos.first) + abs(currentPos.second - targetPos.second);
}


/**
*  Function to iterate over the parent map which contains the positions a start position would neec to traverse to reach the
*  target position. This function is called for all the paths found for start -> target positions.
**/
std::vector<std::pair<int, int>> PathFinder::backtrackParent(const std::map<std::pair<int, int>, std::pair<int, int>> &parent, std::pair<int, int> currentNodePos){
    std::vector<std::pair<int, int>> returnVal;

    // Loop terminates when the currentNodePos reaches the starting position since it was never added to the map
    while (parent.count(currentNodePos)){
        returnVal.push_back(currentNodePos);
        currentNodePos = parent.at(currentNodePos);
    }
    returnVal.push_back(currentNodePos);  // push start
    std::reverse(returnVal.begin(), returnVal.end());
    return returnVal;

}