#include "B+ Tree.h"

Node::ptr::ptr() {}
Node::ptr::~ptr() {}

Node::Node()
{
    this->isLeaf = false;
    this->ptr2Next = NULL;
}

BP_Tree::BP_Tree()
{
    /*
        By default it will take the maxIntChildLimit as 4 and maxLeafNodeLimit as 3.

        ::REASON FOR TWO SEPARATE VARIABLES maxIntChildLimit & maxLeafNodeLimit :=
            We are keeping the two separate orders because internal nodes can hold more values in one disc block
            as the size of the tree pointer is small but the size of the data pointer in the leaf node is large
            so we can only put less nodes in the leafs as compared to the internal nodes.
     */
    this->maxIntChildLimit = 4;
    this->maxLeafNodeLimit = 3;
    
    this->root = NULL;
}

BP_Tree::BP_Tree(int degreeInternal, int degreeLeaf)
{
    this->maxIntChildLimit = degreeInternal;
    this->maxLeafNodeLimit = degreeLeaf;

    this->root = NULL;
}

Node* BP_Tree::getRoot() { return this->root; }

Node* BP_Tree::firstLeftNode(Node* cursor)
{
    if (cursor->isLeaf)
        return cursor;

    for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++)
    {
        if (cursor->ptr2TreeOrData.ptr2Tree[i] != NULL)
            return firstLeftNode(cursor->ptr2TreeOrData.ptr2Tree[i]);
    }

    return NULL;
}

Node** BP_Tree::findParent(Node* cursor, Node* child)
{
    /*
        Finds parent using depth-first traversal and ignores leaf nodes as they cannot be parents,
            also ignores second last level because we will never find parent of a leaf node during insertion using this function.
     */
    Node* parent = NULL;

    if (cursor->isLeaf || cursor->ptr2TreeOrData.ptr2Tree[0]->isLeaf)
        return NULL;

    for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++)
    {
        if (cursor->ptr2TreeOrData.ptr2Tree[i] == child)
        {
            parent = cursor;
            return &parent;
        }
        else 
        {
            new (&cursor->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
            parent = *findParent(cursor->ptr2TreeOrData.ptr2Tree[i], child);
        }
    }

    return &parent;
}