#include "B+ Tree.h"

void BP_Tree::insert(int key, FILE* filePtr) // In leaf node
{
    /*
        1. If the node has an empty space, insert the key/reference pair into the node.
        2. If the node is already full, split it into two nodes, distributing the keys evenly between the two nodes.
            If the node is a leaf, take a copy of the minimum value in the second of these two nodes and repeat this insertion algorithm to insert it into the parent node.
            If the node is a non-leaf, exclude the middle value during the split and repeat this insertion algorithm to insert this excluded value into the parent node.
     */
    if (root == NULL)
    {
        root = new Node;
        root->isLeaf = true;
        root->keys.push_back(key);
        new (&root->ptr2TreeOrData.dataPtr) std::vector<FILE*>;

        // Now root->ptr2TreeOrData.dataPtr is the active member of the union
        root->ptr2TreeOrData.dataPtr.push_back(filePtr);

        cout << key << "ROOT!!" << endl;
        return;
    }
    else 
    {
        Node* cursor = root;
        Node* parent = NULL;

        // Searching for the possible position for the given key by doing the same procedure done in search
        while (cursor->isLeaf == false)
        {
            parent = cursor;
            int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
        }

        // The cursor is now the leaf node in which we'll insert the new key
        if (cursor->keys.size() < maxLeafNodeLimit)
        {
            // If the leaf node is not full, find the correct position for the new key and insert!
            int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor->keys.push_back(key);
            cursor->ptr2TreeOrData.dataPtr.push_back(filePtr);

            if (i != cursor->keys.size() - 1)
            {
                for (int j = cursor->keys.size() - 1; j < i; j--) // Shifting the position for keys and data-pointer
                {
                    cursor->keys[j] = cursor->keys[j - 1];
                    cursor->ptr2TreeOrData.dataPtr[j] = cursor->ptr2TreeOrData.dataPtr[j - 1];
                }

                // Insert 
                cursor->keys[i] = key;
                cursor->ptr2TreeOrData.dataPtr[i] = filePtr;
            }

            cout << "Inserted successfully: " << key << endl;
        }
        else 
        {
            // Node overflowed, split it!
            vector<int> virtualNode(cursor->keys);
            vector<FILE*> virtualDataNode(cursor->ptr2TreeOrData.dataPtr);

            // Finding the probable place to insert the key
            int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();

            virtualNode.push_back(key); // To create space
            virtualDataNode.push_back(filePtr); // ...also to create space

            if (i != virtualNode.size() - 1)
            {
                for (int j = virtualNode.size() - 1; j > i; j--) // Shift position of keys and data-pointer
                {
                    virtualNode[j] = virtualNode[j - 1];
                    virtualDataNode[j] = virtualDataNode[j - 1];
                }

                // Inserting
                virtualNode[i] = key;
                virtualDataNode[i] = filePtr;
            }

            // Create new leaf
            Node* newLeaf = new Node;
            newLeaf->isLeaf = true;
            new (&newLeaf->ptr2TreeOrData.dataPtr) std::vector<FILE*>;
            // Now newLeaf->ptr2TreeOrData.ptr2Tree is the active member of the union

            // Swapping the next ptr 
            Node* temp = cursor->ptr2Next;
            cursor->ptr2Next = newLeaf;
            newLeaf->ptr2Next = temp;

            // Resizing and copying the keys and dataPtr to old node 
            cursor->keys.resize((maxLeafNodeLimit) / 2 + 1); // Check for +1 while partitioning
            cursor->ptr2TreeOrData.dataPtr.reserve((maxLeafNodeLimit) / 2 + 1);

            for (int i = 0; i <= (maxLeafNodeLimit) / 2; i++)
            {
                cursor->keys[i] = virtualNode[i];
                cursor->ptr2TreeOrData.dataPtr[i] = virtualDataNode[i];
            }

            // Pushing new keys and dataPtr to new node
            for (int i = (maxLeafNodeLimit) / 2 + 1; i < virtualNode.size(); i++)
            {
                newLeaf->keys.push_back(virtualNode[i]);
                newLeaf->ptr2TreeOrData.dataPtr.push_back(virtualDataNode[i]);
            }

            if (cursor == root)
            {
                // If cursor is root node, create new node
                Node* newRoot = new Node;
                newRoot->keys.push_back(newLeaf->keys[0]);
                new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
                newRoot->ptr2TreeOrData.ptr2Tree.push_back(cursor);
                newRoot->ptr2TreeOrData.ptr2Tree.push_back(newLeaf);
                root = newRoot;
                cout << "Create new root!" << endl;
            }
            else 
            {
                // Insert new key into the parent
                insertInternal(newLeaf->keys[0], &parent, &newLeaf);
            }
        }
    }
}

void BP_Tree::insertInternal(int x, Node** cursor, Node** child) // In internal nodes
{
    if ((*cursor)->keys.size() < maxIntChildLimit - 1)
    {
        // If cursor is not full find the position for the new key
        int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();
        (*cursor)->keys.push_back(x);
        //new (&(*cursor)->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
        // Now root->ptr2TreeOrData.ptr2Tree is the active member of the union
        (*cursor)->ptr2TreeOrData.ptr2Tree.push_back(*child);

        if (i != (*cursor)->keys.size() - 1) // If there are more than one element
        {
            // Different loops because size is different for both 

            for (int j = (*cursor)->keys.size() - 1; j > i; j--) // Shifting the position for keys and data-pointer
            {
                (*cursor)->keys[j] = (*cursor)->keys[j - 1];
            }

            for (int j = (*cursor)->ptr2TreeOrData.ptr2Tree.size() - 1; j > (i + 1); j--)
            {
                (*cursor)->ptr2TreeOrData.ptr2Tree[j] = (*cursor)->ptr2TreeOrData.ptr2Tree[j - 1];
            }

            (*cursor)->keys[i] = x;
            (*cursor)->ptr2TreeOrData.ptr2Tree[i + 1] = *child;
        }

        cout << "Inserted key in the internal node" << endl;
    }
    else // Splitting
    {
        cout << "Inserted node in internal node successful" << endl;
        cout << "Overflow in internal, splitting internal nodes" << endl;

        vector<int> virtualKeyNode((*cursor)->keys);
        vector<Node*> virtualTreePtrNode((*cursor)->ptr2TreeOrData.ptr2Tree);

        int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin(); // Finding the position for x
        virtualKeyNode.push_back(x); // To create space
        virtualTreePtrNode.push_back(*child); // ...also to create space

        if (i != virtualKeyNode.size() - 1)
        {
            for (int j = virtualKeyNode.size() - 1; j > i; j--) // Shifting position for keys and data-pointer
            {
                virtualKeyNode[j] = virtualKeyNode[j - 1];
            }

            for (int j = virtualTreePtrNode.size() - 1; j > (i + 1); j--)
            {
                virtualTreePtrNode[j] = virtualTreePtrNode[j - 1];
            }

            virtualKeyNode[i] = x;
            virtualTreePtrNode[i + 1] = *child;
        }

        int partitionKey; // Exclude middle element while splitting
        partitionKey = virtualKeyNode[(virtualKeyNode.size() / 2)]; // Right biased
        int partitionIdx = (virtualKeyNode.size() / 2);

        // Resizing and copying the keys and tree ptr to old node
        (*cursor)->keys.resize(partitionIdx);
        (*cursor)->ptr2TreeOrData.ptr2Tree.reserve(partitionIdx + 1);

        for (int i = 0; i < partitionIdx; i++)
        {
            (*cursor)->keys[i] = virtualKeyNode[i];
        }

        for (int i = 0; i < partitionIdx + 1; i++)
        {
            (*cursor)->ptr2TreeOrData.ptr2Tree[i] = virtualTreePtrNode[i];
        }

        Node* newInternalNode = new Node;
        new (&newInternalNode->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
        // Pushing new keys and tree ptr to new node

        for (int i = partitionIdx + 1; i < virtualKeyNode.size(); i++)
        {
            newInternalNode->keys.push_back(virtualKeyNode[i]);
        }

        for (int i = partitionIdx + 1; i < virtualTreePtrNode.size(); i++) // Because only key is excluded, not the pointer
        {
            newInternalNode->ptr2TreeOrData.ptr2Tree.push_back(virtualTreePtrNode[i]);
        }

        if ((*cursor) == root)
        {
            // If cursor is a root, create a new node 
            Node* newRoot = new Node;
            newRoot->keys.push_back(partitionKey);
            new (&newRoot->ptr2TreeOrData.ptr2Tree) std::vector<Node*>;
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(*cursor);
            // Now newRoot->ptr2TreeOrData.ptr2Tree is the active member of the union
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(newInternalNode);

            root = newRoot;
            cout << "Created new ROOT!" << endl;
        }
        else 
        {
            /*
                ::Recursion :=
             */
            insertInternal(partitionKey, findParent(root, *cursor), &newInternalNode);
        }
    }
}