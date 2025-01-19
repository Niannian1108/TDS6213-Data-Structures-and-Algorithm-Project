// doctor.cpp
#include "doctor.h"
#include <cstring>
#include <iostream>

using namespace std;

manageDoctors::manageDoctors(managePatients* patientSys)
    : head(nullptr), root(nullptr), patientSystem(patientSys) {
    loadFromFile();
}

manageDoctors::~manageDoctors() {
    while (head != nullptr) {
        Doctor* temp = head;
        head = head->next;
        delete temp;
    }
    cleanBST(root);
}

void manageDoctors::addDoctor() {
    Doctor* newDoctor = new Doctor();

    try {
        cout << "\n=== Add New Doctor ===\n";

        do {
            cout << "Enter Doctor ID: ";
            cin >> newDoctor->id;
        } while (newDoctor->id <= 0);

        cin.ignore();
        do {
            cout << "Enter Doctor Name: ";
            cin.getline(newDoctor->name, 50);
        } while (!valUserInput(newDoctor->name, 2));

        do {
            cout << "Enter Specialization: ";
            cin.getline(newDoctor->specialization, 50);
        } while (!valUserInput(newDoctor->specialization, 2));

        do {
            cout << "Enter Contact Number: ";
            cin.getline(newDoctor->contact, 15);
        } while (!valUserInput(newDoctor->contact, 1));

        newDoctor->next = head;
        head = newDoctor;
        insertBST(root, newDoctor);

        cout << "\nDoctor added successfully!\n";
        saveToFile();
    }
    catch(...) {
        cout << "Error adding doctor. Please try again.\n";
        delete newDoctor;
    }
}

void manageDoctors::assignPatient() {
    if (!head) {
        cout << "No doctors in the system.\n";
        return;
    }

    int doctorId, patientId;

    cout << "Enter Doctor ID: ";
    cin >> doctorId;

    DoctorBSTNode* doctorNode = searchBST(root, doctorId);
    if (!doctorNode) {
        cout << "Doctor not found.\n";
        return;
    }

    cout << "Enter Patient ID to assign: ";
    cin >> patientId;

    // Check if patient exists
    Patient* currentPatient = patientSystem->head;
    bool patientFound = false;

    while (currentPatient != nullptr) {
        if (currentPatient->id == patientId) {
            patientFound = true;
            break;
        }
        currentPatient = currentPatient->next;
    }

    if (!patientFound) {
        cout << "Patient not found.\n";
        return;
    }

    // Check if patient is already assigned
    for (int i = 0; i < doctorNode->doctorData->patientCount; i++) {
        if (doctorNode->doctorData->assignedPatients[i] == patientId) {
            cout << "This patient is already assigned to this doctor.\n";
            return;
        }
    }

    // Check if doctor has reached maximum patients
    if (doctorNode->doctorData->patientCount >= MAX_PATIENTS) {
        cout << "Doctor has reached maximum patient capacity.\n";
        return;
    }

    // Add patient to doctor's list
    doctorNode->doctorData->assignedPatients[doctorNode->doctorData->patientCount++] = patientId;
    cout << "Patient " << patientId << " successfully assigned to Dr. " << doctorNode->doctorData->name << "!\n";
    saveToFile();
}

void manageDoctors::displayDoctorPatients(int doctorId) {
    DoctorBSTNode* doctorNode = searchBST(root, doctorId);
    if (!doctorNode) {
        cout << "Doctor not found.\n";
        return;
    }

    cout << "\nPatients assigned to Dr. " << doctorNode->doctorData->name << ":\n";
    for (int i = 0; i < doctorNode->doctorData->patientCount; i++) {
        int patientId = doctorNode->doctorData->assignedPatients[i];
        BSTNode* patientNode = patientSystem->searchBST(patientSystem->root, patientId);
        if (patientNode) {
            cout << "ID: " << patientNode->patientData->id << ", Name: " << patientNode->patientData->name << "\n";
        }
    }
}

void manageDoctors::cleanBST(DoctorBSTNode* node) {
    if (node) {
        cleanBST(node->left);
        cleanBST(node->right);
        delete node;
    }
}

void manageDoctors::insertBST(DoctorBSTNode*& node, Doctor* doctor) {
    if (!node) {
        node = new DoctorBSTNode(doctor);
        return;
    }

    if (doctor->id < node->doctorData->id) {
        insertBST(node->left, doctor);
    } else if (doctor->id > node->doctorData->id) {
        insertBST(node->right, doctor);
    }
}

DoctorBSTNode* manageDoctors::searchBST(DoctorBSTNode* node, int id) {
    if (!node || !node->doctorData) return nullptr;
    if (node->doctorData->id == id) return node;
    if (id < node->doctorData->id)
        return searchBST(node->left, id);
    return searchBST(node->right, id);
}

void manageDoctors::deleteBST(DoctorBSTNode*& node, int id) {
    if (!node) return;

    if (id < node->doctorData->id)
        deleteBST(node->left, id);
    else if (id > node->doctorData->id)
        deleteBST(node->right, id);
    else {
        if (!node->left) {
            DoctorBSTNode* temp = node->right;
            delete node;
            node = temp;
        }
        else if (!node->right) {
            DoctorBSTNode* temp = node->left;
            delete node;
            node = temp;
        }
        else {
            DoctorBSTNode* temp = node->right;
            while (temp->left)
                temp = temp->left;
            node->doctorData = temp->doctorData;
            deleteBST(node->right, temp->doctorData->id);
        }
    }
}

void manageDoctors::saveToFile() {
    FILE* file = fopen("doctors.txt", "w");
    if (!file) {
        cout << "Error opening file for writing.\n";
        return;
    }

    Doctor* current = head;
    while (current) {
        fprintf(file, "%d\n%s\n%s\n%s\n%d\n",
                current->id, current->name, current->specialization,
                current->contact, current->patientCount);

        for (int i = 0; i < current->patientCount; i++) {
            fprintf(file, "%d\n", current->assignedPatients[i]);
        }
        current = current->next;
    }
    fclose(file);
}

void manageDoctors::loadFromFile() {
    FILE* file = fopen("doctors.txt", "r");
    if (!file) return;

    // Clear existing data
    while (head != nullptr) {
        Doctor* temp = head;
        head = head->next;
        delete temp;
    }
    cleanBST(root);
    root = nullptr;

    char buffer[100];
    while (!feof(file)) {
        Doctor* newDoctor = new Doctor();

        if (fscanf(file, "%d\n", &newDoctor->id) != 1) {
            delete newDoctor;
            break;
        }

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

        // Add to linked list
        newDoctor->next = nullptr;
        if (!head) {
            head = newDoctor;
        } else {
            Doctor* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newDoctor;
        }

        insertBST(root, newDoctor);
    }
    fclose(file);
}


void manageDoctors::editDoctor() {
    if (!head) {
        cout << "No doctors in the system.\n";
        return;
    }

    int searchId;
    cout << "Enter Doctor ID to edit: ";
    cin >> searchId;

    DoctorBSTNode* result = searchBST(root, searchId);
    if (!result) {
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
        switch(choice) {
            case 1:
                do {
                    cout << "Enter new name: ";
                    cin.getline(result->doctorData->name, 50);
                } while (!valUserInput(result->doctorData->name, 2));
                break;
            case 2:
                do {
                    cout << "Enter new specialization: ";
                    cin.getline(result->doctorData->specialization, 50);
                } while (!valUserInput(result->doctorData->specialization, 2));
                break;
            case 3:
                do {
                    cout << "Enter new contact: ";
                    cin.getline(result->doctorData->contact, 15);
                } while (!valUserInput(result->doctorData->contact, 1));
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

void manageDoctors::deleteDoctor() {
    if (!head) {
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

        while (current != nullptr) {
            if (current->id == deleteId) {
                found = true;
                break;
            }
            prev = current;
            current = current->next;
        }

        if (!found) {
            cout << "Doctor not found.\n";
            return;
        }

        if (prev == nullptr) {
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

void manageDoctors::searchDoctor() {
    if (!head) {
        cout << "No doctors in the system.\n";
        return;
    }

    int searchId;
    cout << "Enter Doctor ID to search: ";
    cin >> searchId;

    try {
        DoctorBSTNode* result = searchBST(root, searchId);
        if (result && result->doctorData) {
            cout << "\nDoctor Found:\n";
            cout << "ID: " << result->doctorData->id << "\n";
            cout << "Name: " << result->doctorData->name << "\n";
            cout << "Specialization: " << result->doctorData->specialization << "\n";
            cout << "Contact: " << result->doctorData->contact << "\n";
            cout << "Number of assigned patients: " << result->doctorData->patientCount << "\n";
        } else {
            cout << "Doctor not found.\n";
        }
    }
    catch (...) {
        cout << "Error occurred while searching.\n";
    }
}

void manageDoctors::displayAllDoctors() {
    if (!head) {
        cout << "No doctors in the system.\n";
        return;
    }

    cout << "\n=== All Doctors ===\n";
    cout << "ID\tName\t\tSpecialization\t\tContact\t\t\tPatients\n";
    cout << "--------------------------------------------------------------------------------\n";

    Doctor* current = head;
    while (current != nullptr) {
        // Calculate padding for name and specialization
        string nameStr(current->name);
        string specStr(current->specialization);
        string contactStr(current->contact);

        // Add padding based on string length
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

bool manageDoctors::valUserInput(const char* str, int type) {
    if (!str[0]) return false;

    for (int i = 0; str[i]; i++) {
        if (type == 1) { // Numeric
            if (!isdigit(str[i])) return false;
        }
        else if (type == 2) { // Alphabetic
            if (!isalpha(str[i]) && str[i] != ' ') return false;
        }
    }
    return true;
}

void manageDoctors::sortDoctors() {
    if (!head || !head->next) return;

    Doctor *i, *j;
    for (i = head; i != nullptr; i = i->next) {
        for (j = i->next; j != nullptr; j = j->next) {
            if (i->id > j->id) {
                // Swap data
                int tempId = i->id;
                i->id = j->id;
                j->id = tempId;

                char tempStr[50];
                strcpy(tempStr, i->name);
                strcpy(i->name, j->name);
                strcpy(j->name, tempStr);

                strcpy(tempStr, i->specialization);
                strcpy(i->specialization, j->specialization);
                strcpy(j->specialization, tempStr);

                strcpy(tempStr, i->contact);
                strcpy(i->contact, j->contact);
                strcpy(j->contact, tempStr);

                // Swap patient arrays
                int tempPatients[MAX_PATIENTS];
                memcpy(tempPatients, i->assignedPatients, sizeof(int) * MAX_PATIENTS);
                memcpy(i->assignedPatients, j->assignedPatients, sizeof(int) * MAX_PATIENTS);
                memcpy(j->assignedPatients, tempPatients, sizeof(int) * MAX_PATIENTS);

                // Swap patient counts
                int tempCount = i->patientCount;
                i->patientCount = j->patientCount;
                j->patientCount = tempCount;
            }
        }
    }
    cout << "Doctors sorted successfully!\n";
}
