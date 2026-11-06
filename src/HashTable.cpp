/*
 * @Author: @MatthewHtang
 * @Date: 2026-11-06 12:17:43
 * @Last Modified time: 2026-11-06 12:17:43
 *
 * This file is the actual implementation of the Hash Table and its functions
 * Five main functions include: the constructor, hashFunction(), insert(), find(), and the getters.
 */
#include "HashTable.h"
#include "Utils.h"
#include <iostream>
#include <string>

using namespace std;

// Called struct(hashtable) -> constructor(hashtable)
HashTable::HashTable(int capacity)
{
    // And initialize them
    this->capacity = capacity;
    this->collisionCount = 0; // No collision encounter yet
    this->entryCount = 0;     // No entries count yet

    // if the table is empty
    if (this->capacity < 1)
    {
        this->capacity = 1; // the table needs atleast one bucket
    }
    buckets.resize(this->capacity); // resize the bucket
}
// The hash function: h = (h * 31 + c) % capacity.
// HashTable -> hashfunction -> implement the function
// This hash function takes a key (facility id) then
//  convert it into a bucket number

int HashTable::hashFunction(const string &key) const
{
    int h = 0; // initial starting index to 0

    // for loop to check all the character and compute the h
    for (char c : key)
    {
        h = (h * 31 + c) % capacity;
    }
    return h;
}

// Insert a key into a hash table
bool HashTable::insert(const string &key, int index)
{

    // first convert the key into uppercase and store it in normalized
    string normalized = toUpper(key); // "f101" -> "F101"

    // Get that normalized facility id's index and store it into b
    int b = hashFunction(normalized); // eg, b = 3

    // Go through every entry inside the selected bucket, one at a time,
    // without modifying it.
    //  This look confusing, but this first go to that nested vector array
    //  and check if any of those the same with what we are going to add,
    //  (which is store in normalized)
    for (int i = 0; i < buckets[b].size(); i++)
    {
        if (buckets[b][i].key == normalized)
        {
            return false; // if that place is already occupied, return false
        }
    }
    // if the bucket is not empty, and already occupied
    //  count the collision
    if (!buckets[b].empty())
    {
        collisionCount++;
    }

    // Create a new entry and add it to the bucket index where it belongs
    //  normalized-> key(facility id), and index -> bucket index
    buckets[b].push_back({normalized, index});

    entryCount++; // Now we count the entry
    return true;
}

// This function find whether it exist or not
bool HashTable::find(const string &key) const
{

    // Convert the key(f101) into capital letter
    string normalized = toUpper(key);

    // Get that bucket index number
    int b = hashFunction(normalized);

    // for loop to get the key(facility id) and compare it with the
    // the input, eg(F010 = F010?)-> return its index
    for (int i = 0; i < buckets[b].size(); i++)
    {
        if (buckets[b][i].key == normalized)
        {
            return buckets[b][i].index;
        }
    }
    // if not found return -1
    return -1;
}

// Function to get capacity
int HashTable::getCapacity() const
{
    return capacity;
}

// Function to get entry count
int HashTable::getEntryCount() const
{
    return entryCount;
}

// Function to get collision count
int HashTable::getCollisionCount() const
{
    return collisionCount;
}
