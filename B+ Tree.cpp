#include "B+ Tree.h"

#define _CRT_SECURE_NO_DEPRECATE // For VS 2019

void insertionMethod(BP_Tree** bpTree)
{
    int rollNum, age, marks;
    string name;

    cout << "Please provide the rollNum: ";
    cin >> rollNum;

    cout << "\nWhat's the name, age and marks acquired?: ";
    cin >> name >> age >> marks;

    string fileName = "DBFiles/";
    fileName += to_string(rollNum) + ".txt";
    FILE* filePtr = fopen(fileName.c_str(), "w");
    string userTuple = name + " " + to_string(age) + " " + to_string(marks) + "\n";
    fprintf(filePtr, userTuple.c_str());
    //fclose(filePtr);

    (*bpTree)->insert(rollNum, filePtr);
    fclose(filePtr);
}

void searchMethod(BP_Tree* bpTree)
{
    int rollNum;
    cout << "What's the rollNum to search?: ";
    cin >> rollNum;

    bpTree->search(rollNum);
}

void printMethod(BP_Tree* bpTree)
{
    int opt;
    cout << "Press \n\t1.Hierarchical-Display \n\t2.Sequential-Display\n";
    cin >> opt;

    cout << "\nHere is your file structure" << endl;

    if (opt == 1)
    {
        bpTree->display(bpTree->getRoot());
    }
    else 
    {
        bpTree->seqDisplay(bpTree->getRoot());
    }
}

int main()
{
    cout << "\n***Welcome to DATABASE SERVER***\n" << endl;

    bool flag = true;
    int option;

    int maxChildInt = 3,
        maxNodeLeaf = 3;
    cout << "Please provide the value to limit maximum child internal nodes can have: ";
    cin >> maxChildInt;
    cout << "\nNext, provide the value to limit maximum nodes leaf nodes can have: ";
    cin >> maxNodeLeaf;

    BP_Tree* bpTree = new BP_Tree(maxChildInt, maxNodeLeaf);

    do 
    {
        cout << "\nPlease provide the queries with respective keys: " << endl;

        cout << "\tPress 1: Insertion \n\tPress 2: Search \n\tPress 3: Print Tree \n\tPress 4: ABORT!" << endl;
        cin >> option;

        switch (option)
        {
            case 1:
                insertionMethod(&bpTree);
                break;

            case 2:
                searchMethod(bpTree);
                break;

            case 3:
                printMethod(bpTree);
                break;

            default:
                flag = false;
                break;
        }
    } while (flag);

    return 0;
}