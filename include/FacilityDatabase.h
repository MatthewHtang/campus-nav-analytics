/*
 * @Author: @MatthewHtang
 * @Date: 2026-11-06 17:53:12 
 * @Last Modified time: 2026-11-06 17:53:12 
 * 
 * This file manages facilities and custom hash table and combine all the features in one place
 * Methods such as loading the file, displaying and searching and getting the count are all
 * included in this blueprint
 * 
 */

 #ifndef FACILITYDATABASE_H
 #define FACILITYDATABASE_H

 #include <string>
 #include <vector>
 #include "Facility.h"
 #include "HashTable.h"
 #include "CampusGraph.h"

 class FacilityDatabase{
    private:
    std::vector<Facility> facilities; //Accepted the facilities in the file order
    HashTable idTable; //Create an object name idTable using using our exsiting HashTable class

    //Declare methods for loading data, displaying data,
    //Search by id and get the count
    
    public:
    //Create a load method that take two parameters (recieve the data dir string without
    //copying or modifying, and create a parameter graph that recieve existing campusgraph object
    //by reference without copying or modifying)
    void load(const std::string& dataDir, const CampusGraph& graph);
    
    //This is for our Menu 1, printing every facility in file order
    void displayAll() const;

    //Search by ID for Menu 2
    void searchById(const std:: string& input) const;


    //This return howmany facilities were accepted
    int getCount() const;
 };


 #endif
