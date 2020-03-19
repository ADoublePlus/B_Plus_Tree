#include "B+ Tree.h"

void BP_Tree::search(int key)
{
    if (root == NULL)
    {
        cout << "No tuples inserted yet" << endl;
        return;
    }
    else 
    {
        Node* cursor = root;

        while (cursor->isLeaf == false)
        {
            /*
                upper_bound returns an iterator pointing to the first element in the range [first, last], which has a value greater than �val�.
                    (Because we are storing the same value in the right node; STL is doing binary search at backend).
             */
            int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor = cursor->ptr2TreeOrData.ptr2Tree[idx]; // upper_bound takes care of all edge cases
        }

        int idx = std::lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin(); // Binary search 

        if (idx == cursor->keys.size() || cursor->keys[idx] != key)
        {
            cout << "Key not found" << endl;
            return;
        }

        /*
            Fetch the data from the disc in main memory using data-ptr,
                using cursor->dataPtr[idx].
         */
        string fileName = "DBFiles/";
        string data;
        fileName += to_string(key) + ".txt";
        FILE* filePtr = fopen(fileName.c_str(), "r");
        cout << "Key found" << endl;
        cout << "Corresponding tuple data is: ";
        char ch = fgetc(filePtr);

        while (ch != EOF)
        {
            printf("%c", ch);
            ch = fgetc(filePtr);
        }

        fclose(filePtr);
        cout << endl;
    }
}