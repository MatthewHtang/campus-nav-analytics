#include "CampusGraph.h"
#include "Utils.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// Start with an empty graph.
CampusGraph::CampusGraph()
    : connectionCount(0)
{
}

// Replace the graph with the accepted buildings in file order.
void CampusGraph::loadBuildings(const string& dataDir)
{
    buildings.clear();
    neighbors.clear();
    connectionCount = 0;

    const string filename = "buildings.txt";
    vector<Record> records;
    int rejected = 0;
    int accepted = 0;
    readRecords(dataDir + "/" + filename, filename, 1, records, rejected);

    for (size_t i = 0; i < records.size(); ++i)
    {
        const string& name = records[i].fields[0];
        if (addBuilding(name))
        {
            ++accepted;
        }
        else
        {
            printWarning(filename, records[i].lineNumber,
                         "duplicate building \"" + name + "\"");
            ++rejected;
        }
    }
    printLoadSummary(filename, accepted, rejected);
}

// Load walkways between accepted buildings; keep their file order.
void CampusGraph::loadConnections(const string& dataDir)
{
    for (size_t i = 0; i < neighbors.size(); ++i)
    {
        neighbors[i].clear();
    }
    connectionCount = 0;

    const string filename = "connections.txt";
    vector<Record> records;
    int rejected = 0;
    int accepted = 0;
    readRecords(dataDir + "/" + filename, filename, 2, records, rejected);

    for (size_t i = 0; i < records.size(); ++i)
    {
        const string& first = records[i].fields[0];
        const string& second = records[i].fields[1];
        string reason;

        if (!hasBuilding(first))
        {
            reason = "unknown building \"" + first + "\"";
        }
        else if (!hasBuilding(second))
        {
            reason = "unknown building \"" + second + "\"";
        }
        else if (equalsIgnoreCase(first, second))
        {
            reason = "self-connection for \"" + first + "\"";
        }
        else if (hasConnection(first, second))
        {
            reason = "duplicate connection (including reversed connections)";
        }

        if (!reason.empty())
        {
            printWarning(filename, records[i].lineNumber, reason);
            ++rejected;
            continue;
        }

        addConnection(first, second);
        ++accepted;
    }
    printLoadSummary(filename, accepted, rejected);
}

// Index of a building ignoring case and outer whitespace; -1 if unknown.
int CampusGraph::findBuildingIndex(const string& name) const
{
    const string target = trim(name);

    if (target.empty())
    {
        return -1;
    }

    for (size_t i = 0; i < buildings.size(); ++i)
    {
        if (equalsIgnoreCase(buildings[i], target))
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}

// Check whether a building exists.
bool CampusGraph::hasBuilding(const string& name) const
{
    return findBuildingIndex(name) != -1;
}

// Add a building once and preserve its original spelling.
bool CampusGraph::addBuilding(const string& name)
{
    const string cleanedName = trim(name);

    if (cleanedName.empty() || hasBuilding(cleanedName))
    {
        return false;
    }

    buildings.push_back(cleanedName);
    neighbors.push_back(vector<int>());
    return true;
}

// Check either direction of a walkway.
bool CampusGraph::hasConnection(
    const string& first,
    const string& second) const
{
    const int a = findBuildingIndex(first);
    const int b = findBuildingIndex(second);

    if (a == -1 || b == -1 || a == b)
    {
        return false;
    }

    for (size_t i = 0; i < neighbors[a].size(); ++i)
    {
        if (neighbors[a][i] == b)
        {
            return true;
        }
    }

    return false;
}

// Add one walkway to both buildings' neighbor lists.
bool CampusGraph::addConnection(
    const string& first,
    const string& second)
{
    const int a = findBuildingIndex(first);
    const int b = findBuildingIndex(second);

    if (a == -1 || b == -1 || a == b ||
        hasConnection(first, second))
    {
        return false;
    }

    // Store both directions, but count the undirected connection once.
    neighbors[a].push_back(b);
    neighbors[b].push_back(a);
    ++connectionCount;
    return true;
}

// Show each building and its neighbors in insertion order.
void CampusGraph::displayGraph() const
{
    if (buildings.empty())
    {
        cout << "No buildings available.\n";
        return;
    }

    cout << "Campus Graph:\n";

    for (size_t i = 0; i < buildings.size(); ++i)
    {
        cout << buildings[i] << ": ";

        if (neighbors[i].empty())
        {
            cout << "(no connections)";
        }
        else
        {
            for (size_t j = 0; j < neighbors[i].size(); ++j)
            {
                if (j > 0)
                {
                    cout << ", ";
                }

                cout << buildings[neighbors[i][j]];
            }
        }

        cout << '\n';
    }
}

// Degree means the number of neighbors a building has.
void CampusGraph::displayStatistics() const
{
    if (buildings.empty())
    {
        cout << "No buildings available.\n";
    }
    cout << "Total buildings: " << buildings.size() << '\n';
    cout << "Total connections: " << connectionCount << '\n';

    for (size_t i = 0; i < buildings.size(); ++i)
    {
        cout << buildings[i] << " - Degree: "
             << neighbors[i].size() << '\n';
    }
}

// Final milestone: show isolated buildings and connected groups.
void CampusGraph::displayConnectivityReport() const
{
    cout << "Campus connectivity report:\n";
    cout << "Total buildings: " << buildings.size() << '\n';
    cout << "Total connections: " << connectionCount << '\n';

    if (buildings.empty())
    {
        cout << "Highest degree: 0\n";
        cout << "Buildings with highest degree: None\n";
        cout << "Isolated buildings: None\n";
        cout << "Connected components: 0\n";
        return;
    }

    size_t highestDegree = 0;

    for (size_t i = 0; i < neighbors.size(); ++i)
    {
        if (neighbors[i].size() > highestDegree)
        {
            highestDegree = neighbors[i].size();
        }
    }

    cout << "Highest degree: " << highestDegree << '\n';
    cout << "Buildings with highest degree: ";

    bool firstLeader = true;

    for (size_t i = 0; i < buildings.size(); ++i)
    {
        if (neighbors[i].size() == highestDegree)
        {
            if (!firstLeader)
            {
                cout << ", ";
            }

            cout << buildings[i];
            firstLeader = false;
        }
    }

    cout << '\n';
    cout << "Isolated buildings: ";

    bool foundIsolated = false;

    for (size_t i = 0; i < buildings.size(); ++i)
    {
        if (neighbors[i].empty())
        {
            if (foundIsolated)
            {
                cout << ", ";
            }

            cout << buildings[i];
            foundIsolated = true;
        }
    }

    if (!foundIsolated)
    {
        cout << "None";
    }

    cout << '\n';

    // Count connected components with a queue traversal.
    vector<bool> visited(buildings.size(), false);
    size_t componentCount = 0;

    for (size_t start = 0; start < buildings.size(); ++start)
    {
        if (visited[start])
        {
            continue;
        }

        ++componentCount;

        queue<int> pending;
        visited[start] = true;
        pending.push(static_cast<int>(start));

        while (!pending.empty())
        {
            const int current = pending.front();
            pending.pop();

            for (size_t i = 0; i < neighbors[current].size(); ++i)
            {
                const int next = neighbors[current][i];

                if (!visited[next])
                {
                    visited[next] = true;
                    pending.push(next);
                }
            }
        }
    }

    cout << "Connected components: " << componentCount << '\n';
}

// BFS uses a queue to visit nearby buildings before farther ones.
void CampusGraph::bfs(const string& start) const
{
    if (trim(start).empty())
    {
        cout << "Error: building name cannot be blank.\n";
        return;
    }
    const int startIndex = findBuildingIndex(start);

    if (startIndex == -1)
    {
        cout << "Error: Unknown building \"" << trim(start) << "\".\n";
        return;
    }

    vector<bool> visited(buildings.size(), false);
    vector<int> distance(buildings.size(), -1);
    queue<int> pending;

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

            // Mark visited when enqueued so a building enters the queue once.
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

// Final milestone: remember parents during BFS to rebuild the route.
void CampusGraph::shortestPath(
    const string& start,
    const string& destination) const
{
    const int startIndex = findBuildingIndex(start);

    if (startIndex == -1)
    {
        cout << "Error: Unknown starting building \""
             << trim(start) << "\".\n";
        return;
    }

    const int destinationIndex = findBuildingIndex(destination);

    if (destinationIndex == -1)
    {
        cout << "Error: Unknown destination building \""
             << trim(destination) << "\".\n";
        return;
    }

    vector<bool> visited(buildings.size(), false);
    vector<int> parent(buildings.size(), -1);
    vector<int> distance(buildings.size(), -1);
    queue<int> pending;

    visited[startIndex] = true;
    distance[startIndex] = 0;
    pending.push(startIndex);

    while (!pending.empty() && !visited[destinationIndex])
    {
        const int current = pending.front();
        pending.pop();

        for (size_t i = 0; i < neighbors[current].size(); ++i)
        {
            const int next = neighbors[current][i];

            if (!visited[next])
            {
                visited[next] = true;
                parent[next] = current;
                distance[next] = distance[current] + 1;
                pending.push(next);
            }
        }
    }

    if (!visited[destinationIndex])
    {
        cout << "No path exists from " << buildings[startIndex]
             << " to " << buildings[destinationIndex] << ".\n";
        return;
    }

    vector<int> path;

    for (int current = destinationIndex;
         current != -1;
         current = parent[current])
    {
        path.push_back(current);
    }

    reverse(path.begin(), path.end());

    cout << "Shortest path from " << buildings[startIndex]
         << " to " << buildings[destinationIndex]
         << " (" << distance[destinationIndex]
         << " connections):\n";

    for (size_t i = 0; i < path.size(); ++i)
    {
        cout << buildings[path[i]] << '\n';
    }
}

// Spelling from buildings.txt, or "" if the building is unknown.
string CampusGraph::findBuilding(const string& name) const
{
    const int index = findBuildingIndex(name);

    if (index == -1)
    {
        return string();
    }

    return buildings[static_cast<size_t>(index)];
}