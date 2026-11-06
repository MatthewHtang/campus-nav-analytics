/*
 * @Author: Matthew Htang
 * @Date: 2026-11-05 10:08:45
 * @Last Modified by: Sankalp (Member C)
 * @Last Modified time: 2026-11-06
 *
 * Purpose: Declares the shared helpers used by every module: text cleaning
 *   (trim, split, case conversion and comparison), ID checks, the common file
 *   reader and warning printer used by all four loaders, a percentage
 *   formatter for the category report, and menu input helpers that recover
 *   from blank, text and out-of-range input.
 *
 * Credit: trim, split, toUpper, toLower, equalsIgnoreCase and the original
 *   function list by Matthew Htang (Member A); completed by Sankalp (Member C).
 */

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

using namespace std;

// One accepted data-file line, including its real line number.
struct Record
{
    int lineNumber;
    vector<string> fields;
};

// Text helpers. Inner spaces are preserved.
string trim(string text);
vector<string> split(string text, char delimiter);
string toUpper(string text);
string toLower(string text);
bool equalsIgnoreCase(string a, string b);

// Normalize IDs before validating, hashing, inserting, or comparing.
string normalizeId(const string& id);

// Expects an already normalized uppercase ID: F followed by three digits.
bool isValidId(string id);

// Shared file-warning and load-summary output.
void printWarning(
    const string& filename,
    int lineNumber,
    const string& reason);

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