// Authors: Matthew Htang (Member A), Alka Gurung (Member B) and Sankalp (Member C)
// Purpose: Load campus data and run the Milestone 1 menu.

#include <iostream>
#include <string>
#include "CampusGraph.h"
#include "FacilityDatabase.h"
#include "DepartmentTree.h"
#include "Utils.h"

#include <stdlib.h>

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

    int choice;

    do {
        cout << "\n";
        cout << "\033[32m" << "=============================================================\n";
        cout << "\033[32m" << "   Smart Campus Navigation and Facility Analytics System\n";
        cout << "\033[32m" << "=============================================================\n";
        cout << "\033[32m" << "|                  1. View all facilities                   |\n";
        cout << "\033[32m" << "|                  2. Search facility by ID                 |\n";
        cout << "\033[32m" << "|                  3. Search facility by name               |\n";
        cout << "\033[32m" << "|                  4. Search facility by category           |\n";
        cout << "\033[32m" << "|                  5. View campus graph                     |\n";
        cout << "\033[32m" << "|                  6. Display graph counts and degrees      |\n";
        cout << "\033[32m" << "|                  7. Find reachable buildings (BFS)        |\n";
        cout << "\033[32m" << "|                  8. Find shortest path (BFS)              |\n";
        cout << "\033[32m" << "|                  9. Add department                        |\n";
        cout << "\033[32m" << "|                 10. Search department                     |\n";
        cout << "\033[32m" << "|                 11. Display department hierarchy          |\n";
        cout << "\033[32m" << "|                 12. Preorder traversal                    |\n";
        cout << "\033[32m" << "|                 13. Postorder traversal                   |\n";
        cout << "\033[32m" << "|                 14. Level-order traversal                 |\n";
        cout << "\033[32m" << "|                 15. Department statistics                 |\n";
        cout << "\033[32m" << "|                 16. Facility category report              |\n";
        cout << "\033[32m" << "|                 17. Graph connectivity report             |\n";
        cout << "\033[32m" << "|                 18. Exit                                  |\n";
        cout << "\033[32m" << "=============================================================\n";
        cout << "\033[0m";
        cout << "                                Enter Choice: \n";
        
        cin >> choice;
        cin.ignore();

        // Facility menu options
        if (choice == 1) {
            db.displayAll();
        }
        else if (choice == 2) {
            string input = promptLine("Enter Facility ID: ");
            db.searchById(input);
        }
        else if (choice == 3) {
            //call your function accoriding to the menu
        }
        else if (choice == 4) {
            //call your function accoriding to the menu
        }
        else if (choice == 5) {
            graph.displayGraph();
        }
        else if (choice == 6) {
            graph.displayStatistics();
        }
        else if (choice == 7) {
            string startBuilding = promptLine("Enter starting building: ");
            graph.bfs(startBuilding);
        }
        else if (choice == 8) {
            //call your function accoriding to the menu
        }
        else if (choice == 9) {
            // The first department becomes the root without a parent prompt.
            departments.addDepartmentFromInput();
        }
        else if (choice == 10){
            string departmentName = promptLine("Enter department name: ");
            departments.searchDepartment(departmentName);
        }
        else if (choice == 11) {
            // Show the hierarchy with four spaces for each depth.
            departments.displayHierarchy();
        }
        else if (choice == 12) {
            // Visit each parent before its children.
            departments.displayPreorder();
        }
        else if (choice == 13) {
            //call your function accoriding to the menu
        }
        else if (choice == 14) {
            //call your function accoriding to the menu
        }
        else if (choice == 15) {
           //call your function accoriding to the menu
        }
        else if (choice == 16) {
            //call your function accoriding to the menu
        }
        else if (choice == 17) {
            //call your function accoriding to the menu
        }
        else{
            cout << "Error: enter a valid number" << endl;
        }
        
    } while (choice != 18);

    return 0;
}
