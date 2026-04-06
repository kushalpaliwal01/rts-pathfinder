# Globus Medical Take Home Assessment

## Overview 
Design and Implement a Path Finding Algorithm for battle units in a Real Time Strategy Game. 
The application accepts a battlefield map in JSON format compatible with RiskyLab Tilemap, 
parses it into a grid, and computes the optimal path for a battle unit from its starting position to a target position.

## Design Decisions
**Initial Plan**
* Includes working on the implementation of the backend first(single battle unit then mulitbattle units)and 
then focusing on the intergration with the frontend in QML.
* Three Components: JsonParser, PathFinder, GridState, TileSet
* JsonParser will take in a json file compatible with RiskyLab Tilemap, parse the fields of the file and output
a 2d grid containing the positions of the ground terrain, elevated terrain, battle unit starting position, target
position.
* PathFinder will take in the 2d grid from the JsonParser as an input and find the quickest path from the starting
positon of the battleunit to the target position and return an array containing the steps to reach the said
target position
* GridState will store both the grid and battle unit positions, target positions and will act a single source
of truth for the UI and path finder algorithm. 
* TileSet is the class responsible for mapping tile values to their meaning. It knows that in a given tileset,
value 3 means elevated terrain and value 8 means target position. JsonParser would use this to correctly interpret
the numbers it reads from the JSON. Without it JsonParser would have to harcode the variable values of the tile
based on the icon set being used. 

**Revised Plan**
* I am thinking another component would be required to orchestrate the flow between the QML and the backend called
the GameController. Initially i was thinking of using the GridState for that task but that would violate the Single
Responsibility Principle. So now in the updated plan the QML would trigger actions on the GameController, which will call
the JsonParser store the result in GridState and calls the PathFinder. GridState would remain as a pure data store. 


**Some Considerations and Assumptions**
* The documentation mentions returning the path to the target position from the starting position of the battle unit
but there is no mention of the shortest path or any particular path. For the initial implementation since all the cells
are equally weighted I am thinking the shortest path would be the optimal path. 
* The spec mentions that the tile value for target position and elevated terrain could change depending on the icon set.
So, rather than hardcoding values as enum, I am thinking of introducing a TileSet class where the constant values are 
fixed across tilesets and there are variable values that are configured based on the tileset. The JsonParser would read
the tileset name and look up the correct configuration. If the tileset is unrecognizable we log a warning and fall back to 
default to the values mentioned in the spec. 
* Now for the path finding algorithm DFS is out since it doesn't outputs the shortest path so the choice is between A* and BFS
and the choice is clear since BFS uses uninformed decisions and explores equally in all directions before deciding on the shortest path,
whereas A* uses smarter guessing by priortizing nodes that seem closer to the goal.
* The document doesn't mention if a single path is required or could multiple optimal paths be provided as an option to the user.
A* algorithm would naturally return one path and the tie breaking would work implicitly by the order cells are processed
* An edge case would be if no path is found to the target position in which case the user should be notified
* Multiunit will be addressed after single unit is complete and working. 


** Inconsitencies with the Document ** 
*The spec states tile values are integers (0, 8, 3, -1) but in the sample JSON contains float values (0.5, 8.1). This might be some
rendering offset from the tileset where the decimal represents the sub tile variant. I made the decision to floor the float values to
integers before operating on the values, treating 8.1 as 8 which is the target tile value and 0.5 as -1 for the single battle unit case since it
doesn't match any known tile type after flooring except in the case of multiple battle units starting position. 

## Implementation Update

### Completed
* JsonParser: parses the RiskyLab compatible Json file and builds a 2d grid and detects start and target positions and packages this in a struct
called ParseResult
* PathFinder: A* with Manhatten distance heurisitics, and implemented a lazy detection using closed set and finally a parent map for reconstructing
the path from the starting position to the target position
* GameController: acts as the bridge between the C++ backend and QML frontend. Calls JsonParser to parse the map, passes the result to PathFinder
with the elevated tile value, then flattens the 2D grid into a QVariantList where each cell is assigned a type integer. Exposes this list to the 
QML with a cellschanged signal so QML automatically re-renders when data updates.
* Main.qml: renders the grid driven by the GameController's cells property. Each cell is a Rectangle colored based on its type — white (ground),
black (elevated/wall), blue (path), green (start), red (target).
* PathFinder: Extended the previous version to handle multiple start and target positions. The start positions are mapped to the target position with
the closest manhatten distance. If two start positions map to the same target position the second start position would acquire one of the four cells
adjacent to the target cell. If no such cells exist a warning is sent to the user. The path for each start to target position is generated sequentially
and the exact path is added to the reservation table with the row, col, timestep value. so that if another start position cannot acquire a cell at a certain
timestep when another start position or battle unit has already acquired so it is considered as an unoccupiable cell. 


### Revised Decisions
* TileSet abstraction removed and replaced with IconSet, a simple constant class since the original decision was based up on different icon sets having
different tile values requiring a factory pattern however, examining the sample data showed that the variation was (8.1, 8.2, 8.3) which is already resolved
with flooring
* GridState is not used as a QML bridge anymore and is just used as a pure data store 
* For handling multiple start and target positions I used cooperative A* algorithm which is near optimal. 

### Next Steps
* Multi unit support: I need to extend the ParseResult to hold multiple start and multiple target positions. One thing I need to decide upon is
a way to avoid collisions when the units move simultaneously and try to acquire the same tile. 
* Improved UI: Currenlty the UI just shows the grid with the path and I would like to add a step through buttons that would allow the user to step
to the next step using a button and verify the path. 