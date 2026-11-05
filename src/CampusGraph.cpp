#include "CampusGraph.h"
#include <iostream>
#include <queue>
#include <cctype>

using namespace std;

// Remove extra spaces from  the beginning and end.
string trimGraphName(string name) {
    size_t first = name.find_first_not_of(" \t\r\n");

    if (first == string::npos)//returns empty if the name only has spaces
        return "";

    size_t last = name.find_last_not_of(" \t\r\n");
    return name.substr(first, last - first + 1);
}

// changes a name to uppercase so that we can compare them easily
string normalizeGraphName(string name) {
    name = trimGraphName(name);

    for (char &letter : name) {
        letter = toupper(static_cast<unsigned char>(letter));
    }

    return name;
}

// Find a building's position in our list
// returns its position if found or -1 if not found
int CampusGraph::findBuilding(string name) const {
    string target = normalizeGraphName(name);

    for (int i = 0; i < (int)buildings.size(); i++) {
        if (normalizeGraphName(buildings[i]) == target)
            return i;
    }

    return -1;
}

// Add a building if it is not already present.
bool CampusGraph::addBuilding(string name) {
    name = trimGraphName(name);

    if (name.empty() || findBuilding(name) != -1)//donot add an empty or repeated building
        return false;

    buildings.push_back(name);
    neighbors.push_back(vector<int>());// make an empty neighbor list for the new building

    return true;
}

// connects two buildings together
bool CampusGraph::addConnection(string first, string second) {
    int a = findBuilding(first);
    int b = findBuilding(second);

    // Both buildings must exist and cannot be the same
    if (a == -1 || b == -1 || a == b)
        return false;

    //  Checks if buildings are already connected,Reject duplicates, including reversed connections.
    for (int neighbor : neighbors[a]) {
        if (neighbor == b)
            return false;
    }

    neighbors[a].push_back(b);// adds the connection in both directions
    neighbors[b].push_back(a);
    connectionCount++;// counts this connection only once.

    return true;
}

// Display buildings and their neighbors.
void CampusGraph::displayGraph() const {
    if (buildings.empty()) {
        cout << "No buildings available.\n";
        return;
    }

    for (int i = 0; i < (int)buildings.size(); i++) {// Goes through each building
        cout << buildings[i] << ": ";

        if (neighbors[i].empty()) {// checks if the building has any connections
            cout << "(no connections)";
        } else {
            for (int j = 0; j < (int)neighbors[i].size(); j++) {
                if (j > 0)
                    cout << ", ";

                cout << buildings[neighbors[i][j]];
            }
        }

        cout << "\n";
    }
}

// Shows the total buildings, connections and degrees
void CampusGraph::displayStatistics() const {
    cout << "Total buildings: " << buildings.size() << "\n";
    cout << "Total connections: " << connectionCount << "\n";

    for (int i = 0; i < (int)buildings.size(); i++) {// Degrees means how many connections a building has
        cout << buildings[i]
             << " - Degree: "
             << neighbors[i].size() << "\n";
    }
}

// Breadth-First Search using BFS to find all buildings we can reach from a start
void CampusGraph::bfs(string start) const {
    int startIndex = findBuilding(start);// finding the starting building

    if (startIndex == -1) {// Stoping if the building doesnot exist
        cout << "Error: Unknown building.\n";
        return;
    }

    vector<bool> visited(buildings.size(), false);// Keeps the tract of buildings we have already found
    vector<int> distance(buildings.size(), -1);// stores how many connections awayeach building is
    queue<int> q;// queue stores buildings waiting to be explored

    visited[startIndex] = true;// starting with 1 st building
    distance[startIndex] = 0;
    q.push(startIndex);

    int reachableCount = 0;// count how many building we can reach

    cout << "BFS starting from "
         << buildings[startIndex] << ":\n";

    while (!q.empty()) {// keep going untilthere are no more buildings to visit
        int current = q.front();// Get the first building from the queue
        q.pop();

        cout << buildings[current]// show the building and its distance
             << " - Distance: "
             << distance[current] << "\n";

        reachableCount++;

        for (int next : neighbors[current]) {// checking every building connected to this one.
            if (!visited[next]) {// only visit buildings we have not found yet.
                visited[next] = true;
                distance[next] = distance[current] + 1;// the next building is one connectionfarther
                q.push(next);// adds it to the queue to explore later
            }
        }
    }

    cout << "Reachable: " << reachableCount// show how many buildings were reachable
         << " of " << buildings.size()
         << " buildings\n";
}