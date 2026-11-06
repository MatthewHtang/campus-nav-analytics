#include <iostream>
#include <string>
using namespace std;

int main(){

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

        cin >> choice;
    } while (choice != 10);
}