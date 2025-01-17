// doctor.cpp
#include "doctor.h"

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
        std::cout << "\n=== Add New Doctor ===\n";

        do {
            std::cout << "Enter Doctor ID: ";
            std::cin >> newDoctor->id;
        } while (newDoctor->id <= 0);

        std::cin.ignore();
        do {
            std::cout << "Enter Doctor Name: ";
            std::cin.getline(newDoctor->name, 50);
        } while (!valUserInput(newDoctor->name, 2));

        do {
            std::cout << "Enter Specialization: ";
            std::cin.getline(newDoctor->specialization, 50);
        } while (!valUserInput(newDoctor->specialization, 2));

        do {
            std::cout << "Enter Contact Number: ";
            std::cin.getline(newDoctor->contact, 15);
        } while (!valUserInput(newDoctor->contact, 1));

        newDoctor->next = head;
        head = newDoctor;
        insertBST(root, newDoctor);

        std::cout << "\nDoctor added successfully!\n";
        saveToFile();
    }
    catch(...) {
        std::cout << "Error adding doctor. Please try again.\n";
        delete newDoctor;
    }
}

void manageDoctors::assignPatient() {
    if (!head) {
        std::cout << "No doctors in the system.\n";
        return;
    }

    int doctorId, patientId;

    std::cout << "Enter Doctor ID: ";
    std::cin >> doctorId;

    DoctorBSTNode* doctorNode = searchBST(root, doctorId);
    if (!doctorNode) {
        std::cout << "Doctor not found.\n";
        return;
    }

    std::cout << "Enter Patient ID to assign: ";
    std::cin >> patientId;

    // Search through the patient linked list instead of BST
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
        std::cout << "Patient not found.\n";
        return;
    }

    // Check if patient is already assigned to this doctor
    for (int assignedId : doctorNode->doctorData->assignedPatients) {
        if (assignedId == patientId) {
            std::cout << "This patient is already assigned to this doctor.\n";
            return;
        }
    }

    // Add patient to doctor's list
    doctorNode->doctorData->assignedPatients.push_back(patientId);
    std::cout << "Patient " << patientId << " successfully assigned to Dr. "
              << doctorNode->doctorData->name << "!\n";
    saveToFile();
}
void manageDoctors::displayDoctorPatients(int doctorId) {
    DoctorBSTNode* doctorNode = searchBST(root, doctorId);
    if (!doctorNode) {
        std::cout << "Doctor not found.\n";
        return;
    }

    std::cout << "\nPatients assigned to Dr. " << doctorNode->doctorData->name << ":\n";
    for (int patientId : doctorNode->doctorData->assignedPatients) {
        // Display patient details using direct root access
        BSTNode* patientNode = patientSystem->searchBST(patientSystem->root, patientId);
        if (patientNode) {
            std::cout << "ID: " << patientNode->patientData->id
                     << ", Name: " << patientNode->patientData->name << "\n";
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
    std::ofstream file("doctors.txt");
    if (!file) {
        std::cout << "Error opening file for writing.\n";
        return;
    }

    Doctor* current = head;
    while (current) {
        file << current->id << "\n"
             << current->name << "\n"
             << current->specialization << "\n"
             << current->contact << "\n";

        // Save assigned patients
        file << current->assignedPatients.size() << "\n";
        for (int patientId : current->assignedPatients) {
            file << patientId << "\n";
        }

        current = current->next;
    }
    file.close();
}

void manageDoctors::loadFromFile() {
    std::ifstream file("doctors.txt");
    if (!file) return;

    // Clear existing data
    while (head != nullptr) {
        Doctor* temp = head;
        head = head->next;
        delete temp;
    }
    cleanBST(root);
    root = nullptr;

    while (file) {
        Doctor* newDoctor = new Doctor();

        if (!(file >> newDoctor->id)) {
            delete newDoctor;
            break;
        }

        file.ignore();
        file.getline(newDoctor->name, 50);
        file.getline(newDoctor->specialization, 50);
        file.getline(newDoctor->contact, 15);

        // Load assigned patients
        int numPatients;
        file >> numPatients;
        for (int i = 0; i < numPatients; i++) {
            int patientId;
            file >> patientId;
            newDoctor->assignedPatients.push_back(patientId);
        }
        file.ignore();

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
    file.close();
}

void manageDoctors::editDoctor() {
    if (!head) {
        std::cout << "No doctors in the system.\n";
        return;
    }

    int searchId;
    std::cout << "Enter Doctor ID to edit: ";
    std::cin >> searchId;

    DoctorBSTNode* result = searchBST(root, searchId);
    if (!result) {
        std::cout << "Doctor not found.\n";
        return;
    }

    std::cout << "\nCurrent Doctor Details:\n";
    std::cout << "1. Name: " << result->doctorData->name << "\n";
    std::cout << "2. Specialization: " << result->doctorData->specialization << "\n";
    std::cout << "3. Contact: " << result->doctorData->contact << "\n";
    std::cout << "4. Cancel Edit\n";

    int choice;
    std::cout << "\nEnter field number to edit: ";
    std::cin >> choice;
    std::cin.ignore();

    try {
        switch(choice) {
            case 1:
                do {
                    std::cout << "Enter new name: ";
                    std::cin.getline(result->doctorData->name, 50);
                } while (!valUserInput(result->doctorData->name, 2));
                break;
            case 2:
                do {
                    std::cout << "Enter new specialization: ";
                    std::cin.getline(result->doctorData->specialization, 50);
                } while (!valUserInput(result->doctorData->specialization, 2));
                break;
            case 3:
                do {
                    std::cout << "Enter new contact: ";
                    std::cin.getline(result->doctorData->contact, 15);
                } while (!valUserInput(result->doctorData->contact, 1));
                break;
            case 4:
                std::cout << "Edit cancelled.\n";
                return;
            default:
                std::cout << "Invalid choice.\n";
                return;
        }
        std::cout << "Doctor record updated successfully!\n";
        saveToFile();
    }
    catch (...) {
        std::cout << "Error updating doctor information.\n";
    }
}

void manageDoctors::deleteDoctor() {
    if (!head) {
        std::cout << "No doctors in the system.\n";
        return;
    }

    int deleteId;
    std::cout << "Enter Doctor ID to delete: ";
    std::cin >> deleteId;

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
            std::cout << "Doctor not found.\n";
            return;
        }

        if (prev == nullptr) {
            head = current->next;
        } else {
            prev->next = current->next;
        }

        deleteBST(root, deleteId);
        delete current;
        std::cout << "Doctor deleted successfully!\n";
        saveToFile();
    }
    catch (...) {
        std::cout << "Error deleting doctor.\n";
    }
}

void manageDoctors::searchDoctor() {
    if (!head) {
        std::cout << "No doctors in the system.\n";
        return;
    }

    int searchId;
    std::cout << "Enter Doctor ID to search: ";
    std::cin >> searchId;

    try {
        DoctorBSTNode* result = searchBST(root, searchId);
        if (result && result->doctorData) {
            std::cout << "\nDoctor Found:\n";
            std::cout << "ID: " << result->doctorData->id << "\n";
            std::cout << "Name: " << result->doctorData->name << "\n";
            std::cout << "Specialization: " << result->doctorData->specialization << "\n";
            std::cout << "Contact: " << result->doctorData->contact << "\n";
            std::cout << "Number of assigned patients: " << result->doctorData->assignedPatients.size() << "\n";
        } else {
            std::cout << "Doctor not found.\n";
        }
    }
    catch (...) {
        std::cout << "Error occurred while searching.\n";
    }
}

void manageDoctors::displayAllDoctors() {
    if (!head) {
        std::cout << "No doctors in the system.\n";
        return;
    }

    std::cout << "\n=== All Doctors ===\n";
    std::cout << "ID\tName\t\tSpecialization\t\tContact\t\t\tPatients\n";
    std::cout << "--------------------------------------------------------------------------------\n";

    Doctor* current = head;
    while (current != nullptr) {
        // Calculate padding for name and specialization
        std::string nameStr(current->name);
        std::string specStr(current->specialization);
        std::string contactStr(current->contact);

        // Add padding based on string length
        std::string namePad = nameStr.length() < 8 ? "\t\t" : "\t";
        std::string specPad = specStr.length() < 8 ? "\t\t\t" : "\t\t";
        std::string contactPad = contactStr.length() < 8 ? "\t\t\t" : "\t\t";

        std::cout << current->id << "\t"
                 << current->name << namePad
                 << current->specialization << specPad
                 << current->contact << contactPad
                 << current->assignedPatients.size() << "\n";
        current = current->next;
    }
    std::cout << "--------------------------------------------------------------------------------\n";
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
                std::swap(i->id, j->id);

                char temp[50];
                strcpy(temp, i->name);
                strcpy(i->name, j->name);
                strcpy(j->name, temp);

                strcpy(temp, i->specialization);
                strcpy(i->specialization, j->specialization);
                strcpy(j->specialization, temp);

                strcpy(temp, i->contact);
                strcpy(i->contact, j->contact);
                strcpy(j->contact, temp);

                // Swap assigned patients vector
                std::vector<int> tempPatients = i->assignedPatients;
                i->assignedPatients = j->assignedPatients;
                j->assignedPatients = tempPatients;
            }
        }
    }
    std::cout << "Doctors sorted successfully!\n";
}
