#include <iostream>
#include <string>
#include <list>
#include <iterator>

using namespace std;

typedef struct
{
    int x;
    int y;
    bool locked = false;
} node;

class RoadNetwork
{

    int numAvenues, numStreets;
    list<node> fountain = list<node>();
    list<node> **graph;

public:
    RoadNetwork(int val1, int val2)
    {
        numAvenues = val1;
        numStreets = val2;
        *graph = new list<node>[numAvenues];        
        
        //add edges
        for (int i = 0; i < numAvenues; i++)
        {
            graph[i] = new list<node>[numStreets];            
            for (int j = 0; i < numStreets; j++)
            {
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
    }

    void addEdge(int startX,int startY, int endX, int endY)
    {
        node newNode;
        newNode.x = endX - 1;
        newNode.y = endY - 1;
        graph[startX-1][startY-1].push_back(newNode);
    }

    void addHouse(int avenue, int street)
    {
        node newNode;
        newNode.x = avenue - 1;
        newNode.y = street - 1;
        fountain.push_back(newNode);
    }

    void addSupermarket(int avenue, int street)
    {
        node sink;
        sink.x = -1;
        sink.y = -1;
        graph[avenue-1][street-1].push_back(sink);
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
}
