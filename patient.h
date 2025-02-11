#ifndef PATIENT_H
#define PATIENT_H

#include <stdio.h>
#include <cstring>
#include <cctype>

class manageDoctors;

struct Patient
{
    int id;
    char name[50];
    int age;
    char contact[15];
    char diagnosis[100];
    Patient* next;

    Patient() : id(0), age(0), next(nullptr)
    {
        name[0] = '\0';
        contact[0] = '\0';
        diagnosis[0] = '\0';
    }
};

struct BSTNode
{
    Patient* patientData;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Patient* patient) : patientData(patient), left(nullptr), right(nullptr) {}
};

class managePatients
{
    friend class manageDoctors;
    friend class manageAppointments;

private:
    Patient* head;
    BSTNode* root;

    void insertBST(BSTNode*& node, Patient* patient);
    BSTNode* searchBST(BSTNode* node, int id);
    void deleteBST(BSTNode*& node, int id);
    void cleanBST(BSTNode* node);
    void quickSort(Patient* start, Patient* end);

public:
    managePatients();
    ~managePatients();
    managePatients(const managePatients& other) = delete;
    managePatients& operator=(const managePatients& other) = delete;

    void addPatient();
    void editPatient();
    void deletePatient();
    void searchPatient();
    void displayAllPatients();
    void sortPatients();

    void saveToFile();
    void loadFromFile();
    bool valUserInput(const char* str, int type);
    void clearScreen();
};

#endif
