/*
 * @Author: @MatthewHtang
 * @Date: 2026-11-07 23:50:31 
 * @Last Modified time: 2026-11-07 23:50:31 
 */

// Authors: Matthew Htang (original text helpers), Sankalp (shared helpers).


#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

using namespace std;

// One line with valid fields. Each loader still checks names and references.
struct Record
{
    int lineNumber;
    vector<string> fields;
};

// Remove outer whitespace; keep spaces inside the text.
string trim(string text);
// Keep every field, even an empty field after the last delimiter.
vector<string> split(string text, char delimiter);
// Convert letters to uppercase.
string toUpper(string text);
// Convert letters to lowercase.
string toLower(string text);
// Compare letters without case; callers trim names first.
bool equalsIgnoreCase(string a, string b);

// Normalize IDs before validating, hashing, inserting, or comparing.
string normalizeId(const string& id);

// Expects an already normalized uppercase ID: F followed by three digits.
bool isValidId(string id);

// Show the filename, line number, and rejection reason.
void printWarning(
    const string& filename,
    int lineNumber,
    const string& reason);

// Show accepted and rejected counts for one file.
void printLoadSummary(
    const string& filename,
    int acceptedCount,
    int rejectedCount);

// Read and structurally validate records. Rejections are added to
// rejectedCount; accepted records are appended to records.
bool readRecords(
    string path,
    string filename,
    int expectedFieldCount,
    vector<Record>& records,
    int& rejectedCount);

// Return a one-decimal percentage, or 0.0% when total is zero.
string formatPercent(int count, int total);

// Read a whole trimmed line, allowing names with spaces.
string promptLine(const string& prompt);

// Read a numeric menu choice from one complete line.
// Returns -1 for invalid input and eofChoice when input reaches EOF.
int readMenuChoice(
    const string& prompt,
    int minChoice,
    int maxChoice,
    int eofChoice);

#endif
