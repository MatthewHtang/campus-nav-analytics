/*
 * @Author: Matthew Htang
 * @Date: 2026-11-05 10:08:45 
 * @Last Modified by: Matthew Htang
 * @Last Modified time: 2026-11-05 10:09:05
 * 
 * This file contain a blue print for the data cleaner.
 * The following impplemented function include; 
 * triming, spliting, toUpper, toLower, equalsIgnoreCase, 
 * isValidId and read the record
 */


 // Add header guard to avoid compiler error
 #ifndef UTILS_H
 #define UTILS_H

 #include <string>
 #include <vector>

 using namespace std;

 struct Record
 {
    int lineNumber;
    vector<string> fields;
 };

 //Remove space, tab, \r, \n from both end of the text
 string trim(string text);

 //Split the text on the delimiter
 vector<string> split(string text, char delimiter);

 //Return a copy of text with all the letters converted to uppercase
 string toUpper(string text);

 //Return a copy of text with all the letter converted to lowercase
 string toLower(string text);

 //Return true or false if a and b match while ignoring capitalization
 bool equalsIgnoreCase(string a, string b);

 //This function return true or false after checking id validity
 bool isValidId(string id);

 //This readRecords return true or false depends on when the file could open or fail to open
 //It reads the file and skip the space and comment lines, wrong field count and empty 
 //fields are counted towards the rejectedCount
bool readRecords(
   string path, 
   string filename, 
   int expectedFieldCount,
   vector<Record>& records, 
   int& rejectedCount
);

 #endif

 