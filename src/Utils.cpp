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
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string trim(string text)
{
    const string whitespace = " \t\r\n";
    const size_t first = text.find_first_not_of(whitespace);

    if (first == string::npos)
    {
        return "";
    }

    const size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

vector<string> split(string text, char delimiter)
{
    vector<string> fields;
    string field;

    for (size_t i = 0; i < text.size(); ++i)
    {
        if (text[i] == delimiter)
        {
            fields.push_back(field);
            field.clear();
        }
        else
        {
            field += text[i];
        }
    }

    // Preserve the last field, including an empty trailing field.
    fields.push_back(field);
    return fields;
}

string toUpper(string text)
{
    for (size_t i = 0; i < text.size(); ++i)
    {
        text[i] = static_cast<char>(
            toupper(static_cast<unsigned char>(text[i])));
    }

    return text;
}

string toLower(string text)
{
    for (size_t i = 0; i < text.size(); ++i)
    {
        text[i] = static_cast<char>(
            tolower(static_cast<unsigned char>(text[i])));
    }

    return text;
}

bool equalsIgnoreCase(string a, string b)
{
    return toLower(a) == toLower(b);
}

string normalizeId(const string& id)
{
    return toUpper(trim(id));
}

bool isValidId(string id)
{
    if (id.size() != 4 || id[0] != 'F')
    {
        return false;
    }

    for (size_t i = 1; i < id.size(); ++i)
    {
        if (id[i] < '0' || id[i] > '9')
        {
            return false;
        }
    }

    return true;
}

void printWarning(
    const string& filename,
    int lineNumber,
    const string& reason)
{
    cout << "Warning: " << filename << ", line " << lineNumber
         << ": " << reason << " - record rejected\n";
}

void printLoadSummary(
    const string& filename,
    int acceptedCount,
    int rejectedCount)
{
    cout << "Load summary: " << filename << ": "
         << acceptedCount << " accepted, "
         << rejectedCount << " rejected\n";
}

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
        cout << "Error: cannot open file \"" << filename << "\" ("
             << path << "). Treating it as empty.\n";
        return false;
    }

    int lineNumber = 0;
    string line;

    while (getline(file, line))
    {
        ++lineNumber;

        // Remove a UTF-8 byte-order mark if present at the start of the file.
        if (lineNumber == 1 &&
            line.compare(0, 3, "\xEF\xBB\xBF") == 0)
        {
            line.erase(0, 3);
        }

        line = trim(line);

        // A comment starts only when # is the first non-whitespace character.
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        vector<string> fields = split(line, '|');

        for (size_t i = 0; i < fields.size(); ++i)
        {
            fields[i] = trim(fields[i]);
        }

        if (static_cast<int>(fields.size()) != expectedFieldCount)
        {
            printWarning(
                filename,
                lineNumber,
                "wrong number of fields (expected " +
                    to_string(expectedFieldCount) + ", found " +
                    to_string(fields.size()) + ")");
            ++rejectedCount;
            continue;
        }

        size_t emptyField = fields.size();

        for (size_t i = 0; i < fields.size(); ++i)
        {
            if (fields[i].empty())
            {
                emptyField = i;
                break;
            }
        }

        if (emptyField != fields.size())
        {
            printWarning(
                filename,
                lineNumber,
                "field " + to_string(emptyField + 1) + " is empty");
            ++rejectedCount;
            continue;
        }

        Record record;
        record.lineNumber = lineNumber;
        record.fields = fields;
        records.push_back(record);
    }

    return true;
}

string formatPercent(int count, int total)
{
    if (total <= 0)
    {
        return "0.0%";
    }

    ostringstream output;
    output << fixed << setprecision(1)
           << (100.0 * count / total) << '%';

    return output.str();
}

string promptLine(const string& prompt)
{
    cout << prompt;
    cout.flush();

    string answer;

    if (!getline(cin, answer))
    {
        return "";
    }

    return trim(answer);
}

int readMenuChoice(
    const string& prompt,
    int minChoice,
    int maxChoice,
    int eofChoice)
{
    if (minChoice < 0 || maxChoice < minChoice)
    {
        cout << "Error: invalid menu choice range.\n";
        return -1;
    }

    cout << prompt;
    cout.flush();

    string input;

    if (!getline(cin, input))
    {
        return eofChoice;
    }

    input = trim(input);

    if (input.empty())
    {
        cout << "Error: enter a menu choice.\n";
        return -1;
    }

    int value = 0;
    bool tooLarge = false;

    for (size_t i = 0; i < input.size(); ++i)
    {
        const char character = input[i];

        if (character < '0' || character > '9')
        {
            cout << "Error: enter a number from "
                 << minChoice << " to " << maxChoice << ".\n";
            return -1;
        }

        const int digit = character - '0';

        if (!tooLarge)
        {
            if (value >
                (numeric_limits<int>::max() - digit) / 10)
            {
                tooLarge = true;
            }
            else
            {
                value = value * 10 + digit;
            }
        }
    }

    if (tooLarge || value < minChoice || value > maxChoice)
    {
        cout << "Error: enter a number from "
             << minChoice << " to " << maxChoice << ".\n";
        return -1;
    }

    return value;
}