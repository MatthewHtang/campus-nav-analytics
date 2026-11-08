/*
 * @Author: @MatthewHtang
 * @Date: 2026-11-06 11:23:01
 * @Last Modified By: @MatthewHtang
 * @Last Modified time: 2026-11-06 11:23:53
 *
 * This file is a blue print for hash table, hash function, insert, find, getCapacity,
 * getCollision and getCount
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
using namespace std;

// We choose our starting number of a bucket to be 11, because a prime number
// spreads those keys(facility id) more evenly
// So initiale it with a fix number
const int INITIAL_CAPACITY = 11;

// Struct an entry that has a bucket index and key whhich is the facility ID (F102)
struct Entry
{
    string key;
    int index;
};

// Hash table
class HashTable
{
private:
    // We declare a vector of buckets and each bucket hold a vector of Entry object
    // We use nested vector because to avoid collision, we have to use sperate chaining method
    std::vector<std::vector<Entry>> buckets;

    int capacity;       // Total number of buckets
    int entryCount;     // Total number of entries currently stored
    int collisionCount; // Number of insertion into non empty buckets

    // This function takes a key (facility id) and converts a that key into a bucket number from 0 to -1
    int hashFunction(const std::string &key) const;

public:
    // Constructor: create a hashtable with the given capacity
    // And it initially use its initial capacity (11) if it was not provided

    HashTable(int capacity = INITIAL_CAPACITY); // capacity = 11

    // This inserts Facility Id and its corresponding vector index
    //  And returns full if the insertion is sucessfull and false if the ID already existed
    bool insert(const std::string &key, int index);

    // This searches for Facility ID and return its vector index
    // Returns -1 if Id is not found
    // We are not modifying anything so (const)
    int find(const std::string &key) const; // This line data type changed from bool -> int after fixing bugs (Matthew)

    // This is our Getters, it returns information about our hashtable
    // And we do not need to modify it

    int getCapacity() const;
    int getEntryCount() const;
    int getCollisionCount() const;
};

#endif
