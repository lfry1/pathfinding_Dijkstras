// dijsktras.cpp
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <vector>
#include <list>
#include <limits>

using namespace std;

struct Node
{
    int row;   // = 0;
    int col;   // = 0;
    char type; // = ' ';
    int cost;
    int distance; // = -1;
    vector<Node *> neighbors;
    int backRow;
    int backCol;
    bool visited; // = false;
};

struct Graph
{
    vector<vector<Node>> graph; // 2d vector, every tile on the map is a node in here
    map<char, int> edgeWeights; // ie g costs 2, m costs 7
    int MAP_ROWS;
    int MAP_COLS;
};

// mycomparison is for use with nodePQ
//  i straight up took this from https://www.cplusplus.com/reference/queue/priority_queue/priority_queue/
class mycomparison
{
    bool reverse;

public:
    mycomparison(const bool &revparam = false)
    {
        reverse = revparam;
    }
    bool operator()(const Node *n1, const Node *n2) const
    {
        if (reverse)
            return (n1->distance > n2->distance);
        else
            return (n1->distance < n2->distance);
    }
};
typedef priority_queue<Node *, vector<Node *>, mycomparison> nodePQ;

// declaration for function used to implement Dijkstra's
void shortestPath(Graph &G, int startRow, int startCol, int endRow, int endCol);

// declaration for print function - prints backedge path
void print(Graph &G, int startRow, int startCol, int endRow, int endCol);

// declaration for utility fn that finds valid neighbors of each node
void assignNeighbors(Graph &g);

// declaration for utility fn that prints the graph it read in
void printGraph(Graph &g);

//+++++++++++++++++++++++++ MAIN EXECUTION ++++++++++++++++++++++++
int main(int argc, char *argv[])
{
    // ifstream file;
    //  file.open("input");

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
            Node n; // should this be declared outside of the loop?
            map<char, int>::iterator t;
            n.row = i;
            n.col = j;
            n.type = tileType;
            n.cost = tileWeights.find(tileType)->second;
            n.distance = 999;
            n.backRow = -1;
            n.backCol = -1;
            n.visited = false;
            Node *nPtr = &n;
            rowV.push_back(n);
        }
        g.graph.push_back(rowV);
    }
    // get start and end positions
    cin >> startRow >> startCol >> endRow >> endCol;
    g.graph[startRow][startCol].distance = 0;

    assignNeighbors(g);
    // printGraph(g);
    shortestPath(g, startRow, startCol, endRow, endCol);
    cout << "eh?" << endl;
    print(g, startRow, startCol, endRow, endCol);
    return 0;
} // end main

//+++++++++++++++++++++++++ IMPLEMENTS DIJKSTRAS ++++++++++++++++++++++++
void shortestPath(Graph &g, int startRow, int startCol, int endRow, int endCol)
{
    multimap<int, Node *> unvisited;
    int curRow;
    int curCol;
    int adjRow;
    int adjCol;
    int d; // distance

    // insert starting value into unvisited

    unvisited.insert(pair<int, Node *>(0, &g.graph[startRow][startCol]));

    while (!unvisited.empty())
    {
        curRow = unvisited.begin()->second->row;
        curCol = unvisited.begin()->second->col;
        unvisited.erase(unvisited.begin());

        // cout << curRow << " " << curCol << "  has " << g.graph[curRow][curCol].neighbors.size() << " neighbors: " << endl;
        //  look thru each of n's neighbors
        for (int i = 0; i < g.graph[curRow][curCol].neighbors.size(); i++)
        {
            // cout << "   " << g.graph[curRow][curCol].neighbors[i]->row << " " << g.graph[curRow][curCol].neighbors[i]->col;
            // cout << " - VISITED = " << g.graph[curRow][curCol].neighbors[i]->visited << endl;

            // make sure neighbor hasn't already been visited
            if (g.graph[curRow][curCol].neighbors[i]->visited == false)
            {
                adjRow = g.graph[curRow][curCol].neighbors[i]->row;
                adjCol = g.graph[curRow][curCol].neighbors[i]->col;

                d = g.graph[curRow][curCol].distance + g.graph[curRow][curCol].cost;
                // cout << "       distance is " << d << endl;

                if (d < g.graph[curRow][curCol].neighbors[i]->distance)
                {
                    g.graph[curRow][curCol].neighbors[i]->distance = d;
                    g.graph[curRow][curCol].neighbors[i]->backRow = curRow;
                    g.graph[curRow][curCol].neighbors[i]->backCol = curCol;
                    unvisited.insert(pair<int, Node *>(d, g.graph[curRow][curCol].neighbors[i]));
                }
            }
        }
        g.graph[curRow][curCol].visited = true;
    }
}
/*
cout << "NODE:      " << g.graph[curRow][curCol].row << " " << g.graph[curRow][curCol].col << endl;
cout << "DISTANCE:  " << g.graph[curRow][curCol].distance << endl;
cout << "BACKEDGE:  " << g.graph[curRow][curCol].backRow << " " << g.graph[curRow][curCol].backCol << endl;
cout << endl;

for (int i = 0; i < g.MAP_ROWS; i++)
{
    for (int j = 0; j < g.MAP_COLS; j++)
    {
        cout << "FINAL NODE:            " << g.graph[i][j].row << " " << g.graph[i][j].col << endl;
        cout << "DISTANCE FROM START:   " << g.graph[i][j].distance << endl;
        cout << "FINAL BACKEDGE:        " << g.graph[i][j].backRow << " " << g.graph[i][j].backCol << endl;
        cout << endl;
    }
}
*/

//+++++++++++++++++++++++++ MISC UTILITY FNS ++++++++++++++++++++++++
void assignNeighbors(Graph &g)
{
    for (int i = 0; i < g.MAP_ROWS; i++)
    {
        for (int j = 0; j < g.MAP_COLS; j++)
        {
            // FIND ADJACENCIES AND PUT THEM IN THE unvisited
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

// utility function used to make sure i read in the graph correctly
void printGraph(Graph &g)
{
    for (int i = 0; i < g.MAP_ROWS; i++)
    {
        for (int j = 0; j < g.MAP_COLS; j++)
        {
            cout << i << " " << j << "'s neighbors are ";
            for (int k = 0; k < g.graph[i][j].neighbors.size(); k++)
            {
                cout << g.graph[i][j].neighbors[k]->row << " " << g.graph[i][j].neighbors[k]->col << "  ";
            }
            cout << endl;
        }
    }
}

// print function takes END NODE as arg, prints path from START TO END
void print(Graph &g, int startRow, int startCol, int endRow, int endCol)
{
    vector<pair<int, int>> path;
    pair<int, int> tile; //<row, col>
    int r = endRow;
    int r2;
    int c = endCol;

    // traverse backedge till ya find startNode
    while ((r != startRow) || (c != startCol))
    {
        tile.first = r;
        tile.second = c;
        path.push_back(tile);

        r2 = g.graph[r][c].backRow;
        c = g.graph[r][c].backCol;
        r = r2;
    }
    tile.first = startRow;
    tile.second = startCol;
    path.push_back(tile);

    cout << g.graph[endRow][endCol].distance <<endl; 
    for (int i = path.size() - 1; i > -1; i--)
    {
        cout << path[i].first << " " << path[i].second << endl;
    }
}
