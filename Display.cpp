#ifndef NODE_H

#include "B+ Tree.h"

void BP_Tree::display(Node* cursor)
{
    /* Depth-First Search */
    if (cursor != NULL)
    {
        for (int i = 0; i < cursor->keys.size(); i++)
        {
            cout << cursor->keys[i] << " ";
        }

        cout << endl;

        if (cursor->isLeaf != true)
        {
            for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++)
            {
                display(cursor->ptr2TreeOrData.ptr2Tree[i]);
            }
        }
    }
}

void BP_Tree::seqDisplay(Node* cursor)
{
    Node* firstLeft = firstLeftNode(cursor);

    if (firstLeft == NULL)
    {
        cout << "No data in the database yet!" << endl;
        return;
    }

    while (firstLeft != NULL)
    {
        for (int i = 0; i < firstLeft->keys.size(); i++)
        {
            cout << firstLeft->keys[i] << " ";
        }

        firstLeft = firstLeft->ptr2Next;
    }

    cout << endl;
}

#endif