#include "pathfinder.h"
#include <queue>
#include <set>
#include <iostream>
#include <algorithm>

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

std::vector<std::pair<int, int>> PathFinder::findPath(ParseResult gridMap, int elevatedValue) {
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
    start.cellPosition = gridMap.startPosition;
    start.gCost = 0;

    int manhattenDist = calculateManhattenDistance(gridMap.startPosition, gridMap.targetPosition);
    start.fCost = start.gCost + manhattenDist;

    openSet.push(start);


    while(!openSet.empty()){
        Node currentNode = openSet.top();
        openSet.pop();


        // Check for duplicate entry
        if (closedSet.count(currentNode.cellPosition)){
            continue;
        }

        if (currentNode.cellPosition == gridMap.targetPosition){
            std::cout << "Path Found";
            result = backtrackParent(parentMap, currentNode.cellPosition);
            return result;
        }

        closedSet.insert(currentNode.cellPosition);



        for (int i=0; i<4; i++){
            int rowVal = currentNode.cellPosition.first + deltaR[i];
            int colVal = currentNode.cellPosition.second + deltaC[i];

            // Check if the cell position is within the grid else skip
            if (rowVal < 0 || rowVal > rows || colVal < 0 || colVal > columns){
                continue;
            }

            // Check if the cell position on the map is an elevated terrain or not if yes skip
            if (gridMap.grid[rowVal][colVal] == elevatedValue){
                continue;
            }

            if (closedSet.count({rowVal, colVal})) {
                continue;
            }

            // Create the node and add it to the queue
            Node newNode;
            newNode.gCost = currentNode.gCost + 1;
            manhattenDist = calculateManhattenDistance({rowVal, colVal}, gridMap.targetPosition);
            newNode.fCost = newNode.gCost + manhattenDist;
            newNode.cellPosition = {rowVal, colVal};

            parentMap[std::make_pair(rowVal, colVal)] = currentNode.cellPosition;

            openSet.push(newNode);

        }





    }
    std::cout << "No Path found";
    return {};
}

int PathFinder::calculateManhattenDistance(std::pair<int, int> currentPos, std::pair<int, int> targetPos){
    return abs(currentPos.first - targetPos.first) + abs(currentPos.second - targetPos.second);
}

std::vector<std::pair<int, int>> PathFinder::backtrackParent(const std::map<std::pair<int, int>, std::pair<int, int>> parent, std::pair<int, int> currentNodePos){
    std::vector<std::pair<int, int>> returnVal;

    while (parent.count(currentNodePos)){
        returnVal.push_back(currentNodePos);
        currentNodePos = parent.at(currentNodePos);
    }
    returnVal.push_back(currentNodePos);  // push start
    std::reverse(returnVal.begin(), returnVal.end());
    return returnVal;

}