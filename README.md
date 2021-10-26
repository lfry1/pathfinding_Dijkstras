
 ++++++++++++++++++++++++++++++++   PROJECT 4 - PATH FINDING   ++++++++++++++++++++++++++++++++
                                Louise M. Fry & Mikaela Atkinson
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

This project implements a Dijkstra's Algorithm to perform rudimentary path finding on a 2D tile-based map.

This project is a solution to Project 4 for CS302 at Univerity of TN Knoxville, taught by Dr. Scott Emrich
    (Assignment here: https://web.eecs.utk.edu/~semrich/cs302-21/assignments/proj04.html )
    
Last updated 10/26/2021 at 1:43 PM

 Input from stdin must have the following form:
    TILES_N
    TILE_NAME_0	TILE_COST_0
    ...
    TILE_NAME_N-1	TILE_COST_N-1

    MAP_ROWS MAP_COLUMNS
    TILE_0_0    ...
    ...

    TRAVELER_START_ROW TRAVELER_START_COL
    TRAVELER_END_ROW   TRAVELER_END_COL

 Output has form:
    COST
    START_ROW START_COL
    ROW_1 COL_1
    ...
    END_ROW END_COL

IMPLEMENTATION DETAILS:
The 2D tile-based map/graph is represented as a 2d vector inside of a custom Graph struct. 
Each vertex within the graph is stored in a custom Node struct. 
The weight of each vertex is its cost. Cost is represented by characters in the input tile map – 
this char is stored as “char type” in the Node and its weight is stored as “int cost”. 

Each Node n has a “vector<Node *> neighbors” member, which is an adjacency list of n’s valid neighbors. 
This implementation uses a “multimap<int, Node *> unvisited” (with the int being the distance from starting node) 
instead of a priority queue as the “unvisited” container.  
Priority queue would likely have been faster, because it uses a heap. 

BUGS/NOTES: 
This implementation is crashing for N > 500 and therefore benchmarking only goes to 200.
This doesn’t scale too well, due in part to the use of the multimap rather than the priority queue.

BENCHMARKING:
-----------------------------------------------------
| N             | Elapsed Time (s) | Memory Usage   |
|---------------|------------------|----------------|
| 10            | 0.00             | 3315           |
| 20            | 0.00             | 3390           |
| 50            | 0.00             | 3594           |
| 100           | 0.02             | 4427           |
| 200           | 0.08             | 7726           |
| 500           | N/A              | N/A            |
| 1000          | N/A              | N/A            |
|---------------|------------------|----------------|

