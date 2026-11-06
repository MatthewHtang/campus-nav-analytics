#include <iostream>
#include <string>
#include "Facility.h"

using namespace std;

int main(){
    Facility f("F004", "Computer Lab A", "Science Building", "Computer Lab");
f.print();
cout << f.getId() << endl;
}
