// Author: Sankalp (Member C)
// Purpose: Load, add, search, and display departments in a general tree.

#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H

#include <string>
#include <vector>

#include "DepartmentNode.h"
using namespace std;

class DepartmentTree
{
public:
    DepartmentTree(); // start empty
    ~DepartmentTree(); // deletes every node
    // Copying raw pointers would make two trees delete the same nodes.
    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;

    // Replace the tree from departments.txt and print warnings and a summary.
    void loadDepartments(const string& dataDir);

    // Use NONE for the first parent. Return false with a reason on failure.
    bool addDepartment(const string& name, const string& parentName,
                       string& reason);

    // Menu option 6: ask for a parent only when the tree already has a root.
    void addDepartmentFromInput();

    bool isEmpty() const;       // true when there is no root
    int departmentCount() const; // number of accepted departments

    // Show the name, parent, child count, and depth.
    void searchDepartment(const string& name) const;
    void displayHierarchy() const; // preorder, 4 spaces of indent per depth
    void displayPreorder() const;  // preorder sequence, no indentation

    // ---- Final-phase features (not in the M1 menu) -----------------------
    void displayPostorder() const;
    void displayLevelOrder() const; // uses a queue
    void displayStatistics() const; // total, leaves, height

private:
    DepartmentNode* root;
    vector<DepartmentNode*> nodes; // every node in insertion order (owner)

    void clear(); // delete all nodes
    DepartmentNode* findNode(const string& name) const; // ignore case
    void printIndented(const DepartmentNode* node) const;
    void collectPreorder(const DepartmentNode* node,
                         vector<string>& out) const;
    void collectPostorder(const DepartmentNode* node,
                          vector<string>& out) const;
    void printSequence(const string& title,
                       const vector<string>& names) const;
};

#endif
