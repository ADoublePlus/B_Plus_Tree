#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;

#ifndef NODE_H
#define NODE_H

class Node 
{
    /*
        General size of thie node should be equal to the block size;
            which will limit the number of disk access rights and increase the accessing time.
        Intermediate nodes only hold the tree pointers which is of considerably small size (so they are able to hold more tree pointers)
            and only leaf nodes hold the data pointer directly to the disc.

        IMPORTANT := All data must be present in the leaf node.
     */
    public:
        bool isLeaf;
        vector<int> keys;
        //Node* ptr2Parent; // Pointer to parent node; CANNOT USE, see: https://stackoverflow.com/questions/57831014/why-we-are-not-saving-the-parent-pointer-in-b-tree-for-easy-upward-traversal-in
        Node* ptr2Next; // Pointer to connect next node for leaf nodes

        union ptr // Creates memory efficient node
        {
            vector<Node*> ptr2Tree; // Array of pointers to children sub-trees for intermediate nodes
            vector<FILE*> dataPtr; // Data-Pointer for the leaf node

            ptr();
            ~ptr();
        } ptr2TreeOrData;

        friend class BP_Tree; // To access private members of the node and hold the encapsulation concept

    public:
        Node();
};

class BP_Tree
{
    /*
        ::For Root Node :=
            The root node has atleast two tree pointers.

        ::For Internal Nodes :=
            1. ceil(maxIntChildLimit / 2) <= #of children <= maxIntChildLimit.
            2. ceil(maxIntChildLimit / 2) - 1 <= #of keys <= maxIntChildLimit - 1.

        ::For Leaf Nodes :=
            1. ceil(maxLeafNodeLimit / 2) - 1 <= #of keys <= maxLeafNodeLimit - 1.
     */
    private:
        int maxIntChildLimit; // Limiting #of children for internal nodes 
        int maxLeafNodeLimit; // Limiting #of nodes for leaf nodes 

        Node* root; // Pointer to the B+ Tree root
        void insertInternal(int x, Node** cursor, Node** child); // Insert x from child in cursor(parent)
        Node** findParent(Node* cursor, Node* child);
        Node* firstLeftNode(Node* cursor);

    public:
        BP_Tree();
        BP_Tree(int degreeInternal, int degreeLeaf);

        Node* getRoot();
        void display(Node* cursor);
        void seqDisplay(Node* cursor);
        void search(int key);
        void insert(int key, FILE* filePtr);
        //void remove(int key);
};

#endif