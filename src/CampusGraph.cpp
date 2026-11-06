/*
 * @Author: Sankalp (Member C)
 * @Date: 2026-11-05
 * @Last Modified by: mikey.zhaopeng
 * @Last Modified time: 2026-11-06 13:36:35
 */

#include "CampusGraph.h"
#include "Utils.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

CampusGraph::CampusGraph()
    : connectionCount(0)
{
}

int CampusGraph::findBuilding(const string& name) const
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

bool CampusGraph::hasBuilding(const string& name) const
{
    return findBuilding(name) != -1;
}

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

bool CampusGraph::hasConnection(
    const string& first,
    const string& second) const
{
    const int a = findBuilding(first);
    const int b = findBuilding(second);

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

bool CampusGraph::addConnection(
    const string& first,
    const string& second)
{
    const int a = findBuilding(first);
    const int b = findBuilding(second);

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

void CampusGraph::displayGraph() const
{
    if (buildings.empty())
    {
        cout << "No buildings available.\n";
        return;
    }

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

void CampusGraph::displayStatistics() const
{
    cout << "Total buildings: " << buildings.size() << '\n';
    cout << "Total connections: " << connectionCount << '\n';

    for (size_t i = 0; i < buildings.size(); ++i)
    {
        cout << buildings[i] << " - Degree: "
             << neighbors[i].size() << '\n';
    }
}

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

    // Traverse each not-yet-visited building to count components.
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

void CampusGraph::bfs(const string& start) const
{
    const int startIndex = findBuilding(start);

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

    if (startIndex == -1)
    {
        cout << "Error: Unknown starting building \""
             << trim(start) << "\".\n";
        return;
    }

    const int destinationIndex = findBuilding(destination);

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