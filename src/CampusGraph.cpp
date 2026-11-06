/*
 * @Author: Sankalp (Member C)
 * @Date: 2026-11-05
 * @Last Modified by: Sankalp (Member C)
 *
 * Purpose: Implements CampusGraph: loading and validating buildings and
 *   connections, graph display, degrees, BFS reachability, shortest path
 *   (BFS with parent tracking) and the connectivity report.
 *
 * Credit: Alka (Member B) - owner of the graph / BFS / shortest path /
 *   connectivity work in the project's suggested team split.
 */

#include "CampusGraph.h"
#include "Utils.h"

#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

// Create an empty graph.
CampusGraph::CampusGraph() : edgeCount(0)
{
}

// ---------------------------------------------------------------------------
// Loading
// ---------------------------------------------------------------------------

// Load buildings.txt: one nonempty name per line; later duplicates (ignoring
// case) are rejected and the first spelling is kept.
void CampusGraph::loadBuildings(const string& dataDir)
{
    const string filename = "buildings.txt";
    vector<Record> records;
    int rejected = 0;
    readRecords(dataDir + "/" + filename, filename, 1, records, rejected);

    int accepted = 0;
    for (size_t i = 0; i < records.size(); i++)
    {
        const string& name = records[i].fields[0];
        int existing = findBuilding(name);
        if (existing >= 0)
        {
            printWarning(filename, records[i].lineNumber,
                         "duplicate building \"" + name +
                         "\" (already loaded as \"" + names[existing] + "\")");
            rejected++;
            continue;
        }
        names.push_back(name);
        adjacency.push_back(vector<int>());
        accepted++;
    }
    cout << filename << ": " << accepted << " accepted, "
         << rejected << " rejected" << endl;
}

// Load connections.txt: both names must be accepted buildings; self
// connections and duplicate/reversed connections are rejected.
void CampusGraph::loadConnections(const string& dataDir)
{
    const string filename = "connections.txt";
    vector<Record> records;
    int rejected = 0;
    readRecords(dataDir + "/" + filename, filename, 2, records, rejected);

    int accepted = 0;
    for (size_t i = 0; i < records.size(); i++)
    {
        const string& nameA = records[i].fields[0];
        const string& nameB = records[i].fields[1];
        int a = findBuilding(nameA);
        int b = findBuilding(nameB);
        int line = records[i].lineNumber;

        if (a < 0 && b < 0)
        {
            printWarning(filename, line, "unknown buildings \"" + nameA +
                         "\" and \"" + nameB + "\"");
            rejected++;
        }
        else if (a < 0)
        {
            printWarning(filename, line, "unknown building \"" + nameA + "\"");
            rejected++;
        }
        else if (b < 0)
        {
            printWarning(filename, line, "unknown building \"" + nameB + "\"");
            rejected++;
        }
        else if (a == b)
        {
            printWarning(filename, line, "self-connection \"" + names[a] + "\"");
            rejected++;
        }
        else if (hasEdge(a, b))
        {
            printWarning(filename, line, "duplicate or reversed connection \"" +
                         names[a] + "\" - \"" + names[b] + "\"");
            rejected++;
        }
        else
        {
            addEdge(a, b);
            accepted++;
        }
    }
    cout << filename << ": " << accepted << " accepted, "
         << rejected << " rejected" << endl;
}

// ---------------------------------------------------------------------------
// Lookup helpers
// ---------------------------------------------------------------------------

// Linear search over the building names, ignoring case; input is trimmed.
int CampusGraph::findBuilding(const string& name) const
{
    string target = trim(name);
    if (target.empty())
    {
        return -1;
    }
    for (size_t i = 0; i < names.size(); i++)
    {
        if (equalsIgnoreCase(names[i], target))
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Spelling from buildings.txt, or "" if the building is unknown.
string CampusGraph::canonicalName(const string& name) const
{
    int index = findBuilding(name);
    return index < 0 ? string() : names[index];
}

// Name of the building at index (caller must pass a valid index).
string CampusGraph::buildingName(int index) const
{
    return names[index];
}

// Number of buildings.
int CampusGraph::buildingCount() const
{
    return static_cast<int>(names.size());
}

// Number of undirected connections (each counted once).
int CampusGraph::connectionCount() const
{
    return edgeCount;
}

// Number of distinct neighbors of the building at index.
int CampusGraph::degree(int index) const
{
    return static_cast<int>(adjacency[index].size());
}

// True if a and b are already connected.
bool CampusGraph::hasEdge(int a, int b) const
{
    for (size_t i = 0; i < adjacency[a].size(); i++)
    {
        if (adjacency[a][i] == b)
        {
            return true;
        }
    }
    return false;
}

// Add an undirected edge: update both endpoints, count it once.
void CampusGraph::addEdge(int a, int b)
{
    adjacency[a].push_back(b);
    adjacency[b].push_back(a);
    edgeCount++;
}

// Validate a typed building name; print the error if it cannot be used.
bool CampusGraph::resolveOrReport(const string& name, int& index) const
{
    string trimmed = trim(name);
    if (trimmed.empty())
    {
        cout << "Error: building name cannot be blank." << endl;
        return false;
    }
    index = findBuilding(trimmed);
    if (index < 0)
    {
        cout << "Error: unknown building \"" << trimmed << "\"." << endl;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// Display
// ---------------------------------------------------------------------------

// Show every building (file order) with its neighbors (connection order).
void CampusGraph::displayGraph() const
{
    if (names.empty())
    {
        cout << "No buildings are loaded." << endl;
        return;
    }
    cout << "Campus graph (" << names.size() << " buildings, "
         << edgeCount << " connections):" << endl;
    for (size_t i = 0; i < names.size(); i++)
    {
        cout << names[i] << ": ";
        if (adjacency[i].empty())
        {
            cout << "(no connections)";
        }
        for (size_t j = 0; j < adjacency[i].size(); j++)
        {
            if (j > 0)
            {
                cout << ", ";
            }
            cout << names[adjacency[i][j]];
        }
        cout << endl;
    }
}

// Show total buildings, total connections and each building's degree.
void CampusGraph::displayCountsAndDegrees() const
{
    if (names.empty())
    {
        cout << "No buildings are loaded." << endl;
    }
    cout << "Total buildings: " << names.size() << endl;
    cout << "Total connections: " << edgeCount << endl;
    if (names.empty())
    {
        return;
    }
    cout << "Degree of each building:" << endl;
    for (size_t i = 0; i < names.size(); i++)
    {
        cout << "  " << names[i] << ": " << adjacency[i].size() << endl;
    }
}

// ---------------------------------------------------------------------------
// BFS
// ---------------------------------------------------------------------------

// BFS from startName. A building is marked visited when it is enqueued, so
// each one is queued once and distances are the fewest connections.
void CampusGraph::bfsReachability(const string& startName) const
{
    if (names.empty())
    {
        cout << "No buildings are loaded." << endl;
        return;
    }
    int start;
    if (!resolveOrReport(startName, start))
    {
        return;
    }

    vector<bool> visited(names.size(), false);
    vector<int> distance(names.size(), 0);
    queue<int> pending;
    visited[start] = true;
    pending.push(start);

    cout << "Buildings reachable from " << names[start]
         << " (BFS order):" << endl;
    int reachable = 0;
    while (!pending.empty())
    {
        int current = pending.front();
        pending.pop();
        reachable++;
        cout << "  " << names[current] << " (distance "
             << distance[current] << ")" << endl;

        for (size_t i = 0; i < adjacency[current].size(); i++)
        {
            int next = adjacency[current][i];
            if (!visited[next])
            {
                visited[next] = true; // mark at enqueue
                distance[next] = distance[current] + 1;
                pending.push(next);
            }
        }
    }
    cout << "Reachable: " << reachable << " of " << names.size()
         << " buildings" << endl;
}

// ---------------------------------------------------------------------------
// Shortest path (Final)
// ---------------------------------------------------------------------------

// BFS with parent tracking. Neighbors are scanned in insertion order and the
// first discovery sets the parent, so insertion order breaks ties.
void CampusGraph::shortestPath(const string& startName,
                               const string& destName) const
{
    if (names.empty())
    {
        cout << "No buildings are loaded." << endl;
        return;
    }
    int start = -1;
    int dest = -1;
    bool startOk = resolveOrReport(startName, start);
    bool destOk = resolveOrReport(destName, dest);
    if (!startOk || !destOk)
    {
        return;
    }

    vector<bool> visited(names.size(), false);
    vector<int> parent(names.size(), -1);
    queue<int> pending;
    visited[start] = true;
    pending.push(start);
    while (!pending.empty() && !visited[dest])
    {
        int current = pending.front();
        pending.pop();
        for (size_t i = 0; i < adjacency[current].size(); i++)
        {
            int next = adjacency[current][i];
            if (!visited[next])
            {
                visited[next] = true;
                parent[next] = current;
                pending.push(next);
            }
        }
    }

    if (!visited[dest])
    {
        cout << "No path exists between " << names[start] << " and "
             << names[dest] << "." << endl;
        return;
    }

    vector<int> path; // built from destination back to start
    for (int at = dest; at != -1; at = parent[at])
    {
        path.push_back(at);
    }
    cout << "Shortest path from " << names[start] << " to "
         << names[dest] << ":" << endl;
    for (size_t i = path.size(); i > 0; i--)
    {
        cout << "  " << names[path[i - 1]] << endl;
    }
    cout << "Number of connections: " << path.size() - 1 << endl;
}

// ---------------------------------------------------------------------------
// Connectivity report (Final)
// ---------------------------------------------------------------------------

// Count connected components with repeated BFS over unvisited buildings.
int CampusGraph::countComponents() const
{
    vector<bool> visited(names.size(), false);
    int components = 0;
    for (size_t s = 0; s < names.size(); s++)
    {
        if (visited[s])
        {
            continue;
        }
        components++;
        queue<int> pending;
        visited[s] = true;
        pending.push(static_cast<int>(s));
        while (!pending.empty())
        {
            int current = pending.front();
            pending.pop();
            for (size_t i = 0; i < adjacency[current].size(); i++)
            {
                int next = adjacency[current][i];
                if (!visited[next])
                {
                    visited[next] = true;
                    pending.push(next);
                }
            }
        }
    }
    return components;
}

// Totals, every building tied for highest degree, isolated buildings and
// the number of connected components.
void CampusGraph::connectivityReport() const
{
    if (names.empty())
    {
        cout << "No buildings are loaded." << endl;
        cout << "Total buildings: 0" << endl;
        cout << "Total connections: 0" << endl;
        return;
    }
    cout << "Total buildings: " << names.size() << endl;
    cout << "Total connections: " << edgeCount << endl;

    size_t highest = 0;
    for (size_t i = 0; i < names.size(); i++)
    {
        if (adjacency[i].size() > highest)
        {
            highest = adjacency[i].size();
        }
    }
    cout << "Highest degree (" << highest << "): ";
    bool first = true;
    for (size_t i = 0; i < names.size(); i++)
    {
        if (adjacency[i].size() == highest)
        {
            cout << (first ? "" : ", ") << names[i];
            first = false;
        }
    }
    cout << endl;

    cout << "Isolated buildings: ";
    first = true;
    for (size_t i = 0; i < names.size(); i++)
    {
        if (adjacency[i].empty())
        {
            cout << (first ? "" : ", ") << names[i];
            first = false;
        }
    }
    if (first)
    {
        cout << "None";
    }
    cout << endl;

    cout << "Connected components: " << countComponents() << endl;
}