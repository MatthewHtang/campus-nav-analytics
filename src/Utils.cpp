/*
 * @Author: @MatthewHtang
 * @Date: 2026-11-05 10:07:02
 * @last Modified by: Sankalp (Member C)
 * @Last Modified time: 2026-11-06 10:07:24
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
 *
 * Credit: trim, split, toUpper, toLower and equalsIgnoreCase by Matthew Htang
 * (Member A). isValidId and readRecords completed by Sankalp (Member C).
 * Change to Matthew's code: trim() now takes "string text" so that it matches
 * its declaration in Utils.h (it was "const string&", which left the declared
 * trim(string) undefined at link time).
 */

#include "Utils.h"

#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Whitespace characters: space, tab, newline and carriage return.
static const string WHITESPACE = " \t\n\r";

// ---------------------------------------------------------------------------
// Text helpers
// ---------------------------------------------------------------------------

// Remove whitespace from the beginning of s.
static string leftTrim(const string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    if (start == string::npos)
    {
        return ""; // all whitespace
    }
    return s.substr(start);
}

// Remove whitespace from the end of s.
static string rightTrim(const string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    if (end == string::npos)
    {
        return ""; // all whitespace
    }
    return s.substr(0, end + 1);
}

//Remove WHITESPACES from both side
//This return the fully trimed of a string
//First, left trim first and then right trim, then return
//The parameter is "string text" to match the declaration in Utils.h
string trim(string text){
    return rightTrim(leftTrim(text));
}

// Split on every delimiter and keep the last field, even if empty.
vector<string> split(string text, char delimiter)
{
    vector<string> result;
    string current;
    for (char c : text)
    {
        if (c == delimiter)
        {
            result.push_back(current);
            current.clear();
        }
        else
        {
            current += c;
        }
    }
    result.push_back(current);
    return result;
}

// Uppercase copy of text.
string toUpper(string text)
{
    for (char& c : text)
    {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }
    return text;
}

// Lowercase copy of text.
string toLower(string text)
{
    for (char& c : text)
    {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return text;
}

// True if a and b match while ignoring capitalization.
bool equalsIgnoreCase(string a, string b)
{
    return toLower(a) == toLower(b);
}

//isValidId expects an ID that is already trimmed and uppercased.
//A valid ID is the letter F followed by exactly three digits, like F001.
//Callers must run trim() and toUpper() first, so "f001" is rejected here.
bool isValidId(string id){
    //length must be exactly 4 and the first character must be 'F'
    if (id.size() != 4 || id[0] != 'F'){
        return false;
    }

    //characters 1 to 3 must all be digits
    for (size_t i = 1; i < 4; i++){
        if (id[i] < '0' || id[i] > '9'){
            return false;
        }
    }
    return true;
}

//Print one rejection warning: filename, line number and the reason.
//Only used inside this file, so it is not declared in Utils.h.
static void printRejection(const string& filename, int lineNumber, const string& reason){
    cout << "Warning: " << filename << ", line " << lineNumber << ": "
         << reason << " - record rejected" << endl;
}

//readRecords is the shared filter used to load all four data files.
//- Returns false (after printing an error naming the file) if it cannot be
//  opened; the caller then treats the file as empty.
//- Blank lines and comment lines (first non-space character is '#') are
//  skipped. A '#' later in a line is ordinary text, like "Room #5".
//- Each line is read whole with getline, trimmed (this also removes the '\r'
//  of Windows line endings) and split on '|'; every field is trimmed.
//- A line with the wrong number of fields or an empty field is rejected with
//  a warning (filename, line number, reason) and counted in rejectedCount.
//  rejectedCount is added to, not reset, so the caller should start it at 0.
//- Each accepted line is appended to records with its real line number.
bool readRecords(
    string path,
    string filename,
    int expectedFieldCount,
    vector<Record>& records,
    int& rejectedCount)
{
    ifstream file(path.c_str());
    if (!file.is_open()){
        cout << "Error: cannot open file \"" << filename << "\" (" << path
             << "). Treating it as empty." << endl;
        return false;
    }

    int lineNumber = 0;
    string line;
    while (getline(file, line)){
        lineNumber += 1;

        //Drop a UTF-8 byte-order mark if an editor added one to the first line
        if (lineNumber == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0){
            line.erase(0, 3);
        }

        line = trim(line);
        if (line.empty() || line[0] == '#'){
            continue;
        }

        vector<string> fields = split(line, '|');
        for (size_t i = 0; i < fields.size(); i++){
            fields[i] = trim(fields[i]);
        }

        //Wrong number of fields
        if (static_cast<int>(fields.size()) != expectedFieldCount){
            printRejection(filename, lineNumber,
                           "wrong number of fields (expected " +
                           to_string(expectedFieldCount) + ", found " +
                           to_string(fields.size()) + ")");
            rejectedCount += 1;
            continue;
        }

        //Any empty required field
        bool hasEmptyField = false;
        size_t emptyPosition = 0;
        for (size_t i = 0; i < fields.size(); i++){
            if (fields[i].empty()){
                hasEmptyField = true;
                emptyPosition = i + 1;
                break;
            }
        }
        if (hasEmptyField){
            printRejection(filename, lineNumber,
                           "field " + to_string(emptyPosition) + " is empty");
            rejectedCount += 1;
            continue;
        }

        Record record;
        record.lineNumber = lineNumber;
        record.fields = fields;
        records.push_back(record);
    }
    return true;
}