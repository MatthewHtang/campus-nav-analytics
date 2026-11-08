// Authors: Matthew Htang (original text helpers), Sankalp (shared helpers).
// Purpose: Handle whitespace, file validation, and whole-line input.

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

// Remove whitespace at both ends, including Windows carriage returns.
string trim(string text)
{
    const string whitespace = " \t\r\n\f\v";
    const size_t first = text.find_first_not_of(whitespace);

    if (first == string::npos)
    {
        return "";
    }

    const size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

// Keep empty fields so the loader can reject missing values.
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

// Cast to unsigned char before using the character functions.
string toUpper(string text)
{
    for (size_t i = 0; i < text.size(); ++i)
    {
        text[i] = static_cast<char>(
            toupper(static_cast<unsigned char>(text[i])));
    }

    return text;
}

// Return a lowercase copy without changing the original text.
string toLower(string text)
{
    for (size_t i = 0; i < text.size(); ++i)
    {
        text[i] = static_cast<char>(
            tolower(static_cast<unsigned char>(text[i])));
    }

    return text;
}

// Spaces inside names still matter.
bool equalsIgnoreCase(string a, string b)
{
    return toLower(a) == toLower(b);
}

// Always normalize an ID before checking or hashing it.
string normalizeId(const string& id)
{
    return toUpper(trim(id));
}