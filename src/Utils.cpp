/*
 * @Author: Matthew Htang
 * @Date: 2026-11-05 10:07:02
 * @Last Modified by: Sankalp (Member C)
 * @Last Modified time: 2026-11-06
 *
 * Purpose: Implements the shared helpers declared in Utils.h: text cleaning,
 *   ID normalization and validation, the common data-file reader and warning
 *   printer, the percentage formatter, and menu input helpers.
 *
 * Credit: trim (leftTrim/rightTrim), split, toUpper, toLower and
 *   equalsIgnoreCase by Matthew Htang (Member A); everything else and the
 *   final version by Sankalp (Member C). Changes to Matthew's code: trim()
 *   now matches its declaration in Utils.h (it was "const string&", which
 *   left the declared trim(string) undefined at link time), and the case
 *   functions cast to unsigned char so non-ASCII bytes are safe.
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

// Remove whitespace from both ends; the parameter matches Utils.h.
string trim(string text)
{
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

// ---------------------------------------------------------------------------
// Facility IDs
// ---------------------------------------------------------------------------

// Trim and uppercase an ID so every later step sees the same spelling.
string normalizeId(const string& id)
{
    return toUpper(trim(id));
}

// A valid ID is 'F' plus exactly three digits (input already normalized).
bool isValidId(string id)
{
    if (id.size() != 4 || id[0] != 'F')
    {
        return false;
    }
    for (size_t i = 1; i < 4; i++)
    {
        if (id[i] < '0' || id[i] > '9')
        {
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// File loading
// ---------------------------------------------------------------------------

// Print a rejection warning with filename, line number and reason.
void printWarning(const string& filename, int lineNumber, const string& reason)
{
    cout << "Warning: " << filename << ", line " << lineNumber << ": "
         << reason << " - record rejected" << endl;
}

// Read a data file line by line (see Utils.h for the full contract).
bool readRecords(
    string path,
    string filename,
    int expectedFieldCount,
    vector<Record>& records,
    int& rejectedCount)
{
    ifstream file(path.c_str());
    if (!file.is_open())
    {
        cout << "Error: cannot open file \"" << filename << "\" (" << path
             << "). Treating it as empty." << endl;
        return false;
    }

    int lineNumber = 0;
    string line;
    while (getline(file, line))
    {
        lineNumber++;

        // Drop a UTF-8 byte-order mark if an editor added one to line 1.
        if (lineNumber == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0)
        {
            line.erase(0, 3);
        }

        line = trim(line); // also removes the '\r' of Windows line endings
        if (line.empty() || line[0] == '#')
        {
            continue; // blank line or comment line
        }

        vector<string> fields = split(line, '|');
        for (size_t i = 0; i < fields.size(); i++)
        {
            fields[i] = trim(fields[i]);
        }

        if (static_cast<int>(fields.size()) != expectedFieldCount)
        {
            printWarning(filename, lineNumber,
                         "wrong number of fields (expected " +
                         to_string(expectedFieldCount) + ", found " +
                         to_string(fields.size()) + ")");
            rejectedCount++;
            continue;
        }

        size_t emptyPosition = 0;
        for (size_t i = 0; i < fields.size(); i++)
        {
            if (fields[i].empty())
            {
                emptyPosition = i + 1;
                break;
            }
        }
        if (emptyPosition != 0)
        {
            printWarning(filename, lineNumber,
                         "field " + to_string(emptyPosition) + " is empty");
            rejectedCount++;
            continue;
        }

        Record record;
        record.lineNumber = lineNumber;
        record.fields = fields;
        records.push_back(record);
    }
    return true;
}

// ---------------------------------------------------------------------------
// Reports
// ---------------------------------------------------------------------------

// "20.0%" style text; 0.0% when total is 0 so we never divide by zero.
string formatPercent(int count, int total)
{
    double percent = (total == 0) ? 0.0 : 100.0 * count / total;
    ostringstream out;
    out << fixed << setprecision(1) << percent << "%";
    return out.str();
}

// ---------------------------------------------------------------------------
// Menu and prompt input
// ---------------------------------------------------------------------------

// Read one whole line after a prompt; "" for blank input or end of input.
string promptLine(const string& prompt)
{
    cout << prompt;
    if (cin.fail() && !cin.eof())
    {
        cin.clear(); // recover from an earlier failed numeric read
    }
    string line;
    if (!getline(cin, line))
    {
        cout << endl; // finish the prompt line at end of input
        return "";
    }
    return trim(line);
}

// Read one menu choice; -1 after an error message, eofChoice at end of input.
int readMenuChoice(const string& prompt, int minChoice, int maxChoice,
                   int eofChoice)
{
    cout << prompt;
    if (cin.fail() && !cin.eof())
    {
        cin.clear(); // recover from an earlier failed numeric read
    }
    string line;
    if (!getline(cin, line))
    {
        cout << endl; // finish the prompt line at end of input
        return eofChoice;
    }
    line = trim(line);

    // Digits only (no sign, no letters) and short enough not to overflow.
    bool valid = !line.empty() && line.size() <= 9;
    int value = 0;
    for (size_t i = 0; valid && i < line.size(); i++)
    {
        if (line[i] < '0' || line[i] > '9')
        {
            valid = false;
        }
        else
        {
            value = value * 10 + (line[i] - '0');
        }
    }
    if (valid && value >= minChoice && value <= maxChoice)
    {
        return value;
    }

    cout << "Invalid choice. Please enter a number from " << minChoice
         << " to " << maxChoice << "." << endl;
    return -1;
}