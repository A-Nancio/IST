#include <iostream>
#include <string>
#include <list>

using namespace std;

typedef struct{
    int x;
    int y;
} point;

typedef struct{
    int d = 0;
    point pi;
    bool hasHouse = false;
    bool hasSuperMarket = false;
    bool locked = false;
} node; 



class GridGraph{
    node **graph;
public:
    GridGraph(int length, int height){
        graph = new node*[length];
        for (int i = 0; i < length; i++) {
            graph[i] = new node[height];
        } 
    }

    void addSuperMarket(int x, int y){
        graph[x][y].hasSuperMarket = 'true';
    }

    void addHouse(int x, int y){
        graph[x][y].hasHouse = 'true';
    }
};




int main(){
    int length, height, numSupermakets, numHouses;
    int x,y;
    //gather graph info
    cin >> length;
    cin.ignore();
    cin >> height;

    GridGraph roadNetwork(length, height);
    
    cin >> numSupermakets;
    cin.ignore();
    cin >> numHouses;

    for (int i = 0; i < numSupermakets; i++){
        cin >> x;
        cin >> y;
        roadNetwork.addSuperMarket(x, y);
    }

    for (int i = 0; i < numHouses; i++){
        cin >> x;
        cin.ignore();
        cin >> y;
        roadNetwork,addHouse(x, y);
    }
}

