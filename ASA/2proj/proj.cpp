#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include <queue>
#define FOUNTAIN 0      //DO NOT CHANGE THESE VARIABLES
#define SINK     1      //
#define NIL      -1

using namespace std;

typedef struct
{
    int x;
    int y;
} Edge;

class RoadNetwork
{

    int numAvenues, numStreets, FSindex;
    list<Edge> **graph;
    bool **cap;

public:
    RoadNetwork(int val1, int val2)
    {
        numAvenues = val1;
        numStreets = val2;
        FSindex = numAvenues;  //Index to the fountain/sink
        
        graph = new list<Edge>*[numAvenues + 1];
        cap = new bool*[numAvenues];       
        //add edges
        for (int i = 0; i < numAvenues; i++)
        {
            graph[i] = new list<Edge>[numStreets];  
            cap[i] = new bool[numStreets];          
            
            for (int j = 0; j < numStreets; j++)
            {
                cap[i][j] = false;
                if (i > 0)
                    addEdge(i, j, i-1, j);
                if (i < numAvenues-1)
                    addEdge(i, j, i+1, j);
                if (j > 0)
                    addEdge(i, j, i, j-1);
                if (j < numStreets-1)
                    addEdge(i, j, i, j+1);
            }
        }
        graph[FSindex] = new list<Edge>[2];  //    graph[FSindex][0] = fountain, graph[FSindex][1] = sink
    }                                        //    note: graph[FSindex][1] is an empty list, sink has no foward edges

    void addEdge(int startX,int startY, int endX, int endY)
    {
        Edge newNode;
        newNode.x = endX;
        newNode.y = endY;
        graph[startX][startY].push_back(newNode);
    }

    void addHouse(int avenue, int street)
    {
        Edge newNode;
        newNode.x = avenue - 1;
        newNode.y = street - 1;
        graph[FSindex][FOUNTAIN].push_back(newNode);
    }

    void addSupermarket(int avenue, int street)
    {
        Edge sink;
        sink.x = FSindex;
        sink.y = SINK;
        graph[avenue-1][street-1].push_back(sink);
    }

    bool bfs()
    {
        //data for bfs
        bool visited[numAvenues][numStreets], sinkReached = false;
        Edge prev[numAvenues][numStreets], endVertex;
        for (int i = 0; i < numAvenues; i++)
            for (int j = 0; j < numStreets; j++)
            {
                prev[i][j].x = NIL;
                prev[i][j].y = NIL;
                visited[i][j] = false;
            }

        list<Edge> :: iterator vertex;
        queue<list<Edge>> q = queue<list<Edge>>();
        
        //q.push(graph[FSindex][FOUNTAIN]);    //add fountain to the queue
        int x = FSindex;
        int y = FOUNTAIN;
        
        for (vertex = graph[FSindex][FOUNTAIN].begin(); vertex != graph[FSindex][FOUNTAIN].end(); ++vertex)
        {
            visited[vertex->x][vertex->y] = true;
            prev[vertex->x][vertex->y].x = FSindex;
            prev[vertex->y][vertex->y].y = FOUNTAIN;
            q.push(graph[vertex->x][vertex->y]);
        }
        //start queue
        while (!q.empty())
        {   
            if (sinkReached)
                break;
            for (vertex = q.front().begin(); vertex != q.front().end(); vertex++)
            {   
                if (vertex->x == FSindex && vertex->y == SINK)
                {
                    sinkReached = true;
                    endVertex.x = vertex->x;
                    endVertex.y = vertex->y;
                    break;
                }
                cout << "x:" << x << "   y:" << y << endl;
                std::cout << std::boolalpha << "visited:" << visited[x][y] << std::endl;
                std::cout << std::noboolalpha << "cap:" << cap[x][y] << std::endl;
                if (visited[vertex->x][vertex->y] == false && cap[vertex->x][vertex->y] == false)
                {
                    visited[vertex->x][vertex->y] = true;
                    prev[vertex->x][vertex->y].x = x;
                    prev[vertex->x][vertex->y].y = y;
                    q.push(graph[vertex->x][vertex->y]);
                    cout << "pushing\n";
                }
            }
            q.pop();
        }

        if (!sinkReached)
            return false;   //sink wasn't reached, there are no more augmenting paths
        else    //sink was reached, augment the path
        {
            Edge edge = prev[endVertex.x][endVertex.y];
            while(edge.x != FSindex && edge.y != FOUNTAIN)
            {
                cap[edge.x][edge.y] = true;
                edge = prev[edge.x][edge.y];
            }
            return true;
        }
    }

    int edmondsKarp()
    {
        int counter = 0;
        bool hasAugmentingPath = true;
        while (hasAugmentingPath)
        {
            cout << "attempt:" << counter << endl;
            hasAugmentingPath = bfs();
            counter++;
        }
        return counter;
    }
};

int main()
{
    int totalAvenues, totalStreets;
    int numHouses, numSupermarkets;
    int avenue, street;
 
    //gather graph info
    cin >> totalAvenues;
    cin.ignore();
    cin >> totalStreets;
    RoadNetwork network = RoadNetwork(totalAvenues, totalStreets);

    //gather house and supermarket locations
    cin >> numSupermarkets;
    cin.ignore();
    cin >> numHouses;
    for (int i = 0; i < numSupermarkets; i++)
    {   
        cin >> avenue;
        cin.ignore();
        cin >> street;
        network.addSupermarket(avenue, street);
    }

    for (int i = 0; i < numHouses; i++)
    {
        cin >> avenue;
        cin.ignore();
        cin >> street;
        network.addHouse(avenue, street);
    }
    int out = network.edmondsKarp();
    cout << "output:" << out << endl;

    return 0;
}
