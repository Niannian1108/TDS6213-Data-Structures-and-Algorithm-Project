#include "patient.h"
#include <iostream>

using namespace std;

//constructor to initialise empty structures and loads all existing data from local file
managePatients::managePatients() : head(nullptr), root(nullptr)
{
    loadFromFile();
}

//destructor to clean all dynamically allocated mem
managePatients::~managePatients()
{
    //cleans linked list
    while (head != nullptr)
    {
        Patient* temp = head;
        head = head->next;
        delete temp;
    }
    //cleans bst
    cleanBST(root);
}

void managePatients::cleanBST(BSTNode* node)  //deletes bst nodes
{
    if (node)
    {
        cleanBST(node->left);
        cleanBST(node->right);
        delete node;        //cleans left, right subtree and then current node
    }
}

void managePatients::addPatient() {
    Patient* newPatient = new Patient();

    try {
        clearScreen();
        printf("\n=== Add New Patient ===\n");

        do {
            printf("Enter Patient ID: ");
            scanf("%d", &newPatient->id);
            getchar(); // Clear input buffer
        } while (newPatient->id <= 0);

        do {
            printf("Enter Patient Name: ");
            fgets(newPatient->name, 50, stdin);
            newPatient->name[strcspn(newPatient->name, "\n")] = 0;
        } while (!valUserInput(newPatient->name, 2));

        do {
            printf("Enter Age: ");
            scanf("%d", &newPatient->age);
            getchar(); // Clear input buffer
        } while (newPatient->age <= 0 || newPatient->age > 120);

        do {
            printf("Enter Contact Number: ");
            fgets(newPatient->contact, 15, stdin);
            newPatient->contact[strcspn(newPatient->contact, "\n")] = 0;
        } while (!valUserInput(newPatient->contact, 1));

        printf("Enter Diagnosis: ");
        fgets(newPatient->diagnosis, 100, stdin);
        newPatient->diagnosis[strcspn(newPatient->diagnosis, "\n")] = 0;

        newPatient->next = head;
        head = newPatient;
        insertBST(root, newPatient);

        printf("\nPatient added successfully!\n");
        saveToFile();
    }
    catch(...) {
        printf("Error adding patient. Please try again.\n");
        delete newPatient;
    }
}

void managePatients::editPatient() {
    if (!head) {
        printf("No patients in the system.\n");
        return;
    }

    int searchId;
    printf("Enter Patient ID to edit: ");
    scanf("%d", &searchId);
    getchar(); // Clear input buffer

    BSTNode* result = searchBST(root, searchId);
    if (!result) {
        printf("Patient not found.\n");
        return;
    }

    Patient* patient = result->patientData;
    printf("\nCurrent Patient Details:\n");
    printf("1. Name: %s\n", patient->name);
    printf("2. Age: %d\n", patient->age);
    printf("3. Contact: %s\n", patient->contact);
    printf("4. Diagnosis: %s\n", patient->diagnosis);
    printf("5. Cancel Edit\n");

    int choice;
    printf("\nEnter field number to edit: ");
    scanf("%d", &choice);
    getchar(); // Clear input buffer

    try {
        switch(choice) {
            case 1:
                do {
                    printf("Enter new name: ");
                    fgets(patient->name, 50, stdin);
                    patient->name[strcspn(patient->name, "\n")] = 0;
                } while (!valUserInput(patient->name, 2));
                break;

            case 2:
                do {
                    printf("Enter new age: ");
                    scanf("%d", &patient->age);
                    getchar(); // Clear input buffer
                } while (patient->age <= 0 || patient->age > 150);
                break;

            case 3:
                do {
                    printf("Enter new contact: ");
                    fgets(patient->contact, 15, stdin);
                    patient->contact[strcspn(patient->contact, "\n")] = 0;
                } while (!valUserInput(patient->contact, 1));
                break;

            case 4:
                printf("Enter new diagnosis: ");
                fgets(patient->diagnosis, 100, stdin);
                patient->diagnosis[strcspn(patient->diagnosis, "\n")] = 0;
                break;

            case 5:
                printf("Edit cancelled.\n");
                return;

            default:
                printf("Invalid choice.\n");
                return;
        }
        printf("Patient record updated successfully!\n");
        saveToFile();
    }
    catch (...) {
        printf("Error updating patient information.\n");
    }
}

void managePatients::deletePatient()
{
    if (!head)
    {
        std::cout << "No patients in the system.\n";
        return;
    }

    int deleteId;
    std::cout << "Enter Patient ID to delete: ";
    std::cin >> deleteId;

    try
    {
        //search for pt record in linked list
        Patient* current = head;
        Patient* prev = nullptr;
        bool found =false;

        while (current != nullptr)
        {
            if (current->id ==deleteId)
            {
                found = true;
                break;
            }
            prev =current;
            current = current->next;
        }

        if(!found)
        {
            std::cout << "Patient not found.\n";
            return;
        }

        //removes rec from linked list
        if (prev ==nullptr)
        {
            head = current->next;
        }

        else
        {
            prev->next =current->next;
        }

        //remove from bst
        deleteBST(root, deleteId);

        delete current;
        std::cout<< "Patient deleted successfully!\n";
        saveToFile();   //saves changes into local file
    }

    catch(...)
    {
        std::cout << "Error deleting patient.\n";
    }
}

//delete bst node
void managePatients::deleteBST(BSTNode*& node, int id)
{
    if(!node) return;

    if(id< node->patientData->id)
        deleteBST(node->left, id);

    else if (id > node->patientData->id)
        deleteBST(node->right, id);

    else
    {
        //once finds node, delets it
        if (!node->left)    //if no left child
        {
            BSTNode* temp = node->right;
            delete node;
            node = temp;
        }
        else if(!node->right)   //if no right childe
        {
            BSTNode* temp = node->left;
            delete node;
            node = temp;
        }
        else
        {
            //if node has 2 child, will find successor which is in the right subtree
            BSTNode*temp = node->right;
            while(temp->left)
                temp = temp->left;
            node->patientData =temp->patientData;
            deleteBST(node->right,temp->patientData->id);
        }
    }
}

void managePatients::displayAllPatients() {
    if(!head) {
        printf("No patients in the system.\n");
        return;
    }

    clearScreen();
    printf("\n=== All Patients ===\n");
    printf("ID\tName\t\tAge\tContact\t\tDiagnosis\n");
    printf("--------------------------------------------------------\n");

    Patient* current = head;
    while (current != nullptr) {
        printf("%d\t%s\t\t%d\t%s\t%s\n",
               current->id, current->name, current->age,
               current->contact, current->diagnosis);
        current = current->next;
    }
    printf("--------------------------------------------------------\n");
}

void managePatients::insertBST(BSTNode*& node,Patient*patient)
{
    if(!node)   //creates new node if the current node is null
    {
        node =new BSTNode(patient);
        return;
    }

    if(patient->id < node->patientData->id)
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
        std::cout << "No patients in the system.\n";
        return;
    }

    int searchId;
    std::cout << "Enter Patient ID to search: ";
    std::cin >> searchId;

    try
    {
        BSTNode*  result= searchBST(root, searchId);
        if (result && result->patientData)
        {
            std::cout << "\nPatient Found:\n";
            std::cout << "ID: " << result->patientData->id << "\n";
            std::cout << "Name: " << result->patientData->name << "\n";
            std::cout << "Age: " <<  result->patientData->age << "\n";
            std::cout << "Contact: " << result->patientData->contact<< "\n";
            std::cout << "Diagnosis: " << result->patientData->diagnosis << "\n";
        }

        else
        {
            std::cout << "Patient not found.\n";
        }
    }
    catch(...)
    {
        std::cout  << "Error occurred while searching.\n";
    }
}


BSTNode* managePatients::searchBST(BSTNode* node, int id)
{
    if(!node || !node->patientData) return nullptr;

    if (node->patientData->id == id) return node;

    if (id <node->patientData->id)
        return searchBST(node->left, id);
    return searchBST(node->right, id);
}

// Replace the existing sortPatients() in patient.cpp with this implementation:

void managePatients::quickSort(Patient* start, Patient* end) {
    if (start == end || start == nullptr || end == nullptr || start == end->next)
        return;

    // Choose pivot as last element
    Patient* pivot = end;
    Patient* current = start;
    Patient* tail = start;

    while (current != end) {
        if (current->id < pivot->id) {
            // Swap current and tail if needed
            if (current != tail) {
                std::swap(current->id, tail->id);
                std::swap(current->age, tail->age);

                char temp[100];
                strcpy(temp, current->name);
                strcpy(current->name, tail->name);
                strcpy(tail->name, temp);

                strcpy(temp, current->contact);
                strcpy(current->contact, tail->contact);
                strcpy(tail->contact, temp);

                strcpy(temp, current->diagnosis);
                strcpy(current->diagnosis, tail->diagnosis);
                strcpy(tail->diagnosis, temp);
            }
            tail = tail->next;
        }
        current = current->next;
    }

    // Place pivot in correct position
    if (tail != pivot) {
        std::swap(tail->id, pivot->id);
        std::swap(tail->age, pivot->age);

        char temp[100];
        strcpy(temp, tail->name);
        strcpy(tail->name, pivot->name);
        strcpy(pivot->name, temp);

        strcpy(temp, tail->contact);
        strcpy(tail->contact, pivot->contact);
        strcpy(pivot->contact, temp);

        strcpy(temp, tail->diagnosis);
        strcpy(tail->diagnosis, pivot->diagnosis);
        strcpy(pivot->diagnosis, temp);
    }

    // Recursively sort sub-lists
    if (start != tail)
        quickSort(start, tail);
    if (tail->next != end)
        quickSort(tail->next, end);
}

void managePatients::sortPatients() {
    if (!head || !head->next) return;

    // Find last node
    Patient* last = head;
    while (last->next != nullptr) {
        last = last->next;
    }

    // Call quickSort
    quickSort(head, last);
    std::cout << "Patients sorted successfully using QuickSort!\n";
}

void managePatients::saveToFile() {
    FILE* file = fopen("patients.txt", "w");
    if(!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    Patient* current = head;
    while (current) {
        fprintf(file, "%d\n%s\n%d\n%s\n%s\n",
                current->id, current->name, current->age,
                current->contact, current->diagnosis);
        current = current->next;
    }
    fclose(file);
}

void managePatients::loadFromFile() {
    FILE* file = fopen("patients.txt", "r");
    if(!file) return;

    // Clear existing data
    while (head != nullptr) {
        Patient* temp = head;
        head = head->next;
        delete temp;
    }
    cleanBST(root);
    root = nullptr;

    char buffer[100];
    while (!feof(file)) {
        Patient* newPatient = new Patient();

        if (fscanf(file, "%d\n", &newPatient->id) != 1) {
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

        // Add to linked list
        newPatient->next = nullptr;
        if (!head) {
            head = newPatient;
        } else {
            Patient* current = head;
            while (current->next != nullptr) {
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
    if(!str[0]) return false;

    for (int i = 0; str[i]; i++)
    {
        if (type == 1)  // Numeric
        {
            if (!isdigit(str[i])) return false;
        }
        else if (type == 2)   //alphabetic
        {
            if (!isalpha(str[i]) && str[i] != ' ') return false;
        }
    }
    return true;  // Move return true outside the loop
}

void managePatients::clearScreen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
