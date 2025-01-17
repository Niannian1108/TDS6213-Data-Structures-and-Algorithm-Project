#include "patient.h"

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

void managePatients::addPatient()
{
    Patient* newPatient = new Patient();

    try
    {
        clearScreen();
        std::cout << "\n=== Add New Patient ===\n";

        //gets patient details and valudates information
        do
        {
            std::cout << "Enter Patient ID: ";
            std::cin >>newPatient->id;
        } while (newPatient->id <= 0);  //makes sure ID is positive

        std::cin.ignore();
        do
        {
            std::cout << "Enter Patient Name: ";
            std::cin.getline (newPatient->name,50);
        } while (!valUserInput (newPatient->name, 2));  //type 2 is for alphabetic

        do
        {
            std::cout <<"Enter Age: ";
            std::cin >> newPatient->age;
        } while (newPatient->age <= 0 || newPatient->age > 120);    //restricts age range

        std::cin.ignore();
        do
        {
            std::cout << "Enter Contact Number: ";
            std::cin.getline (newPatient->contact, 15);
        } while (!valUserInput ( newPatient->contact, 1));    //type 1 for numeric

        std::cout << "Enter Diagnosis: ";
        std::cin.getline(newPatient->diagnosis, 100);

        //add into linked list at the beginning
        newPatient->next =head;
        head  = newPatient;

        //add to bst, searches more efficiently
        insertBST (root,newPatient);

        std::cout << "\nPatient added successfully!\n";
        saveToFile();   //make changes into local file
    }
    catch(...)  //if any error
    {
        std::cout << "Error adding patient. Please try again.\n";
        delete newPatient;
    }
}

void managePatients::editPatient()
{
    if (!head)
    {
        std::cout <<"No patients in the system.\n"; //if no pt recs
        return;
    }

    //uses pt id to search record
    int searchId;
    std::cout <<"Enter Patient ID to edit: ";
    std::cin >> searchId;

    BSTNode*result = searchBST(root, searchId);
    if (!result)    //if no rec found
    {
        std::cout <<"Patient not found.\n";
        return;
    }

    //shows pt recorda
    Patient* patient= result->patientData;
    std::cout << "\nCurrent Patient Details:\n";
    std::cout << "1. Name: " << patient->name << "\n";
    std::cout << "2. Age: " << patient->age <<  "\n";
    std::cout << "3. Contact: " << patient->contact << "\n";
    std::cout << "4. Diagnosis: " <<patient->diagnosis << "\n";
    std::cout << "5. Cancel Edit\n";

    //choes which field to edit instead of retyping the whole record
    int choice;
    std::cout << "\nEnter field number to edit: ";
    std::cin >> choice;
    std::cin.ignore();

    try
    {
        switch(choice)
        {
            case 1:
                do
                {
                    std::cout <<"Enter new name: ";
                    std::cin.getline(patient->name, 50);
                } while (!valUserInput(patient->name, 2));
                break;

            case 2:
                do
                {
                    std::cout << "Enter new age: ";
                    std::cin >> patient->age;
                } while (patient->age <= 0 || patient->age > 150);
                break;

            case 3:
                do
                {
                    std::cout << "Enter new contact: ";
                    std::cin.getline(patient->contact, 15);
                } while (!valUserInput(patient->contact, 1));
                break;

            case 4:
                std::cout << "Enter new diagnosis: ";
                std::cin.getline(patient->diagnosis, 100);
                break;

            case 5:
                std::cout << "Edit cancelled.\n";
                return;

            default:
                std::cout <<"Invalid choice.\n";
                return;
        }
        std::cout << "Patient record updated successfully!\n";
        saveToFile();
    }

    catch (...)
    {
        std::cout << "Error updating patient information.\n";
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

void managePatients::displayAllPatients()
{
    if(!head)
    {
        std::cout <<"No patients in the system.\n";
        return;
    }

    clearScreen();
    std::cout << "\n=== All Patients ===\n";
    std::cout << "ID\tName\t\tAge\tContact\t\tDiagnosis\n";
    std::cout << "--------------------------------------------------------\n";
    //PLEASE DO NOT EDIT THIS HEADER******

    Patient*current =head;
    while (current != nullptr)
    {
        std::cout << current->id << "\t"
                 << current->name << "\t\t"
                 << current->age << "\t"
                 << current->contact << "\t"
                 << current->diagnosis << "\n";
        current = current->next;
    }
    std::cout << "--------------------------------------------------------\n";
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
void managePatients::saveToFile()
{
    std::ofstream file("patients.txt");
    if(!file)
    {
        std::cout<< "Error opening file for writing.\n";
        return;
    }

    Patient* current = head;
    while (current)
    {
        file << current->id << "\n"
             << current->name << "\n"
             << current->age << "\n"
             << current->contact << "\n"
             << current->diagnosis << "\n";
        current =current ->next;
    }
    file.close();
}

void managePatients::loadFromFile()
{
    std::ifstream file ("patients.txt");
    if(!file) return;

    //clears current existing data
    while (head!= nullptr)
    {
        Patient*temp = head;
        head = head -> next;
        delete temp;
    }
    cleanBST(root);
    root = nullptr;

    while (true)
    {
        Patient*newPatient =  new Patient();

        if (!(file >>newPatient->id))
        {
            delete newPatient;
            break;
        }

        file.ignore();
        file.getline (newPatient->name, 50);
        file >> newPatient ->age;
        file.ignore();
        file.getline(newPatient ->contact, 15);
        file.getline(newPatient ->diagnosis,100);

        //add into linked list
        newPatient->next = nullptr;
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
    file.close();
}

bool managePatients::valUserInput(const char* str, int type)
{
    if(!str[0]) return false;

    for (int i = 0; str[i];i++)
    {
        if (type== 1)
        {  // Numeric
            if (!isdigit(str[i])) return false;
        }
    else if (type == 2)   //alphabetics
        {
            if (!isalpha(str[i])&& str[i] != ' ') return false;
        }
    return true;
    }
}

void managePatients::clearScreen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
