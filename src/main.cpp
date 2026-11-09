// Authors: Matthew Htang (Member A), Alka Gurung (Member B) and Sankalp (Member C)
// Purpose: Load campus data and run the Milestone 1 menu.

#include <iostream>
#include <string>
#include "CampusGraph.h"
#include "FacilityDatabase.h"
#include "DepartmentTree.h"
#include "Utils.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Use data by default, or the folder given on the command line.
    string dataDir = argc > 1 ? argv[1] : "data";
    CampusGraph graph;
    FacilityDatabase db;
    DepartmentTree departments;

    // Buildings must load before connections and facilities.
    graph.loadBuildings(dataDir);
    graph.loadConnections(dataDir);
    db.load(dataDir, graph);
    departments.loadDepartments(dataDir);

    int choice = -1;

    do {
        cout << "\n";
        cout << "\033[32m" << "=============================================================\n";
        cout << "\033[32m" << "   Smart Campus Navigation and Facility Analytics System\n";
        cout << "\033[32m" << "=============================================================\n";
        cout << "\033[32m" << "|                  1. View all facilities                   |\n";
        cout << "\033[32m" << "|                  2. Search facility by ID                 |\n";
        cout << "\033[32m" << "|                  3. View campus graph                     |\n";
        cout << "\033[32m" << "|                  4. Display graph counts and degrees      |\n";
        cout << "\033[32m" << "|                  5. Find reachable buildings (BFS)        |\n";
        cout << "\033[32m" << "|                  6. Add department                        |\n";
        cout << "\033[32m" << "|                  7. Search department                     |\n";
        cout << "\033[32m" << "|                  8. Display department hierarchy          |\n";
        cout << "\033[32m" << "|                  9. Run preorder traversal                |\n";
        cout << "\033[32m" << "|                  0. Exit                                   |\n";
        cout << "\033[32m" << "=============================================================\n";
        cout << "\033[0m";

        // Read a whole line so blank or invalid choices do not get stuck.
        choice = readMenuChoice("                   Enter Choice: ", 0, 9, 0);

        if (choice == -1) {
            continue;
        }

        // Facility menu options
        if (choice == 1) {
            db.displayAll();
        }
        else if (choice == 2) {
            string input = promptLine("Enter Facility ID: ");
            db.searchById(input);
        }

        // Campus graph menu options
        else if (choice == 3) {
            graph.displayGraph();
        }
        else if (choice == 4) {
            graph.displayStatistics();
        }
        else if (choice == 5) {
            string startBuilding = promptLine("Enter starting building: ");
            graph.bfs(startBuilding);
        }

        // Department tree menu options
        else if (choice == 6) {
            // The first department becomes the root without a parent prompt.
            departments.addDepartmentFromInput();
        }
        else if (choice == 7) {
            string departmentName = promptLine("Enter department name: ");
            departments.searchDepartment(departmentName);
        }
        else if (choice == 8) {
            // Show the hierarchy with four spaces for each depth.
            departments.displayHierarchy();
        }
        else if (choice == 9) {
            // Visit each parent before its children.
            departments.displayPreorder();
        }
    } while (choice != 0);

    return 0;
}
