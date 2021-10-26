/*
 ++++++++++++++++++++++++++++++++   DIJKSTRAS.CPP   ++++++++++++++++++++++++++++++++
                                    Louise M. Fry

 This program implements a Dijkstra's Algorithm to perform
   rudimentary path finding on a 2D tile-based map.

 This program is a solution to Project 4 in CS302 at UTK taught by Dr. Scott Emrich
    (Assignment here: https://web.eecs.utk.edu/~semrich/cs302-21/assignments/proj04.html )

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


 Project GitHub here: https://github.com/lfry1/proj4

 */
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <vector>
#include <list>
#include <limits>

using namespace std;

struct Node // aka vertex
{
    int row; // node's position in graph is g.graph[row][col]
    int col;
    char type;
    int cost;
    int distance; // init 999;
    vector<Node *> neighbors;
    int backRow;
    int backCol;
    bool visited; // init false;
};

struct Graph // stores all Nodes/vertices
{
    vector<vector<Node>> graph; // 2d vector, every tile on the map is a node in here
    map<char, int> edgeWeights; // ie g costs 2, m costs 7
    int MAP_ROWS;
    int MAP_COLS;
};

// declaration for utility fn that finds valid neighbors of each node
void assignNeighbors(Graph &g);

// declaration for function used to implement Dijkstra's
void shortestPath(Graph &G, int startRow, int startCol, int endRow, int endCol);

// declaration for print function - prints backedge path
void printMyPath(Graph &G, int startRow, int startCol, int endRow, int endCol);

// declaration for utility fn that prints graph info
void printGraph(Graph &g);

//+++++++++++++++++++++++++ MAIN EXECUTION ++++++++++++++++++++++++
int main(int argc, char *argv[])
{
    int numTileTypes;
    char tileType;
    int tileValue;
    map<char, int> tileWeights;
    int mapRows;
    int mapCols;
    int startRow;
    int startCol;
    int endRow;
    int endCol;

    cin >> numTileTypes;
    for (int i = 0; i < numTileTypes; i++)
    {
        cin >> tileType >> tileValue;
        pair<char, int> tile(tileType, tileValue);
        tileWeights.insert(tile);
    }

    cin >> mapRows >> mapCols;
    Graph g;
    g.MAP_ROWS = mapRows;
    g.MAP_COLS = mapCols;
    g.edgeWeights = tileWeights; // i think

    // add each letter(tile) from the input graph to g.graph as a Node
    for (int i = 0; i < mapRows; i++)
    {
        vector<Node> rowV;
        for (int j = 0; j < mapCols; j++)
        {
            cin >> tileType;
            Node n;
            n.row = i;
            n.col = j;
            n.type = tileType;
            n.cost = tileWeights.find(tileType)->second;
            n.distance = numeric_limits<int>::max();
            n.backRow = -1;
            n.backCol = -1;
            n.visited = false;
            rowV.push_back(n);
        }
        g.graph.push_back(rowV);
    }
    // get start and end positions
    cin >> startRow >> startCol >> endRow >> endCol;
    g.graph[startRow][startCol].distance = 0;

    assignNeighbors(g);                                  // find adjacent nodes of every vertex in graph
    shortestPath(g, startRow, startCol, endRow, endCol); // Dijkstra's
    printMyPath(g, startRow, startCol, endRow, endCol);  // prints shortest path from start to end
    return 0;
} // end main

//+++++++++++++++++++++++++ IMPLEMENTS DIJKSTRAS ++++++++++++++++++++++++
void shortestPath(Graph &g, int startRow, int startCol, int endRow, int endCol)
{
    multimap<int, Node *> unvisited;
    int curRow;
    int curCol;
    int d; // distance

    // insert starting value into unvisited
    unvisited.insert(pair<int, Node *>(0, &g.graph[startRow][startCol]));

    // Dijkstra's - visits all nodes and finds their shortest path distance from start
    while (!unvisited.empty())
    {
        // go to shortest-distance unvisited node n
        curRow = unvisited.begin()->second->row;
        curCol = unvisited.begin()->second->col;
        unvisited.erase(unvisited.begin()); // remove n from unvisited

        //  look thru each of n's neighbors
        for (int i = 0; i < g.graph[curRow][curCol].neighbors.size(); i++)
        {
            // make sure neighbor hasn't already been visited
            if (g.graph[curRow][curCol].neighbors[i]->visited == false)
            {
                d = g.graph[curRow][curCol].distance + g.graph[curRow][curCol].cost;

                // if you found a shorter path for an unvisited neighor, update accordingly
                if (d < g.graph[curRow][curCol].neighbors[i]->distance)
                {
                    g.graph[curRow][curCol].neighbors[i]->distance = d;
                    g.graph[curRow][curCol].neighbors[i]->backRow = curRow;
                    g.graph[curRow][curCol].neighbors[i]->backCol = curCol;
                    // add the updated, unvisted neighbor to unvisited
                    unvisited.insert(pair<int, Node *>(d, g.graph[curRow][curCol].neighbors[i]));
                }
            }
        }
        g.graph[curRow][curCol].visited = true; // so it doesn't get re-examined as a neighbor
    }
    cout << count << endl;
}

//+++++++++++++++++++++++++ MISC UTILITY FNS ++++++++++++++++++++++++

// finds valid adjacenies for each node n in graph and pushes them to n's adj list
void assignNeighbors(Graph &g)
{
    for (int i = 0; i < g.MAP_ROWS; i++)
    {
        for (int j = 0; j < g.MAP_COLS; j++)
        {
            // FIND VALID ADJACENCIES AND PUT THEM IN ADJ LIST ("n.neighbors")
            if (i > 0) // up: i-1
            {
                g.graph[i][j].neighbors.push_back(&g.graph[i - 1][j]);
            }
            if (i < g.MAP_ROWS - 1) // down: i+1
            {
                g.graph[i][j].neighbors.push_back(&g.graph[i + 1][j]);
            }
            if (j > 0) // left: j-1
            {
                g.graph[i][j].neighbors.push_back(&g.graph[i][j - 1]);
            }
            if (j < g.MAP_COLS - 1) // right: j+1
            {
                g.graph[i][j].neighbors.push_back(&g.graph[i][j + 1]);
            }
        }
    }
}

// print function prints the shortest path from start to end in format X1 Y1, X2 Y2
void printMyPath(Graph &g, int startRow, int startCol, int endRow, int endCol)
{
    vector<pair<int, int>> path;
    pair<int, int> tile; //<row, col>
    int r = endRow;
    int r2; // temp var
    int c = endCol;

    // traverse backedge till ya find startNode
    // can't print on this traversal b/c it will be end-to-start
    while ((r != startRow) || (c != startCol))
    {
        tile.first = r;
        tile.second = c;
        path.push_back(tile);

        r2 = g.graph[r][c].backRow; // temp so that c != [r2][c].backCol
        c = g.graph[r][c].backCol;
        r = r2; // r = g.graph[r][c].backRow
    }
    tile.first = startRow;
    tile.second = startCol;
    path.push_back(tile);

    // print path distance
    cout << g.graph[endRow][endCol].distance << endl;
    // print path from start to end
    for (int i = path.size() - 1; i > -1; i--)
    {
        cout << path[i].first << " " << path[i].second << endl;
    }
}
