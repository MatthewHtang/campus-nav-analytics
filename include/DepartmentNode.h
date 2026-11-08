// Author: Sankalp (Member C)
// Purpose: One department with a parent and any number of children.

#ifndef DEPARTMENTNODE_H
#define DEPARTMENTNODE_H

#include <string>
#include <vector>
using namespace std;

struct DepartmentNode
{
    string name;                     // spelling from the first accepted record
    DepartmentNode* parent;               // nullptr for the root
    int depth;                            // edges from the root (root = 0)
    vector<DepartmentNode*> children; // siblings in insertion order

    // Create a node; depth is one more than its parent's (0 for the root).
    DepartmentNode(const string& departmentName, DepartmentNode* parentNode)
        : name(departmentName),
          parent(parentNode),
          depth(parentNode ? parentNode->depth + 1 : 0)
    {
    }
};

#endif
