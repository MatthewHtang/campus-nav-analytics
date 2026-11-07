/*
 * @Author: Sankalp (Member C)
 * @Date: 2026-11-05
 * @Last Modified by: mikey.zhaopeng
 * @Last Modified time: 2026-11-06 19:31:19
 */

#include "CampusGraph.h"
#include "Utils.h"

#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

// Find a building's position in the list.
// Ignore extra spaces and capitalization.
// Return -1 if the building does not exist.
int CampusGraph::findBuilding(string name) const {
    string target = toUpper(trim(name));

    for (int i = 0; i < (int)buildings.size(); i++) {
        if (toUpper(trim(buildings[i])) == target) {
            return i;
        }
    }

    return -1;
}

// Add a building if it does not already exist.
bool CampusGraph::addBuilding(string name) {
    name = trim(name);

    // Do not add an empty or repeated building.
    if (name.empty() || findBuilding(name) != -1) {
        return false;
    }

    buildings.push_back(name);

    // Create an empty neighbor list for the new building.
    neighbors.push_back(vector<int>());

    buildings.push_back(cleanedName);
    neighbors.push_back(vector<int>());
    return true;
}

// Connect two buildings together.
bool CampusGraph::addConnection(string first, string second) {
    int a = findBuilding(first);
    int b = findBuilding(second);

    // Both buildings must exist and cannot be the same.
    if (a == -1 || b == -1 || a == b) {
        return false;
    }

    // Check if the connection already exists.
    for (int neighbor : neighbors[a]) {
        if (neighbor == b) {
            return false;
        }
    }

    // Add the connection in both directions.
    neighbors[a].push_back(b);
    neighbors[b].push_back(a);

    // Count each connection only once.
    connectionCount++;

    if (a == -1 || b == -1 || a == b ||
        hasConnection(first, second))
    {
        return false;
    }

    // Store both directions but count the undirected connection once.
    neighbors[a].push_back(b);
    neighbors[b].push_back(a);
    ++connectionCount;
    return true;
}

// Display all buildings and their neighbors.
void CampusGraph::displayGraph() const {
    if (buildings.empty()) {
        cout << "No buildings available.\n";
        return;
    }

    cout << "\nCampus Graph:\n";

    for (int i = 0; i < (int)buildings.size(); i++) {
        cout << buildings[i] << ": ";

        // Check if the building has any connections.
        if (neighbors[i].empty()) {
            cout << "(no connections)";
        }
        else {
            for (int j = 0; j < (int)neighbors[i].size(); j++) {
                if (j > 0) {
                    cout << ", ";
                }

                cout << buildings[neighbors[i][j]];
            }
        }

        cout << '\n';
    }
}

// Display total buildings, connections and degrees.
void CampusGraph::displayStatistics() const {
    cout << "\nCampus Graph Statistics:\n";

    cout << "Total buildings: " << buildings.size() << "\n";
    cout << "Total connections: " << connectionCount << "\n";

    // Degree means the number of connections a building has.
    for (int i = 0; i < (int)buildings.size(); i++) {
        cout << buildings[i]
             << " - Degree: "
             << neighbors[i].size() << "\n";
    }

    cout << "Connected components: " << componentCount << '\n';
}

// Breadth-First Search (BFS).
// Find all buildings reachable from a starting building.
void CampusGraph::bfs(string start) const {
    int startIndex = findBuilding(start);

    // Stop if the starting building does not exist.
    if (startIndex == -1) {
        cout << "Error: Unknown building.\n";
        return;
    }

    // Track which buildings have been visited.
    vector<bool> visited(buildings.size(), false);

    // Store the distance from the starting building.
    vector<int> distance(buildings.size(), -1);

    // Queue stores buildings waiting to be explored.
    queue<int> q;

    // Start BFS from the selected building.
    visited[startIndex] = true;
    distance[startIndex] = 0;
    pending.push(startIndex);

    size_t reachableCount = 0;

    cout << "BFS starting from " << buildings[startIndex] << ":\n";

    while (!pending.empty())
    {
        const int current = pending.front();
        pending.pop();

        cout << buildings[current] << " - Distance: "
             << distance[current] << '\n';
        ++reachableCount;

        for (size_t i = 0; i < neighbors[current].size(); ++i)
        {
            const int next = neighbors[current][i];

            // Mark visited when enqueued to avoid duplicate queue entries.
            if (!visited[next])
            {
                visited[next] = true;
                distance[next] = distance[current] + 1;
                pending.push(next);
            }
        }
    }

    cout << "Reachable: " << reachableCount << " of "
         << buildings.size() << " buildings\n";
}

void CampusGraph::shortestPath(
    const string& start,
    const string& destination) const
{
    const int startIndex = findBuilding(start);

    int reachableCount = 0;

    cout << "\nBFS starting from "
         << buildings[startIndex] << ":\n";

    // Continue until the queue is empty.
    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // Display the building and its distance.
        cout << buildings[current]
             << " - Distance: "
             << distance[current] << "\n";

        for (size_t i = 0; i < neighbors[current].size(); ++i)
        {
            const int next = neighbors[current][i];

        // Explore every neighboring building.
        for (int next : neighbors[current]) {
            if (!visited[next]) {
                visited[next] = true;

                // The next building is one connection farther.
                distance[next] = distance[current] + 1;

                q.push(next);
            }
        }
    }

    // Display the number of reachable buildings.
    cout << "Reachable: "
         << reachableCount
         << " of "
         << buildings.size()
         << " buildings\n";
}