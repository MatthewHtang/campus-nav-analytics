#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <string>
#include <vector>

using namespace std;

class CampusGraph {
private:
    vector<string> buildings;// it stores the name of buildings
    vector<vector<int>> neighbors;// it stores which buildings connect to each other
    int connectionCount = 0; // connectioncount helps to track the number of undirected connections

    int findBuilding(string name) const;// find Building() this function helps to find building regardless of capitalization

public:
    bool addBuilding(string name);
    bool addConnection(string first, string second);

    void displayGraph() const;
    void displayStatistics() const;
    void bfs(string start) const;// bfs() explores buildings using a queue 
};

#endif