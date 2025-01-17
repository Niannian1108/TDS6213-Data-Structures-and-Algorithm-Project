#ifndef PATIENT_H
#define PATIENT_H

#include <iostream>
#include <fstream>
#include <cstring>

//stores patient information
struct Patient
{
    int id;
    char name[50];
    int age;
    char contact[15];
    char diagnosis[100];
    Patient* next;  //pointer to next pt in the list

    //constructor for initialising pt data
    Patient() : id(0), age(0),next(nullptr)
    {
        name[0] = '\0';
        contact[0] = '\0';
        diagnosis[0] = '\0';
    }
};

//binary search tree node structure for ID-based searching
//each node points to one data
struct BSTNode
{
    Patient* patientData;
    BSTNode* left;
    BSTNode* right;

    //creats new node
    BSTNode(Patient* patient) : patientData(patient), left(nullptr), right(nullptr) {}
};

//manages pt records
class managePatients
{
private:
    Patient* head;
    BSTNode* root;

    //functions to help with the bst op
    void insertBST(BSTNode*& node, Patient*patient);
    BSTNode*searchBST(BSTNode* node, int id);
    void deleteBST(BSTNode*& node,int id);
    void cleanBST(BSTNode* node);     //cleans bst memory, unnecessary data

public:
    managePatients();
    ~managePatients();      //constructor and destructot

    //create read update and delete operations
    void addPatient();
    void editPatient();
    void deletePatient();
    void searchPatient();
    void displayAllPatients();
    void sortPatients();

    //input to local file, where we can access this file by adding, editing or deldeting pt recs
    void saveToFile();      //save into local file
    void loadFromFile();    //load from local file, can get previous saved pt records.

    //util functions
    bool valUserInput(const char* str, int type);  // validates user input: numeric and alphabetic
    void clearScreen();
};

#endif
