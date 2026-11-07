/*
 * @Author: @MatthewHtang
 * @Date: 2026-11-06 10:06:24 
 * @Last Modified time: 2026-11-06 10:59:32
 * 
 * This file is the actual implementation of the facility.h.
 * 
 */
#include "Facility.h"
#include <iostream>
using namespace std;

//Calling the Constructor in Facility class -> Constructor ()Facility
//And initialize each private field to the constructor parameter
Facility :: Facility(string id, string name, string building, string category){
    
    this->id = id;
    this->name = name;
    this->building = building;
    this->category = category;
}

//Implement the functions for those getters in header file
//For getId, we will simply return the id without modifying it (const)
string Facility :: getId() const{
    return id;
}

//For getName,return the name without modifying 
string Facility :: getName() const{
    return name;
}

//Return the building without modifying
string Facility :: getBuilding() const{
    return building;
}

//Return the category without modifying
string Facility :: getCategory() const{
    return category;
}
 
//calling print function without modifying
void Facility :: print() const{
    cout << id << " | " << name << " | " << building << " | " << category << " | " << endl;
}

