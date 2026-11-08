#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include "CampusGraph.h"
#include "FacilityDatabase.h"
using namespace std;

int main(){
CampusGraph graph;

    // Loading campus buildings
    ifstream buildingFile("data/buildings.txt");
    string buildingName;

    if (!buildingFile.is_open()) {
        cout << "Error: Could not open buildings file.\n";
    }
    else {
        while (getline(buildingFile, buildingName)) {
            // Skipping blank lines and comments
            if (buildingName.find_first_not_of(" \t\r\n")
                    == string::npos) {
                continue;
            }

            if (buildingName.find_first_not_of(" \t\r\n") != string::npos &&
                buildingName[buildingName.find_first_not_of(" \t\r\n")] == '#') {
                continue;
            }

            graph.addBuilding(buildingName);
        }

        buildingFile.close();
    }
     // Loading campus connections
    ifstream connectionFile("data/connections.txt");
    string line;

    if (!connectionFile.is_open()) {
        cout << "Error: Could not open connections file.\n";
    }
    else {
        while (getline(connectionFile, line)) {

            // Skip blank lines and comments
            size_t first = line.find_first_not_of(" \t\r\n");

            if (first == string::npos || line[first] == '#') {
                continue;
            }

            // Find the separator between building names
            size_t separator = line.find('|');

            if (separator == string::npos) {
                cout << "Invalid connection: " << line << "\n";
                continue;
            }

            // Get the two building names
            string buildingA = line.substr(0, separator);
            string buildingB = line.substr(separator + 1);

            // Add the connection to the graph
            if (!graph.addConnection(buildingA, buildingB)) {
                cout << "Could not add connection: " << line << "\n";
            }
        }

        connectionFile.close();
    }