/*
 * @Author: Sankalp (Member C)
 * @Date: 2026-11-06
 * @Last Modified by: Sankalp (Member C)
 *
 * Purpose: Implements DepartmentTree: loading and validating departments.txt,
 *   adding and searching departments, the indented hierarchy display, and the
 *   preorder, postorder and level-order traversals with department
 *   statistics. Uses trim, equalsIgnoreCase, readRecords and printWarning
 *   from Utils.
 */

#include "DepartmentTree.h"
#include "Utils.h"

#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

// Create an empty tree.
DepartmentTree::DepartmentTree() : root(nullptr)
{
}

// Delete every node (the vector owns them all).
DepartmentTree::~DepartmentTree()
{
    for (size_t i = 0; i < nodes.size(); i++)
    {
        delete nodes[i];
    }
    nodes.clear();
    root = nullptr;
}

// ---------------------------------------------------------------------------
// Loading and adding
// ---------------------------------------------------------------------------

// Load departments.txt: the first valid record is the root (parent NONE);
// every later record needs a parent accepted on an earlier line.
void DepartmentTree::loadDepartments(const string& dataDir)
{
    const string filename = "departments.txt";
    vector<Record> records;
    int rejected = 0;
    readRecords(dataDir + "/" + filename, filename, 2, records, rejected);

    int accepted = 0;
    for (size_t i = 0; i < records.size(); i++)
    {
        string reason;
        if (addDepartment(records[i].fields[0], records[i].fields[1], reason))
        {
            accepted++;
        }
        else
        {
            printWarning(filename, records[i].lineNumber, reason);
            rejected++;
        }
    }
    cout << filename << ": " << accepted << " accepted, "
         << rejected << " rejected" << endl;
}

// Add a department (file or menu). Rejects blank or duplicate names, a first
// department whose parent is not NONE, and unknown parents.
bool DepartmentTree::addDepartment(const string& name, const string& parentName,
                                   string& reason)
{
    string cleanName = trim(name);
    string cleanParent = trim(parentName);

    if (cleanName.empty())
    {
        reason = "department name is empty";
        return false;
    }

    DepartmentNode* existing = findNode(cleanName);
    if (existing != nullptr)
    {
        reason = "duplicate department \"" + cleanName +
                 "\" (already loaded as \"" + existing->name + "\")";
        return false;
    }

    // The first department is the root and must use NONE as its parent.
    if (root == nullptr)
    {
        if (!equalsIgnoreCase(cleanParent, "NONE"))
        {
            reason = "the first department must be the root with parent NONE, "
                     "not \"" + cleanParent + "\"";
            return false;
        }
        root = new DepartmentNode(cleanName, nullptr);
        nodes.push_back(root);
        return true;
    }

    if (cleanParent.empty())
    {
        reason = "parent name is empty";
        return false;
    }
    DepartmentNode* parent = findNode(cleanParent);
    if (parent == nullptr)
    {
        if (equalsIgnoreCase(cleanParent, "NONE"))
        {
            reason = "only the first department can use NONE as its parent";
        }
        else
        {
            reason = "unknown parent \"" + cleanParent + "\"";
        }
        return false;
    }

    DepartmentNode* node = new DepartmentNode(cleanName, parent);
    parent->children.push_back(node); // siblings stay in insertion order
    nodes.push_back(node);
    return true;
}

// Find a department by name, ignoring case and outer whitespace.
DepartmentNode* DepartmentTree::findNode(const string& name) const
{
    string target = trim(name);
    if (target.empty())
    {
        return nullptr;
    }
    for (size_t i = 0; i < nodes.size(); i++)
    {
        if (equalsIgnoreCase(nodes[i]->name, target))
        {
            return nodes[i];
        }
    }
    return nullptr;
}

// True when no department is loaded.
bool DepartmentTree::isEmpty() const
{
    return root == nullptr;
}

// Total number of departments.
int DepartmentTree::departmentCount() const
{
    return static_cast<int>(nodes.size());
}

// ---------------------------------------------------------------------------
// Search and display
// ---------------------------------------------------------------------------

// Show name, parent (None for the root), direct-child count and depth.
void DepartmentTree::searchDepartment(const string& name) const
{
    if (isEmpty())
    {
        cout << "No departments are loaded." << endl;
        return;
    }
    string target = trim(name);
    if (target.empty())
    {
        cout << "Error: department name cannot be blank." << endl;
        return;
    }
    const DepartmentNode* node = findNode(target);
    if (node == nullptr)
    {
        cout << "Department \"" << target << "\" was not found." << endl;
        return;
    }
    cout << "Department: " << node->name << endl;
    cout << "Parent: " << (node->parent ? node->parent->name : string("None")) << endl;
    cout << "Direct children: " << node->children.size() << endl;
    cout << "Depth: " << node->depth << endl;
}

// Print one node and then its children, indented 4 spaces per depth (preorder).
void DepartmentTree::printIndented(const DepartmentNode* node) const
{
    cout << string(static_cast<size_t>(node->depth) * 4, ' ') << node->name << endl;
    for (size_t i = 0; i < node->children.size(); i++)
    {
        printIndented(node->children[i]);
    }
}

// Show the whole hierarchy, one department per line.
void DepartmentTree::displayHierarchy() const
{
    if (isEmpty())
    {
        cout << "No departments are loaded." << endl;
        return;
    }
    printIndented(root);
}

// Print a titled traversal sequence on one line, joined with " -> ".
void DepartmentTree::printSequence(const string& title,
                                   const vector<string>& names) const
{
    cout << title << ": ";
    for (size_t i = 0; i < names.size(); i++)
    {
        cout << (i > 0 ? " -> " : "") << names[i];
    }
    cout << endl;
}

// Preorder: a node, then each child subtree in insertion order.
void DepartmentTree::collectPreorder(const DepartmentNode* node,
                                     vector<string>& out) const
{
    out.push_back(node->name);
    for (size_t i = 0; i < node->children.size(); i++)
    {
        collectPreorder(node->children[i], out);
    }
}

// Show the preorder sequence without indentation.
void DepartmentTree::displayPreorder() const
{
    if (isEmpty())
    {
        cout << "No departments are loaded." << endl;
        return;
    }
    vector<string> order;
    collectPreorder(root, order);
    printSequence("Preorder", order);
}

// ---------------------------------------------------------------------------
// Final-phase traversals and statistics
// ---------------------------------------------------------------------------

// Postorder: each child subtree in insertion order, then the node.
void DepartmentTree::collectPostorder(const DepartmentNode* node,
                                      vector<string>& out) const
{
    for (size_t i = 0; i < node->children.size(); i++)
    {
        collectPostorder(node->children[i], out);
    }
    out.push_back(node->name);
}

// Show the postorder sequence.
void DepartmentTree::displayPostorder() const
{
    if (isEmpty())
    {
        cout << "No departments are loaded." << endl;
        return;
    }
    vector<string> order;
    collectPostorder(root, order);
    printSequence("Postorder", order);
}

// Level-order (breadth-first) with a queue; siblings in insertion order.
void DepartmentTree::displayLevelOrder() const
{
    if (isEmpty())
    {
        cout << "No departments are loaded." << endl;
        return;
    }
    vector<string> order;
    queue<const DepartmentNode*> pending;
    pending.push(root);
    while (!pending.empty())
    {
        const DepartmentNode* current = pending.front();
        pending.pop();
        order.push_back(current->name);
        for (size_t i = 0; i < current->children.size(); i++)
        {
            pending.push(current->children[i]);
        }
    }
    printSequence("Level-order", order);
}

// Total departments, leaf count and height (N/A for an empty tree).
void DepartmentTree::displayStatistics() const
{
    if (isEmpty())
    {
        cout << "No departments are loaded." << endl;
        cout << "Total departments: 0" << endl;
        cout << "Leaf departments: 0" << endl;
        cout << "Tree height: N/A" << endl;
        return;
    }
    int leaves = 0;
    int height = 0;
    for (size_t i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->children.empty())
        {
            leaves++;
        }
        if (nodes[i]->depth > height)
        {
            height = nodes[i]->depth;
        }
    }
    cout << "Total departments: " << nodes.size() << endl;
    cout << "Leaf departments: " << leaves << endl;
    cout << "Tree height: " << height << endl;
}