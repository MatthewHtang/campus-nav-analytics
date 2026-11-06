/*
 * @Author: Matthew Htang
 * @Date: 2026-11-05 10:08:45
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

// Kept on purpose: other headers and sources in the project rely on it.
using namespace std;

// One accepted line of a data file: its real line number and trimmed fields.
struct Record
{
    int lineNumber;
    vector<string> fields;
};

// ---------------------------------------------------------------------------
// Text helpers
// ---------------------------------------------------------------------------

// Remove space, tab, \r and \n from both ends of text (inner spaces kept).
string trim(string text);

// Split text on every delimiter; "A|B|" gives 3 fields, "" gives 1 field.
vector<string> split(string text, char delimiter);

// Return a copy of text with all letters converted to uppercase.
string toUpper(string text);

// Return a copy of text with all letters converted to lowercase.
string toLower(string text);

// True if a and b are equal when capitalization is ignored (inner spaces
// still matter: "Student Union" != "Student  Union").
bool equalsIgnoreCase(string a, string b);

// ---------------------------------------------------------------------------
// Facility IDs (Member A)
// ---------------------------------------------------------------------------

// Normalize an ID from a file or from the user: trim, then uppercase.
// Always call this BEFORE isValidId, hashing, inserting or comparing.
string normalizeId(const string &id);

// True if id is 'F' followed by exactly three digits. Expects an ID that is
// already normalized, so "f001" is rejected here.
bool isValidId(string id);

// ---------------------------------------------------------------------------
// File loading (all members)
// ---------------------------------------------------------------------------

// Print one rejection warning in the shared format:
//   Warning: <filename>, line <n>: <reason> - record rejected
// Loaders call this for every record they reject.
void printWarning(const string &filename, int lineNumber, const string &reason);

// Read one data file. Returns false (after printing an error naming the
// file) if it cannot be opened; the caller then treats the file as empty.
// Skips blank lines and comment lines (first non-space character is '#'),
// trims every field, and rejects lines with the wrong number of fields or an
// empty field with a printWarning. rejectedCount is added to (not reset) for
// each such rejection. Accepted lines are appended to records.
bool readRecords(
    string path,
    string filename,
    int expectedFieldCount,
    vector<Record> &records,
    int &rejectedCount);

// ---------------------------------------------------------------------------
// Reports (Member A)
// ---------------------------------------------------------------------------

// Percentage of count out of total with one decimal place, e.g. "20.0%".
// Returns "0.0%" when total is 0 (never divides by zero).
string formatPercent(int count, int total);

// ---------------------------------------------------------------------------
// Menu and prompt input (main.cpp and every module that prompts)
// ---------------------------------------------------------------------------

// Print prompt, read one whole line (names may contain spaces) and return it
// trimmed. Returns "" for a blank line or at end of input, so callers treat
// "" as a blank name and print their error.
string promptLine(const string &prompt);

// Print prompt and read one menu choice as a whole line. Accepts only digits
// whose value is between minChoice and maxChoice (minChoice must be >= 0).
// For blank, text or out-of-range input it prints an error and returns -1;
// the caller just shows the menu again. At end of input it returns eofChoice
// (pass the Exit option) so piped or scripted runs can never loop forever.
int readMenuChoice(const string &prompt, int minChoice, int maxChoice,
                   int eofChoice);

#endif