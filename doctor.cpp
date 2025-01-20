#include "doctor.h"
#include <cstring>
#include <iostream>

using namespace std;

manageDoctors::manageDoctors(managePatients* patientSys)
    :head(nullptr), root(nullptr), patientSystem(patientSys)
{
    loadFromFile();
}

manageDoctors::~manageDoctors()
{   //manageDoctors destructor, to clean uo allocated memory
    while (head !=nullptr)
    {
        Doctor* temp =head;    //current head of list is stored in a temp pointer 'temp'
        head = head->next;      //next node
        delete temp;
    }       //loop until head becomes null, end of list
    cleanBST(root);
}

void manageDoctors::addDoctor()
{
    Doctor* newDoctor = new Doctor();

    try {   //to handle exceptions if anythng occurs during execution
        cout << "\n=== Add New Doctor ===\n";

        do
        {
            cout << "Enter Doctor ID: ";
            cin >> newDoctor->id;
        } while (newDoctor->id <= 0);   //force users to enter a valid doctor id >0, loops until correct

        cin.ignore();   //clear input buffer
        do
        {
            cout << "Enter Doctor Name: ";
            cin.getline(newDoctor->name, 50);
        } while (!valUserInput(newDoctor->name, 2));    //make sure only alphabets and spaces used

        do
        {
            cout << "Enter Specialization: ";
            cin.getline(newDoctor->specialization, 50);
        } while (!valUserInput(newDoctor->specialization, 2));  //make sure only alphabets and spaces used

        do
        {
            cout << "Enter Contact Number: ";
            cin.getline(newDoctor->contact, 15);
        } while (!valUserInput(newDoctor->contact, 1)); //make sure only numbers

        newDoctor->next = head; //new dr added to the beginning of the linked list, next ptr is set to current head
        head = newDoctor;   //updated to point to new dr
        insertBST(root, newDoctor); //insert to binary search tree

        cout << "\nDoctor added successfully!\n";
        saveToFile();
    }
    catch(...) //if any execption occurs, object is deleted
    {
        cout << "Error adding doctor. Please try again.\n";
        delete newDoctor;
    }
}

void manageDoctors::assignPatient()
{
    if (!head)
    {
        cout << "No doctors in the system.\n";
        return;
    }

    int doctorId, patientId;

    cout << "Enter Doctor ID: ";
    cin >> doctorId;

    DoctorBSTNode* doctorNode = searchBST(root, doctorId); //check if dr id exists
    //search from root of bst with dr id and assign result to doctorNode ptr
    //found, dr node points to the
    if (!doctorNode)
    {
        cout << "Doctor not found.\n";
        return;
    }

    cout << "Enter Patient ID to assign: ";
    cin >> patientId;

//check if patient exists
    Patient* currentPatient = patientSystem->head;
    bool patientFound = false;

    while (currentPatient != nullptr)
    {
        if (currentPatient->id == patientId)
        {
            patientFound = true;
            break;
        }
        currentPatient = currentPatient->next;
    }
    //if patient id is found, it will return true and loop breaks

    if (!patientFound)
    {
        cout << "Patient not found.\n";
        return;
    }

    for (int i = 0; i < doctorNode->doctorData->patientCount; i++) //i starts loop at 0, continues as long as i is less than
    //number of patients assigned to the dr
    {
        if (doctorNode->doctorData->assignedPatients[i] == patientId)
        //check if pt id at current i in assigned pt is = pt id being assigned
        {
            cout << "This patient is already assigned to this doctor.\n";
            return;
        }
    }

    if (doctorNode->doctorData->patientCount >= MAX_PATIENTS)
    {
        cout << "Doctor has reached maximum patient capacity.\n";
        return;
    }
    //check doctor patient maximum capacity

    doctorNode -> doctorData -> assignedPatients[doctorNode->doctorData->patientCount++] = patientId;
    cout << "Patient " << patientId << " successfully assigned to Dr. " << doctorNode->doctorData->name << "!\n";
    saveToFile();
}//assign pt to dr

void manageDoctors::displayDoctorPatients(int doctorId)
{
    DoctorBSTNode* doctorNode = searchBST(root, doctorId);
    if (!doctorNode)
    {
        cout << "Doctor not found.\n";
        return;
    }

    cout << "\nPatients assigned to Dr. " << doctorNode->doctorData->name << ":\n";
    for (int i = 0; i < doctorNode->doctorData->patientCount; i++)     //loops start from 0 as long as less than num of pt assigned to dr
    {
        int patientId = doctorNode->doctorData->assignedPatients[i];
        BSTNode* patientNode = patientSystem->searchBST(patientSystem->root, patientId);    //search pt id, if found, points to bst node
        if (patientNode)
        {
            cout << "ID: " << patientNode->patientData->id << ", Name: " << patientNode->patientData->name << "\n"; //if pt node found then print
        }
    }
}

void manageDoctors::cleanBST(DoctorBSTNode* node)
{
    if (node)
    {
        cleanBST(node->left);
        cleanBST(node->right);
        delete node;
    }
}

void manageDoctors::insertBST(DoctorBSTNode*& node, Doctor* doctor)
{
    if (!node)
    {
        node = new DoctorBSTNode(doctor);
        return;
    }

    if (doctor->id < node->doctorData->id)
    {
        insertBST(node->left, doctor);
    } else if (doctor->id > node->doctorData->id)
    {
        insertBST(node->right, doctor);
    }
}

DoctorBSTNode* manageDoctors::searchBST(DoctorBSTNode* node, int id)
{
    if (!node || !node->doctorData) return nullptr;
    if (node->doctorData->id == id) return node;
    if (id < node->doctorData->id)
        return searchBST(node->left, id);
    return searchBST(node->right, id);
}   //search dr is in bst

void manageDoctors::deleteBST(DoctorBSTNode*& node, int id)
{
    if (!node) return;

    if (id < node->doctorData->id)
        deleteBST(node->left, id);
    else if (id > node->doctorData->id)
        deleteBST(node->right, id);
    else
    {
        if (!node->left)
        {
            DoctorBSTNode* temp = node->right;
            delete node;
            node = temp;
        }
        else if (!node->right)
        {
            DoctorBSTNode* temp = node->left;
            delete node;
            node = temp;
        }
        else
        {
            DoctorBSTNode* temp = node->right;
            while (temp->left)
                temp = temp->left;
            node->doctorData = temp->doctorData;
            deleteBST(node->right, temp->doctorData->id);
        }
    }
}

void manageDoctors::saveToFile()
{  //save to local file doctor txt
    FILE* file = fopen("doctors.txt", "w");
    if (!file)
    {
        cout << "Error opening file for writing.\n";
        return;
    }

    Doctor* current = head;
    while (current)
    {
        fprintf(file, "%d\n%s\n%s\n%s\n%d\n",   //writes dr details
                current->id, current->name, current->specialization,
                current->contact, current->patientCount);

        for (int i = 0; i < current->patientCount; i++)
        {
            fprintf(file, "%d\n", current->assignedPatients[i]);
        }
        current = current->next;
    }
    fclose(file);
}

void manageDoctors::loadFromFile()
{
    FILE* file = fopen("doctors.txt", "r"); //read file
    if (!file) return;

    // Clear existing data
    while (head != nullptr)
    {
        Doctor* temp = head;
        head = head->next;
        delete temp;
    }
    cleanBST(root);
    root = nullptr;

    char buffer[100];
    while (!feof(file))
    {
        Doctor* newDoctor = new Doctor();

        if (fscanf(file, "%d\n", &newDoctor->id) != 1) {
            delete newDoctor;
            break;
        }

        //get dr details frm file
        fgets(newDoctor->name, 50, file);
        newDoctor->name[strcspn(newDoctor->name, "\n")] = 0;

        fgets(newDoctor->specialization, 50, file);
        newDoctor->specialization[strcspn(newDoctor->specialization, "\n")] = 0;

        fgets(newDoctor->contact, 15, file);
        newDoctor->contact[strcspn(newDoctor->contact, "\n")] = 0;

        fscanf(file, "%d\n", &newDoctor->patientCount);

        for (int i = 0; i < newDoctor->patientCount; i++) {
            fscanf(file, "%d\n", &newDoctor->assignedPatients[i]);
        }

        newDoctor->next = nullptr;
        if (!head)
        {
            head = newDoctor;
        } else {
            Doctor* current = head;
            while (current->next != nullptr)
            {
                current = current->next;
            }
            current->next = newDoctor;
        }

        insertBST(root, newDoctor);
    }
    fclose(file);
}


void manageDoctors::editDoctor()
{
    if (!head)
    {
        cout << "No doctors in the system.\n";
        return;
    }

    int searchId;
    cout << "Enter Doctor ID to edit: ";
    cin >> searchId;

    DoctorBSTNode* result = searchBST(root, searchId);
    if (!result)
    {
        cout << "Doctor not found.\n";
        return;
    }

    cout << "\nCurrent Doctor Details:\n";
    cout << "1. Name: " << result->doctorData->name << "\n";
    cout << "2. Specialization: " << result->doctorData->specialization << "\n";
    cout << "3. Contact: " << result->doctorData->contact << "\n";
    cout << "4. Cancel Edit\n";

    int choice;
    cout << "\nEnter field number to edit: ";
        cin >> choice;
    cin.ignore();

    try {
        switch(choice)
        {
            case 1:
                do
                {
                    cout << "Enter new name: ";
                    cin.getline(result->doctorData->name, 50);
                }
                while (!valUserInput(result->doctorData->name, 2));
                break;
            case 2:
                do
                {
                    cout << "Enter new specialization: ";
                    cin.getline(result->doctorData->specialization, 50);
                }
                while (!valUserInput(result->doctorData->specialization, 2));
                break;
            case 3:
                do
                {
                    cout << "Enter new contact: ";
                    cin.getline(result->doctorData->contact, 15);
                }
                while (!valUserInput(result->doctorData->contact, 1));
                break;
            case 4:
                cout << "Edit cancelled.\n";
                return;
            default:
                cout << "Invalid choice.\n";
                return;
        }
        cout << "Doctor record updated successfully!\n";
        saveToFile();
    }
    catch (...) {
        cout << "Error updating doctor information.\n";
    }
}

void manageDoctors::deleteDoctor()
{
    if (!head)
    {
        cout << "No doctors in the system.\n";
        return;
    }

    int deleteId;
    cout << "Enter Doctor ID to delete: ";
    cin >> deleteId;

    try {
        Doctor* current = head;
        Doctor* prev = nullptr;
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
            cout << "Doctor not found.\n";
            return;
        }

        if (prev == nullptr)
        {
            head = current->next;
        } else {
            prev->next = current->next;
        }

        deleteBST(root, deleteId);
        delete current;
        cout << "Doctor deleted successfully!\n";
        saveToFile();
    }
    catch (...) {
        cout << "Error deleting doctor.\n";
    }
}

void manageDoctors::searchDoctor()
{
    if (!head)
    {
        cout << "No doctors in the system.\n";
        return;
    }

    int searchId;
    cout << "Enter Doctor ID to search: ";
    cin >> searchId;

    try {
        DoctorBSTNode* result = searchBST(root, searchId);
        if (result && result->doctorData)
        {
            cout << "\nDoctor Found:\n";
            cout << "ID: " << result->doctorData->id << "\n";
            cout << "Name: " << result->doctorData->name << "\n";
            cout << "Specialization: " << result->doctorData->specialization << "\n";
            cout << "Contact: " << result->doctorData->contact << "\n";
            cout << "Number of assigned patients: " << result->doctorData->patientCount << "\n";
        } else
        {
            cout << "Doctor not found.\n";
        }
    }
    catch (...) {
        cout << "Error occurred while searching.\n";
    }
}

void manageDoctors::displayAllDoctors()
{
    if (!head)
    {
        cout << "No doctors in the system.\n";
        return;
    }

    cout << "\n=== All Doctors ===\n";
    cout << "ID\tName\t\tSpecialization\t\tContact\t\t\tPatients\n";
    cout << "--------------------------------------------------------------------------------\n";

    Doctor* current = head;
    while (current != nullptr)
    {
        //find padding according to length
        string nameStr(current->name);
        string specStr(current->specialization);
        string contactStr(current->contact);

        //add padding
        string namePad = nameStr.length() < 8 ? "\t\t" : "\t";
        string specPad = specStr.length() < 8 ? "\t\t\t" : "\t\t";
        string contactPad = contactStr.length() < 8 ? "\t\t\t" : "\t\t";

        cout << current->id << "\t"
             << current->name << namePad
             << current->specialization << specPad
             << current->contact << contactPad;
        cout << current->patientCount << "\n";
        current = current->next;
    }
    cout << "--------------------------------------------------------------------------------\n";
}

bool manageDoctors::valUserInput(const char* str, int type)
{   //val user data, 1 >> numbers, 2 >> alphabets
    if (!str[0]) return false;

    for (int i = 0; str[i]; i++)
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

void manageDoctors::sortDoctors()
{
    if (!head || !head->next) return;   //check if emtpy

    Doctor *i, *j;
    for (i = head; i != nullptr; i = i->next)
    {
        for (j = i->next; j != nullptr; j = j->next)       //loop thru list from node after i using ptr j
                                                            //j starts at i->next and moves to next node every loop
        {
            if (i->id > j->id)  //compare id and swap
            {
                int tempId = i->id;
                i->id = j->id;
                j->id = tempId;     //swaps dr id

                char tempStr[50];
                strcpy(tempStr, i->name);
                strcpy(i->name, j->name);
                strcpy(j->name, tempStr);   //swaps names of dr

                strcpy(tempStr, i->specialization);
                strcpy(i->specialization, j->specialization);
                strcpy(j->specialization, tempStr); //swaps specialisation

                strcpy(tempStr, i->contact);
                strcpy(i->contact, j->contact);
                strcpy(j->contact, tempStr);    //swaps contact num

                int tempPatients[MAX_PATIENTS];
                memcpy(tempPatients, i->assignedPatients, sizeof(int) * MAX_PATIENTS);
                memcpy(i->assignedPatients, j->assignedPatients, sizeof(int) * MAX_PATIENTS);
                memcpy(j->assignedPatients, tempPatients, sizeof(int) * MAX_PATIENTS);
                //swaps assigned pt arr btwn dr

                int tempCount = i->patientCount;
                i->patientCount = j->patientCount;
                j->patientCount = tempCount;    //swap pt count
            }
        }
    }
    cout << "Doctors sorted successfully!\n";
}
