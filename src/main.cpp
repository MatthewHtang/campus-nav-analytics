// Authors: Matthew Htang (Member A), Alka Gurung (Member B) and Sankalp (Member C)
// Purpose: Load campus data and run the Milestone 1 menu.

#include <iostream>
#include <string>
#include "CampusGraph.h"
#include "FacilityDatabase.h"
#include "DepartmentTree.h"
#include "Utils.h"

#include "Screen.h"

using namespace std;

/**
 * We need to build a helper function here:
 * If the user type a string input where our variables
 * expect for int input, and also to clear a buffer like java, 
 * We need a method that flushes and wipe out all the 
 * input pipe.
 */

 void clearInput(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); }

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

    pauseScreen(); // This let the user first see the load summary first

    int choice = 0;

    do {
        clearScreen(); // Clean screen and show the menu
    
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
        cout << "                  Enter Choice: ";

        cin >> choice;

        //What if the user put a string instead
        if (cin.fail()){
            clearInput();
            cout << "Error: enter a valid number" << endl;
            continue;
        }
        clearInput();

        switch(choice){

            //View all facilities
            case 1:
            db.displayAll();
            break;
            
            //Search facility by ID
            case 2: {
                string input = promptLine("Enter Facility ID: ");
                db.searchById(input);
            }

            //Search facility by Name
            case 3: {
                string input = promptLine("Enter Facility Name: ");
                db.searchByName(input);
                break;
            }

            case 4:
            //Call your function according to the menu
            break;

            case 5:
            graph.displayGraph();
            break;

            case 6:
            graph.displayStatistics();
            break;

            case 7: {
                string startBuilding = promptLine("Enter starting building: ");
                graph.bfs(startBuilding);
            }
            
            case 8:
            //Call your function according to the menu
            break;

            case 9:
            departments.addDepartmentFromInput();
            break;

            case 10: {
                string departmentName = promptLine("Enter department name: ");
                departments.searchDepartment(departmentName);
                break;
            }
            
            case 11:
            departments.displayHierarchy();
            break;

            case 12:
            departments.displayPreorder();
            break;

            case 13:
            //call your function accoriding to the menu
            break;

            case 14:
            //call your function accoriding to the menu
            break;

            case 15:
            //call your function accoriding to the menu
            break;

            case 16:
            //call your function accoriding to the menu
            break;

            case 17:
            //call your function accoriding to the menu
            break;

            case 18:
            cout << "exiting the program" << endl;
            break;

            default:
            cout << "Error: enter a valid number" << endl;
        }

        // Wait here so the user can read the result
        if (choice != 18){
            pauseScreen();
        }
    } while (choice != 18);

    return 0;
}
