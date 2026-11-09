/*
 * @Author: @MatthewHtang
 * @Date: 2026-11-06 18:23:39
 * @Last Modified time: 2026-11-06 18:23:39
 *
 * This is the actual file that include the implementation
 * for displaying, searching, counting for campus facilities
 */

#include "FacilityDatabase.h"
#include "Utils.h"
#include <iostream>

using namespace std;

// Call the load() from FacilityDatabase class and load the facilities file
void FacilityDatabase::load(const std::string &dataDir, const CampusGraph &graph)
{
    const string filename = "facilities.txt"; // Load the txt in filename

    // A vector that will temporarily hold the records read from the file
    vector<Record> records;
    int rejected = 0; // Counter for invalid or rejected record

    // Build a file path ->'filename' passes a file name sperately,
    //  and each record should contain 4 fields, 'records' recieves valid records by reference and
    //  rejected recieve updates to the rejected count
    readRecords(dataDir + "/" + filename, filename, 4, records, rejected);

    // Dont confuse Record in here is the data type
    // rec-> is temp data holder iterating through records
    for (const Record &rec : records)
    {

        // Sperate each 4 fileds into each category
        string id = toUpper(rec.fields[0]); // Capitalize the first field
        string name = rec.fields[1];
        string buildingInput = rec.fields[2];
        string category = rec.fields[3];

        // Validation check 1 for Id, is the id valid?
        if (!isValidId(id)) // if its false
        {
            cout << "Warning: "<< filename << " line " << rec.lineNumber 
            << ": invalid ID: '" << id
            << "' (must be F followed by exactly 3 digits)" << endl;
            rejected ++; // Count the reject
            continue;
        }

        //Check 2: building must exist, if fail cout the warning just like the above
        string building = graph.findBuilding(buildingInput);
        if (building == ""){
            cout << "Warning: "<< filename << " line " << rec.lineNumber 
            << ": unknown building: '" << buildingInput
            << "'" << endl;
            rejected ++; // Count the reject
            continue;
        }

        //Check3: id insertion fail if the id is already there
        int newIndex = facilities.size(); // This store the number of facilities currently in the vector
        
        //This return true or flase, try inserting the facility id, if fails reject the record
        bool inserted = idTable.insert(id, newIndex);  

        //now the condition that check
        if(inserted == false){
            cout << "Warning: "<< filename << " line " << rec.lineNumber 
            << ": duplicated ID: '" << id
            << "'" << endl;
            rejected ++; // Count the reject
            continue;
        }
        // Reuse category spelling from the first accepted facility.
        for (const Facility& existing : facilities) {
            if (equalsIgnoreCase(existing.getCategory(), category)) {
                category = existing.getCategory();
                break;
            }
        }

        //After all check passed, store it at exactly the index the table recorded
        facilities.push_back(Facility(id, name, building, category));
    }
    //Load the summary
    cout << filename << ": " << facilities.size() << " accepted, "
    << rejected << " rejected (hash capacity " << idTable.getCapacity()
    << ", entries " << idTable.getEntryCount()
    << ", collisions " << idTable.getCollisionCount() << ")" << endl;
}

//display and update the status of the facilities
void FacilityDatabase::displayAll() const{
    
    //Check whether the vector is empty
    if (facilities.empty()){

        //If its empty, yell at them with this message
        cout << "No facilities are loaded." << endl;
        cout << "Total facilities: 0 " << endl;
        return;
    }

    //Now we print each facility with for loop
    for (const Facility& f : facilities){
        f.print(); //We are print the temp data f with the print method
    }

    //After that, print the total facilities
    cout << "Total facilities: " << facilities.size() << endl;
}

//This searchById function convert the id->toUpper->trim,
//Then check if its empty or not, check whether its a valid id or not
//and finds its index
void FacilityDatabase::searchById(const string& input) const{
    string id = toUpper(trim(input)); // store the capitalized and trimmed id to id

    //Check if id is blank
    if (id == ""){
        cout << "Error: ID cannot be blank." << endl;
        return;
    }

    //Check if the id is valid or not
    if(!isValidId(id)){
        cout << "Invalid format: Remember ID is F with three digits (eg. F101)." << endl;
        return;
    }

    //finding the id
    int index = idTable.find(id); //store the id index into index

    //if its not found
    if(index == -1){
        cout << "Not found: no facility with ID " << id << endl;
        return;
    }
    //else print the facility with the print method
    cout << "Found: ";
    facilities[index].print();
}

//This returns the number of accepted facilities.
int FacilityDatabase::getCount() const{
    return facilities.size();
}