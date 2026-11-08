/*
 * @Author: @MatthewHtang
 * @Date: 2026-11-06 09:29:58 
 * @Last Modified time: 2026-11-06 09:59:31
 * 
 * This file is a header file where we declared and implement 
 * the blue print for facilities. It has a class called Facility,
 * and Facility has four fields: id, name, building, category. And a constructor 
 * that run those function when the program is called. Getters functions are declared
 * as well as a method for printing
 */


 // Add header guard to avoid compiler error
#ifndef FACILITY_H
#define FACILITY_H
#include <string>

using namespace std;

//A class facility with 4 main fields
 class Facility 
 {
    private:
    string id;
    string name;
    string building;
    string category;

    //Constructor - create facility from its field
    public:
    Facility(string id, string name, 
        string building, string category);

    //We need a const getters for the fields to be publicly access
    string getId() const;
    string getName() const;
    string getBuilding() const;
    string getCategory() const;

    //Method that print those 4 fields
    void print() const;
 };

#endif