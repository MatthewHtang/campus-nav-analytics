/*
 * @Author: Alka Gurung
 * @Date: 2026-11-08
 * @Last Modified: 2026-11-08
 *
 * This header file contains the declaration of the
 * CampusGraph class and its functions for managing
 * campus buildings and connections.
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
    vector<string> buildings;          // building names in file order
    vector<vector<int> > neighbors;    // neighbor indexes in connection order
    size_t connectionCount;            // undirected connections, counted once

    // Index of a building ignoring case and outer whitespace; -1 if unknown.
    int findBuildingIndex(const string& name) const;

public:
    // Start with no buildings or connections.
    CampusGraph();

    // Load buildings first, then connections. Each loader prints a summary.
    void loadBuildings(const string& dataDir);
    void loadConnections(const string& dataDir);

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

    // Spelling from buildings.txt for a building typed in any capitalization
    // (outer whitespace ignored); "" if the building is unknown.
    string findBuilding(const string& name) const;
};

#endif