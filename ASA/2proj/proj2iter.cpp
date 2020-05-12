#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include <queue>
#define IN_OUT   2
#define IN       0
#define OUT      1
#define NIL      -2
#define SINK     -1


using namespace std;

typedef class Point
{
    int _x;
    int _y;
    int _IO;

public: 
    Point(int x, int y, IO){
        _x = x;
        _y = y;
        _IO = IO;
    }
};

class Edge
{
    Point next;
    int capacity = 0;

public:
    Edge(int x, int y, int IO)
    {
        next = Point(x, y, IO);
    }
};

class RoadNetwork
{
    int numAvenues, numStreets, FSindex;
    list<Edge> fountain = list<Edge>();
    list<Edge> ***graph;

public:
    RoadNetwork(int val1, int val2)
    {
        numAvenues = val1;
        numStreets = val2;
        FSindex = numAvenues;   //Index to the fountain/sink

        graph = new list<Edge>**[numAvenues + 1];
        
        //add edges
        for (int i = 0; i < numAvenues; i++)
        {
            graph[i] = new list<Edge>*[numStreets];
            for(int j = 0; j < numStreets; j++)
            {
                graph[i][j] = new list<Edge>[IN_OUT];
                addEdges(i, j);
            }
        }
    }

    void addEdges(int avenue, int street)
    
    {
        graph[avenue][street][IN].push_back(Edge(avenue, street, OUT));
        if (avenue > 0)
        {   
            graph[avenue-1][street][OUT].push_back(Edge(avenue, street, IN));
            graph[avenue][street][OUT].push_back(Edge(avenue-1, street, IN));
        }
        
        if (avenue < numAvenues)
        {
            graph[avenue +1][street][IN].push_back(Edge(avenue, street, IN));
            graph[avenue][street][OUT].push_back(Edge(avenue +1, street, IN));
        }

        if (street > 0)
        {
            graph[avenue][street-1][IN].push_back(Edge(avenue, street, IN));
            graph[avenue][street][OUT].push_back(Edge(avenue, street-1, IN));
        }

        if (avenue < numStreets)
        {
            graph[avenue][street +1][IN].push_back(Edge(avenue, street, IN));
            graph[avenue][street][OUT].push_back(Edge(avenue, street+1, IN));
        }
    }

    list<Edge> getAdjEdges(point pnt)
    {
        return graph[pnt.x][pnt.y][pnt.IO];
    }

    void addHouse(int avenue, int street)
    {
        Edge newEdge = Edge(avenue - 1, street - 1, IN);
        fountain.push_back(newEdge);
    }

    void addSupermarket(int avenue, int street)
    {
        Edge newEdge = Edge(SINK, SINK, IN);
        graph[avenue-1][street-1][OUT].push_back(newEdge);
    }

    bool bfs()
    {
        //data for bfs
        bool visited[numAvenues][numStreets][IN_OUT], sinkReached = false;
        point prev[numAvenues][numStreets][IN_OUT], endVertex;
        for (int i = 0; i < numAvenues; i++)
            for (int j = 0; j < numStreets; j++)
            {
                visited[i][j][IN] = false;
                visited[i][j][OUT] = false;
                prev[i][j][IN] = Point(NIL, NIL, NIL);
                prev[i][j][OUT] = Point(NILL, NILL, NILL);
            }

        queue<point> q;
        q.push(fountain);

        while(!q.empty())
        {

        }
    }
};