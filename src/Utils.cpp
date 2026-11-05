/*
 * @Author: Matthew Htang 
 * @Date: 2026-11-05 10:07:02 
 * @Last Modified by: Matthew Htang
 * @Last Modified time: 2026-11-05 10:23:31
 * 
 * 
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

/*
Split 

result = empty list
current = empty string
for each character c in text:
    if c is the delimiter:
        add current to result
        reset current to empty
    else:
        add c to current
add current to result      ← this last step keeps "A|B|" as 3 fields
return result
*/

/*
toUpper/toLower

copy = text
for each character in copy: convert it with toupper / tolower
return copy
*/

/*
equalsIgnoreCase

return toLower(a) == toLower(b)
*/

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



