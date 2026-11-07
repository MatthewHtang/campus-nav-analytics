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

    //Loading facilities, (needs the graph so it goes after buildings and connections)
    FacilityDatabase db;
    db.load("data", graph);

    //We create an empty bucket choice to store the user respond
    int choice = 0;

    do {
        cout << "\n";
        cout <<"\033[32m"<< "=============================================================\n";
        cout <<"\033[32m"<< "   Smart Campus Navigation and Facility Analytics System\n";
        cout <<"\033[32m"<< "=============================================================\n";
        cout <<"\033[32m"<< "|                  1. View all facilities                   |\n";
        cout <<"\033[32m"<< "|                  2. Search facility by ID                 |\n";
        cout <<"\033[32m"<< "|                  3. View campus graph                     |\n";
        cout <<"\033[32m"<< "|                  4. Display graph counts and degrees      |\n";
        cout <<"\033[32m"<< "|                  5. Find reachable buildings (BFS)        |\n";
        cout <<"\033[32m"<< "|                  6. Add department                        |\n";
        cout <<"\033[32m"<< "|                  7. Search department                     |\n";
        cout <<"\033[32m"<< "|                  8. Display department hierarchy          |\n";
        cout <<"\033[32m"<< "|                  9. Run preorder traversal                |\n";
        cout <<"\033[32m"<< "|                  10. Exit                                 |\n";
        cout <<"\033[32m"<< "=============================================================\n";
        cout <<"\033[32m"<< "                   Enter Choice: ";

          if (!(cin >> choice)) {
            if (cin.eof()) {
                break;
            }

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        //Facility Menu Option
        //Display all facilities in file order
        if (choice == 1){
            db.displayAll(); // display all 
        }
        //Search for a facility by ID and using the hash table
        else if (choice == 2){
            string input;
            // Remove the newline left by cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter Facility ID: ";
            getline(cin, input);
            db.searchById(input);
        }

        // Campus graph menu options
        else if (choice == 3) {
            // Display all buildings and their connections
            graph.displayGraph();
        }

        else if (choice == 4) {
            // Display graph counts and degrees
            graph.displayStatistics();
        }

        else if (choice == 5) {
            // Find reachable buildings using BFS
            string startBuilding;

            // Remove the newline left by cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter starting building: ";
            getline(cin, startBuilding);

            graph.bfs(startBuilding);
        }
    } while (choice != 10);
}