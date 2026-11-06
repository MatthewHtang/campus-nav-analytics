/*
 * @Author: Matthew Htang 
 * @Date: 2026-11-05 10:07:02 
 * @Last Modified by: Matthew Htang
 * @Last Modified time: 2026-11-05 10:23:31
 * 
 * This file contains functions for file loader and cleaner,
 * The implemented functions in this program load the file, trim, split
 * and prepare for the next step. functions implemented in this program include:
 * 1) trim()
 * 2) split()
 * 3) toUpper/toLower()
 * 4) equalsIgnoreCase()
 * 5) isValidId()
 * 6) readRecords()
 */

#include "Utils.h"
#include <iostream>
#include <string>

using namespace std;

//Define whitespace character, tab, newline, and carriage return
const string WHITESPACE = " \t\n\r"; 

//Remove whitespace from the beginning
//We use const reference - accessing the original WHITESPACE without modifying it
string leftTrim(const string& s){

    //(size_t)unasigned integer type (cant be negative), to store from  
    //what index does the line start with character
    size_t start = s.find_first_not_of(WHITESPACE); 
   
    //if all spaces, return empty space string
    if (start == string::npos){
        return "";

    }
    //else return the left trim string
    else {
        return s.substr(start);
    }
}

//Remove the string from the end of the string
string rightTrim(const string& s){

    //Count the the first index from the end and store it in end
    size_t end = s.find_last_not_of(WHITESPACE);

    //If the string is all spaces, return empty
    if (end == string::npos){
        return "";
    }

    //else return the trimed string from the left
    else{
        return s.substr(0, end + 1);
    }
}

//Remove WHITESPACES from both side
//This return the fully trimed of a string
//First, left trim first and then right trim, then return
string trim(const string& s){
    return rightTrim(leftTrim(s));
}

//Build a vector string called split that passes two argument, one
//the actual text text and delimiter
vector<string>split(string text, char delimiter){
    vector<string> result; //This is the list of field we will return
    string current = ""; //This is where we will be building our string

    for (char c : text){    //Look at each character of a text
        // if the text is the same as our delimiter(|), then add current to the result
        if (c == delimiter){    
            result.push_back(current);
            current = "";
        }
        //else, add that character to current
        else {
            current += c;
        }
    }
    result.push_back(current); //save the last field
    return result;  //then return the result

}

//This function takes one string argument(text) and the 
//for loop take each character directly from text and 
// convert it to upper then modify it rightaway using '&' symbol
string toUpper(string text){
    for (char& c : text){
        c = toupper(c);
    }
    return text;
}

//This function convert upper to lower
string toLower(string text){
    for (char& c : text){
        c = tolower(c);
    }
    return text;
}


//This function take two string arguments a and b, compare them
//returns true if they are the same, false if they are not
bool equalsIgnoreCase(string a, string b){
    return toLower(a) == toLower(b);
}

/*
isValidId (expects an ID that is already trimmed and uppercased)

if length is not 4 → false
if first character is not 'F' → false
if any of characters 1–3 is not a digit → false
return true
*/

/*
readRecords: the shared filter from the previous message

open the file at path
if it failed to open:
    print error naming filename
    return false

lineNumber = 0
rejectedCount = 0
while getline reads a line:
    lineNumber += 1
    line = trim(line)
    if line is empty → skip to next line
    if line starts with '#' → skip to next line

    fields = split(line, '|')
    trim every field

    if number of fields ≠ expectedFieldCount:
        print warning: filename, lineNumber, "expected N fields, found M"
        rejectedCount += 1
        skip to next line

    if any field is empty:
        print warning: filename, lineNumber, "empty field"
        rejectedCount += 1
        skip to next line

    add Record{lineNumber, fields} to records
return true
*/



