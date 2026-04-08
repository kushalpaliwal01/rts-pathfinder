# PathFinder - RTS Battle Unit Pathfinding

A pathfinding algorithm implementation for battle units in a Real-Time Strategy game, built as part of a Globus Medical software candidate assessment.

## Overview
The application accepts a battlefield map in JSON format compatible with RiskyLab Tilemap, parses it into a grid, and computes the optimal path for a battle unit from its starting position to a target position using the A* algorithm with Manhattan distance as the heuristic.

## Technology Stack
- C++17
- QML (Qt 6)
- CMake

## Build Instructions

### Prerequisites
* Qt 6.10 or later
* CMake 3.16 or later

### Steps
* Run git clone https://github.com/kushalpaliwal01/rts-pathfinder.git 
* Open Qt Creator and select Open Project
* Navigate to the cloned repository and open `CMakeLists.txt`
* Select a Qt 6.10+ kit
* Click the run button

### Running the Application
1. Click **Load Map** and select a `.json` file from the `testFiles/` directory
2. Use **Next Step** or **Previous Step** to step through unit movement manually
3. Use **Run Forward** or **Run Back** to animate automatically
4. Click **Stop** to pause. 


## Project Structure

The project is split into four backend components and a QML frontend.

**JsonParser** reads the RiskyLab JSON file and returns a `ParseResult` containing 
the grid, start positions, and target positions.

**PathFinder** takes the `ParseResult` and runs Cooperative A* to compute 
collision-free paths for all units. It returns a list of paths — one per unit — 
where each path is an ordered list of (row, col) positions from start to target.

**GridState** owns the cell data as a flat `QVariantList` and uses the pre-computed 
paths to render the grid step by step. On each step it rebuilds the base grid 
(terrain, targets) and overlays unit positions on top.

**GameController** is the bridge between the C++ backend and the QML frontend. 
It exposes the cell list, grid dimensions, and actions (loadMap, nextStep, 
previousStep) to QML via `Q_PROPERTY` and `Q_INVOKABLE`. It also forwards 
GridState's `cellsChanged` signal to QML to trigger re-renders.

**Main.qml** binds to GameController's properties and reacts to its signals. 
The grid is a `Repeater` over the cell list — each cell renders a color and 
optionally a unit or target based on its cell code.


## Design Decisions

### Architecture

The initial plan had three components: `JsonParser`, `PathFinder`, `GridState`. However in this plan 
`GridState` usecase was to act as a QML bridge and also as a data store and I realised this would break the 
Single Responsibility Principle so I introduced another layer called `GameController` which would act as the bridge between
the QML and backend components. The other components were already handling a single task for example: `JsonParser` was 
being used to iterate the Json file received to extract the grid layout and the starting and target positions. `PathFinder`
was only tasked with finding the optimal paths for all the start positions. 

I also had another component called TileSet which was originally designed as a factory pattern because the doc mentioned that
the tile values could vary, however when i checked the values between different icon sets for the same icon the value varied by 
point values which was already being handled since i was flooring the tile values. So I replaced it with a simple constant class.

### Algorithm

The first three algorithms that came to my mind when I saw the assessment was a path finding algorithm was DFS, BFS, A*. 
After I was done reading the algorithm I knew the grid was uniformally weighted and I would have to find the shortest or the optimal 
path so DFS was crossed out since it just returns a path not always the shortest one. Between BFS and A* the decision came down to
the more efficient one which was a clear choice since BFS explores every option whereas A* uses a heuristic which uses the greedy approach
of exploring paths that seem closest to the target first. For the heurisitc I decided on Manhatten distance since the doc mentioned 
the movement can only be up, down, left or right and not diaognally. The other heurisitc i looked at was Euclidean but again it calculates a straight
line distance between the two points and is used when movement is free in any direction, so in this case it could have overestimated the shorted path.
Since A* explores nodes in order of estimated cost, the actual path is not known until the target is reached. To reconstruct it, a parent map was used
each cell stores the cell it was reached from. Once the target is found, the path is recovered by backtracking through this map from target to start 
and reversing the result.


The algorithm for the a single unit and single target position was straightforward but with multi unit there were several challenges and decisons
I had to think about. First was the assignment of starting positions to which target positions. I decided on using the Manhatten distance to find the 
closest target positon to a start position. But then the question was what if there are more start positions and less target positions. For this I decided
that first the start position one by one will be assigned to any unassigned targets. Once all the target positions were assigned the remaining start positions then
decided on the target position based on the closest target from all the assgined target positions. 

Next decision that I had to make was what would happen if two start positions are to be assigned to the same target position who would acquire it since the doc 
mentioned only one unit can acquire any tile at that moment. For this I decided the start position to reach the target position will acquire it and the next unit
will be assigned to one of the four tiles adjacent to the target position. Now this led to an edge case where the four tiles adjacent to the target are also reserved
in which case I return an empty path and log a warning. 

Another decision I had to think about was how would the units moving simultaneously avoid collisions. For this my approach was straightforward. Since the paths
will be computed sequentially, I added timestep to the tile position that a unit acquires. So the movement of a unit to a position contains three things `(row, column, timestep)`.
So if we consider we have two units -> we first calculate the path of the first unit and we add its path to a `reservationTable` with the values (row, column, timestep). Now when
we move onto computing the path for the second unit we have a check that if the row and column that the second unit is trying to acquire at some `timestep` is already in the 
`reservationTable` we skip that cell. 

This introduced an edge case where if i have two start units and at some `timestep` they are trying to acquire each others position at the next `timestep` it would mean they
are passing over each other which didn't seem right. So I added another check that if the cell a unit is trying to acquire at the next timestep is already reserved I would 
skip that cell as well. 

Finally another edge case that I handled was for unreachable targets in which no path would be found so i just log a warning for it and return an empty path. 

### Grid Rendering
On every step, the grid is to be fully reset to its static state with the terrain mapping and then the units are overlaid on top of that grid on their positions at the cuurent
timestep. I chose this method because it avoided thinking about tracking and erasing the previous positions, the reset automatically would handle it. 

I used two separate signals - `cellsChanged` and `gridChanged` to prevent recalculating the cellsize on every step. `gridChanged` only fires on map load when the grid dimnensions change
and `cellsChanged` fire on every step for re-rednering the units at the current timestep. 

### Limitations
* On testing I realised some things like even though Manhatten distance can be used to calculate the shortest distance and assign the targets to the start position based on it but it 
doesn't considers the reachability of those targets. For example let's say there are two target positins `(At, Bt)` and two start positions `(As, Bs)`. Now manhatten distance could assing
the start position `As` to target position `Bt` and `Bs` to `At` based on which is closer to which. But if for those mappingg there is no path because of the elevated terrain then the algorithm
would do nothing. 
* Another limitation I found was when I have two start positions mapping to the same target position. so I have start positions `s1 and s2` and a target positions `t1`. Now using my algorithm 
both the units will be mapped to the same target. Furthermore, let's say `s1` takes 20 steps to reach the target and `s2` takes 30 steps to reach the same target. Now `s1` would reach the target first
but the target position will only have reserved state till timestep 20. And so `s2` although has a target position adjacent to the target cell it could still at timestep 29 acquire the target position 
although a unit is already present there. I thought of a solution for this which was to have reservations for the start position `s1` for some Maximum timestep which would be around the number of tiles
I have in the map but that would be too much wasted memory. 

## Sample Input/Output
* Sample RiskyLab compatible JSON map files are provided in the `testFiles/` directory.Screenshots showing the output of the pathfinding algorithm on these maps are provided in the `output/` directory.
* Screenshots for outputs show the full path trail visible on the grid. This can be reproduced by commenting out the `buildBaseGrid()` call in `advance()` function in `src/gridstate.cpp`.

## Feedback

#### What aspects of the assessment did you find most helpful?
* I feel some aspects of the assignemnt were open ended which I enjoyed since it made the responsibility of making the desing decision on me.

#### What challenges did you encounter, and how could the assessment be improved?
The integration between a native C++ backend and a declarative QML frontend had a steeper learning curve than anticipated, but I think it was a valuable part of the exercise. 

#### Were there any unclear instructions or elements that could use further explanation?
* I think the one thing I found unclear was the the spec refering to `layers[0].data` as the field containing the tile data. In practice, some sample JSON files had the world layer at index 1 rather 
than index 0. The parser handles this by checking if the first layer is named "world" and falling back to the second layer if not. However, this assumes the world layer is always within the first
two layers. Maps with more complex layer structures where the world layer appears at index 2 or beyond are not supported.
* Another inconsistency I found was that the spec states tile values are integers (0, 8, 3, -1) but the actual sample JSON files contain float values like 0.5 and 
8.1. This required an additional step of flooring all parsed values before comparison to correctly identify tile types.


#### Any additional comments or suggestions?
* More sample JSON files with varied icon sets would be helpful to really see the difference between the values and how one would need to handle it. 
