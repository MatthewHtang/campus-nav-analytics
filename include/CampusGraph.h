/*
 * @Author: Sankalp (Member C)
 * @Date: 2026-11-06 13:37:20 
 * @Last Modified by: mikey.zhaopeng
 * @Last Modified time: 2026-11-06 13:40:24
 */

#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <cstddef>
#include <string>
#include <vector>

using namespace std;

class CampusGraph
{
private:
    vector<string> buildings;
    vector<vector<int> > neighbors;
    size_t connectionCount;

    // Find a building, ignoring capitalization and outer whitespace.
    int findBuilding(const string& name) const;

public:
    CampusGraph();

    // Add a unique, nonempty building.
    bool addBuilding(const string& name);

    // Add an undirected connection between two existing, distinct buildings.
    bool addConnection(const string& first, const string& second);

    // These checks help the loader report specific rejection reasons.
    bool hasBuilding(const string& name) const;
    bool hasConnection(const string& first, const string& second) const;

    // Display buildings and neighbors in insertion order.
    void displayGraph() const;

    // Display total buildings, connections, and each building's degree.
    void displayStatistics() const;

    // Display degree leaders, isolated buildings, and component count.
    void displayConnectivityReport() const;

    // Print BFS visit order, distances, and reachable count.
    void bfs(const string& start) const;

    // Print a shortest path. Equal-length choices follow neighbor order.
    void shortestPath(
        const string& start,
        const string& destination) const;
};

#endif