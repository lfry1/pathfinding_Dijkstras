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
    int distance;   // = -1;
    Node *backedge; // = NULL
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

// i straight up took this from https://www.cplusplus.com/reference/queue/priority_queue/priority_queue/
class mycomparison
{
    bool reverse;

public:
    mycomparison(const bool &revparam = false)
    {
        reverse = revparam;
    }
    bool operator()(const Node &n1, const Node &n2) const
    {
        if (reverse)
            return !((n1.row == n2.row) && (n1.col == n2.col));
        else
            return ((n1.row == n2.row) && (n1.col == n2.col));
    }
};

typedef priority_queue<Node, vector<Node>, mycomparison> nodePQ;

// declaration for function used to implement Dijkstra's
void shortestPath(Graph &G, int startRow, int startCol, int endRow, int endCol);

// declaration for print function - prints backedge path
void print(Graph &G, int startRow, int startCol, int endRow, int endCol);

// declaration for utility fn that prints the graph it read in
void printGraph(Graph &g);

// utility fn that creates a multimap of adjacent node*s
// void createAdjList(Graph &g);

// Main Execution
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
            n.distance = numeric_limits<int>::max();
            n.backedge = NULL;
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
    Node startNode;
    startNode.row = startRow; // row and col are all you ned to compare Nodes
    startNode.col = startCol;

    // nodePQ tilesByDistance; // shortestPath will fill this
    shortestPath(g, startRow, startCol, endRow, endCol);
    // cout << "eh?" << endl;
    print(g, startRow, startCol, endRow, endCol);
    return 0;
} // end main

// IMPLEMENTS DIJKSTRAS
void shortestPath(Graph &g, int startRow, int startCol, int endRow, int endCol)
{
    nodePQ unvisited;
    // nodePQ visited; // do we even need this for this application
    vector<Node> visited;
    vector<Node> fVisited;

    Node start;
    Node end2;
    Node *end;
    // Node *nullDummy = new Node;
    //  CREATE UNVISITED
    for (int i = 0; i < g.MAP_ROWS; i++)
    {
        for (int j = 0; j < g.MAP_COLS; j++)
        {
            // change start node dist so it will be at top of unvisited
            if ((g.graph[i][j].row == startRow) && (g.graph[i][j].col == startCol))
            {
                g.graph[i][j].distance = 0;
                // g.graph[i][j].backedge = nullDummy;
                g.graph[i][j].backRow = startRow - 1; // orleave as -1?
                g.graph[i][j].backCol = startCol - 1;
            }
            unvisited.push(g.graph[i][j]);
        }
    }

    // WHILE UNVISITED IS NOT EMPTY
    while (!unvisited.empty())
    {
        Node n = unvisited.top(); // need to pop b4 next loop
        nodePQ frontier;

        // FIND ADJACENCIES AND PUT THEM IN THE FRONTIER
        if (n.row > 0) // up: i-1
        {
            frontier.push(g.graph[n.row - 1][n.col]);
        }
        if (n.row < g.MAP_ROWS - 2) // down: i+1
        {
            frontier.push(g.graph[n.row + 1][n.col]);
        }
        if (n.col > 0) // left: j-1
        {
            frontier.push(g.graph[n.row][n.col - 1]);
        }
        if (n.col < g.MAP_COLS - 2) // right: j+1
        {
            frontier.push(g.graph[n.row][n.col + 1]);
        }

        // FOR EACH ADJACENT NODE n2 ADJACENT TO n
        while (!frontier.empty())
        {
            Node n2 = frontier.top(); // need to pop b4 next loop
            int thisDistance;
            if (!n2.visited)
            {
                thisDistance = n.distance + n2.cost;
                // if you found a shorter path, update distance & backedge
                if (n2.distance > thisDistance) // SIGN
                {
                    g.graph[n2.row][n2.col].distance = thisDistance;
                    g.graph[n.row][n.col].backRow = n2.row;
                    g.graph[n.row][n.col].backCol = n2.col;
                }
            }
            frontier.pop(); // pops n2
        }                   // end while(!frontier.empty())
        n.visited = true;
        visited.push_back(n);
        unvisited.pop(); // pops n
    }                    // end while(!unvisited.empty())

    /* //TESTING -  UNCOMMENT THIS TO SEE MORE PROBLEM DETAILS
    visited.pop();
    visited.pop();
    Node top = visited.top();
    cout << "at END of dijkstras I think top is " << top.row << " " << top.col << endl;
    cout << "and I think top->backedge is  " << top.backedge->row << " " << top.backedge->col << endl;
    cout << "at the END of dijkstras I think end2->backedge is " << end2.backedge->row << " " << end2.backedge->col << endl;


    for (int i = 1; i < visited.size(); i++)
    {
        cout << "NODE FINAL:      " << visited[i].row << " " << visited[i].col << endl;
        cout << "BACKEDGE FINAL:  " << g.graph[visited[i].row][visited[i].col].backRow << " " << g.graph[visited[i].row][visited[i].col].backCol << endl;
    }
    */
} // end dijkstras

//+++++++++++++++++++++++++ MISC UTILITY FNS ++++++++++++++++++++++++

// print function takes END NODE as arg, prints path from START TO END
void print(Graph &g, int startRow, int startCol, int endRow, int endCol)
{
    cout << "entered print function" << endl;
    vector<pair<int, int>> path;
    pair<int, int> tile; //<row, col>
    int r = endRow;
    int c = endCol;
    // traverse backedge till ya find startNode
    while (!(r == startRow && c == startCol))
    {
        tile.first = r;
        tile.second = c;
        cout << "NODE:      " << r << " " << c << endl;
        cout << "BACKEDGE:  " << g.graph[r][c].backRow << " " << g.graph[r][c].backCol << endl;
        cout << endl;
        path.push_back(tile);
        r = g.graph[r][c].backRow;
        c = g.graph[r][c].backCol;
    }
    cout << "while loop completed" << endl;
    /*
    for (int i = 0; i < path.size(); i++)
    {
        cout << path[i].first << " " << path[i].second << endl;
    }
    */
}

// utility function used to make sure i read in the graph correctly
void printGraph(Graph &g)
{
    for (int i = 0; i < g.MAP_ROWS; i++)
    {
        for (int j = 0; j < g.MAP_COLS; j++)
        {
            cout << g.graph[i][j].type << " ";
        }
        cout << endl;
    }
}
