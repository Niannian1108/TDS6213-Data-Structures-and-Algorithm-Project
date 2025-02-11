#include "patient.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cctype>

using namespace std;

//initialise
managePatients::managePatients() : head(nullptr), root(nullptr)
{
    loadFromFile();
}

//destructor, cleans allocated memory
managePatients::~managePatients()
{
    //clear linked list
    while (head != nullptr)
    {
        Patient* temp = head;
        head = head->next;
        delete temp;
    }
    // Clean BST
    cleanBST(root);
}

void managePatients::cleanBST(BSTNode* node)
{
    if (node)
    {
        cleanBST(node->left);
        cleanBST(node->right);
        delete node;  //clean left right subtree and node
    }
}

void managePatients::addPatient()
{
    Patient* newPatient = new Patient();

    try
    {
        clearScreen();
        cout << "\n=== Add New Patient ===\n";

        do
        {
            cout << "Enter Patient ID: ";
            cin >> newPatient->id;
        } while (newPatient->id <= 0);

        cin.ignore();
        do
        {
            cout << "Enter Patient Name: ";
            cin.getline(newPatient->name, 50);
        } while (!valUserInput(newPatient->name, 2));

        do
        {
            cout << "Enter Age: ";
            cin >> newPatient->age;
        } while (newPatient->age <= 0 || newPatient->age > 120);

        cin.ignore();
        do
        {
            cout << "Enter Contact Number: ";
            cin.getline(newPatient->contact, 15);
        } while (!valUserInput(newPatient->contact, 1));

        cout << "Enter Diagnosis: ";
        cin.getline(newPatient->diagnosis, 100);

        newPatient->next = head;
        head = newPatient;
        insertBST(root, newPatient);

        cout << "\nPatient added successfully!\n";
        saveToFile();
    }
    catch(...)
    {
        cout << "Error adding patient. Please try again.\n";
        delete newPatient;
    }
}

void managePatients::editPatient()
{
    if (!head)
    {
        cout << "No patients in the system.\n";
        return;
    }

    int searchId;
    cout << "Enter Patient ID to edit: ";
    cin >> searchId;

    BSTNode* result = searchBST(root, searchId);
    if (!result)
    {
        cout << "Patient not found.\n";
        return;
    }

    Patient* patient = result->patientData;
    cout << "\nCurrent Patient Details:\n";
    cout << "1. Name: " << patient->name << "\n";
    cout << "2. Age: " << patient->age << "\n";
    cout << "3. Contact: " << patient->contact << "\n";
    cout << "4. Diagnosis: " << patient->diagnosis << "\n";
    cout << "5. Cancel Edit\n";

    int choice;
    cout << "\nEnter field number to edit: ";
    cin >> choice;
    cin.ignore();

    try
    {
        switch(choice)
        {
            case 1:
                do
                {
                    cout << "Enter new name: ";
                    cin.getline(patient->name, 50);
                } while (!valUserInput(patient->name, 2));
                break;

            case 2:
                do
                {
                    cout << "Enter new age: ";
                    cin >> patient->age;
                } while (patient->age <= 0|| patient->age > 150);
                break;

            case 3:
                do
                {
                    cout << "Enter new contact: ";
                    cin.getline(patient->contact, 15);
                } while (!valUserInput(patient->contact, 1));
                break;

            case 4:
                cout << "Enter new diagnosis: ";
                cin.getline(patient->diagnosis, 100);
                break;

            case 5:
                cout << "Edit cancelled.\n";
                return;

            default:
                cout << "Invalid choice.\n";
                return;
        }
        cout << "Patient record updated successfully!\n";
        saveToFile();
    }
    catch (...)
    {
        cout << "Error updating patient information.\n";
    }
}

void managePatients::deletePatient()
{
    if (!head)
    {
        cout << "No patients in the system.\n";
        return;
    }

    int deleteId;
    cout << "Enter Patient ID to delete: ";
    cin >> deleteId;

    try
    {
        Patient* current = head;
        Patient* prev = nullptr;
        bool found = false;

        while (current != nullptr)
        {
            if (current->id == deleteId)
            {
                found = true;
                break;
            }
            prev = current;
            current = current->next;
        }

        if (!found)
        {
            cout << "Patient not found.\n";
            return;
        }

        if (prev == nullptr)
        {
            head = current->next;
        }
        else
        {
            prev->next = current->next;
        }

        deleteBST(root, deleteId);

        delete current;
        cout << "Patient deleted successfully!\n";
        saveToFile();
    }
    catch(...)
    {
        cout << "Error deleting patient.\n";
    }
}

void managePatients::deleteBST(BSTNode*& node, int id)
{
    if (!node) return;

    if (id < node->patientData->id)
        deleteBST(node->left, id);
    else if (id > node->patientData->id)
        deleteBST(node->right, id);
    else
    {
        if (!node->left)
        {
            BSTNode* temp = node->right;
            delete node;
            node = temp;
        }
        else if (!node->right)
        {
            BSTNode* temp = node->left;
            delete node;
            node = temp;
        }
        else
        {
            BSTNode* temp = node->right;
            while (temp->left)
                temp = temp->left;
            node->patientData = temp->patientData;
            deleteBST(node->right, temp->patientData->id);
        }
    }
}

void managePatients::displayAllPatients()
{
    if (!head)
    {
        cout << "No patients in the system.\n";
        return;
    }

    clearScreen();
    cout << "\n=== All Patients ===\n";
    cout << "ID\tName\t\tAge\tContact\t\tDiagnosis\n";
    cout << "--------------------------------------------------------\n";

    Patient* current = head;
    while (current != nullptr)
    {
        cout << current->id << "\t"
             << current->name << "\t\t"
             << current->age << "\t"
             << current->contact << "\t"
             << current->diagnosis << "\n";
        current = current->next;
    }
    cout << "--------------------------------------------------------\n";
}

void managePatients::insertBST(BSTNode*& node, Patient* patient)
{
    if (!node)
    {
        node = new BSTNode(patient);
        return;
    }

    if (patient->id < node->patientData->id)
    {
        insertBST(node->left, patient);
    }
    else if (patient->id > node->patientData->id)
    {
        insertBST(node->right, patient);
    }
}

void managePatients::searchPatient()
{
    if (!head)
    {
        cout << "No patients in the system.\n";
        return;
    }

    int searchId;
    cout << "Enter Patient ID to search: ";
    cin >> searchId;

    try
    {
        BSTNode* result = searchBST(root, searchId);
        if (result && result->patientData)
        {
            cout << "\nPatient Found:\n";
            cout << "ID: " << result->patientData->id << "\n";
            cout << "Name: " << result->patientData->name << "\n";
            cout << "Age: " << result->patientData->age << "\n";
            cout << "Contact: " << result->patientData->contact << "\n";
            cout << "Diagnosis: " << result->patientData->diagnosis << "\n";
        }
        else
        {
            cout << "Patient not found.\n";
        }
    }
    catch(...)
    {
        cout << "Error occurred while searching.\n";
    }
}

BSTNode* managePatients::searchBST(BSTNode* node, int id)
{
    if (!node || !node->patientData) return nullptr;

    if (node->patientData->id == id) return node;

    if (id < node->patientData->id)
        return searchBST(node->left, id);
    return searchBST(node->right, id);
}

void managePatients::quickSort(Patient* start, Patient* end)
{
    //base: if start and end are same, one element, or empty, no sorting needed
    if (start == end || start == nullptr || end == nullptr || start == end->next)
        return;

    //find last pt
    Patient* last = start;
    while (last->next != end->next)
        last = last->next;

    //partitioning with pivot
    auto partition = [](Patient* low, Patient* high) -> Patient*
    {
        Patient* pivot = high;  //choose last element as pivot
        Patient* i = nullptr;   //tracks boundary of elem. smaller than the pivot
        Patient* j = low;       //iterated thru the list

        while (j != high)
        {
            //if the current elem. smaller or eq to pivot
            if (j->id <= pivot->id)
            {
                //move i infront
                i = (i == nullptr) ? low : i->next;

                //swapping
                if (i != j)
                {
                    std::swap(i->id, j->id);
                    std::swap(i->age, j->age);

                    //swapping strings
                    char tempName[50];
                    strcpy(tempName, i->name);
                    strcpy(i->name, j->name);
                    strcpy(j->name, tempName);

                    char tempContact[15];
                    strcpy(tempContact, i->contact);
                    strcpy(i->contact, j->contact);
                    strcpy(j->contact, tempContact);

                    char tempDiagnosis[100];
                    strcpy(tempDiagnosis, i->diagnosis);
                    strcpy(i->diagnosis, j->diagnosis);
                    strcpy(j->diagnosis, tempDiagnosis);
                }
            }
            j = j->next;
        }

        //sort pivot
        i = (i == nullptr) ? low : i->next;

        //pivot swap with i
        std::swap(i->id, high->id);
        std::swap(i->age, high->age);

        char tempName[50];
        strcpy(tempName, i->name);
        strcpy(i->name, high->name);
        strcpy(high->name, tempName);

        char tempContact[15];
        strcpy(tempContact, i->contact);
        strcpy(i->contact, high->contact);
        strcpy(high->contact, tempContact);

        char tempDiagnosis[100];
        strcpy(tempDiagnosis, i->diagnosis);
        strcpy(i->diagnosis, high->diagnosis);
        strcpy(high->diagnosis, tempDiagnosis);

        return i;
    };

    auto quickSortInternal = [&](Patient* low, Patient* high)
    {
        if (low != high && low != nullptr && high != nullptr && low != high->next)
        {
            //partitioning and get the pivot point
            Patient* pivotPoint = partition(low, high);

            //locate nodes b4 & after pivot point for recursive call
            Patient* beforePivot = nullptr;
            Patient* current = low;
            while (current != pivotPoint)
            {
                beforePivot = current;
                current = current->next;
            }

            //recursive sort left part
            if (beforePivot != nullptr)
                quickSort(low, beforePivot);

            //find start of right part
            Patient* afterPivot = pivotPoint;
            while (afterPivot->next != high->next)
                afterPivot = afterPivot->next;

            //recursive sort right part
            if (afterPivot->next != high)
                quickSort(afterPivot->next, high);
        }
    };

    quickSortInternal(start, end);
}

void managePatients::sortPatients()
{
    if (!head || !head->next) return;

    Patient* last = head;
    while (last->next != nullptr)
    {
        last = last->next;
    }

    quickSort(head, last);
    cout << "Patients sorted successfully!\n";
}

void managePatients::saveToFile()
{
    FILE* file = fopen("patients.txt", "w");
    if (!file)
    {
        cout << "Error opening file for writing.\n";
        return;
    }

    Patient* current = head;
    while (current)
    {
        fprintf(file, "%d\n%s\n%d\n%s\n%s\n",
                current->id, current->name, current->age,
                current->contact, current->diagnosis);
        current = current->next;
    }   //write into file and update ptr
    fclose(file);
}

void managePatients::loadFromFile()
{
    FILE* file = fopen("patients.txt", "r");
    if (!file) return;

    while (head != nullptr)
    {
        Patient* temp = head;
        head = head->next;
        delete temp;
    }
    cleanBST(root);
    root = nullptr;

    while (true)
    {
        Patient* newPatient = new Patient();

        if (fscanf(file, "%d\n", &newPatient->id) != 1)
        {
            delete newPatient;
            break;
        }

        fgets(newPatient->name, 50, file);
        newPatient->name[strcspn(newPatient->name, "\n")] = 0;

        fscanf(file, "%d\n", &newPatient->age);

        fgets(newPatient->contact, 15, file);
        newPatient->contact[strcspn(newPatient->contact, "\n")] = 0;

        fgets(newPatient->diagnosis, 100, file);
        newPatient->diagnosis[strcspn(newPatient->diagnosis, "\n")] = 0;

        newPatient->next = nullptr;
        //get from file

        if (!head)
        {
            head = newPatient;
        }
        else
        {
            Patient* current = head;
            while (current->next != nullptr)
            {
                current = current->next;
            }
            current->next = newPatient;
        }

        insertBST(root, newPatient);
    }
    fclose(file);
}

bool managePatients::valUserInput(const char* str, int type)
{
    if (!str[0]) return false;

    for (int i = 0; str[i]; i++)    //valideat 1 >> numbers 2 >> alphabets
    {
        if (type == 1)
        {
            if (!isdigit(str[i])) return false;
        }
        else if (type == 2)
        {
            if (!isalpha(str[i]) && str[i] != ' ') return false;
        }
    }
    return true;
}

void managePatients::clearScreen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
