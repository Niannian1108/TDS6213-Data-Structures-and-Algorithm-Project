// doctor.h
#ifndef DOCTOR_H
#define DOCTOR_H

#include "patient.h"

#define MAX_PATIENTS 100 // Maximum patients per doctor

struct Doctor {
    int id;
    char name[50];
    char specialization[50];
    char contact[15];
    int assignedPatients[MAX_PATIENTS]; // Array to store patient IDs
    int patientCount;                   // Number of assigned patients
    Doctor* next;

    Doctor() : id(0), patientCount(0), next(nullptr) {
        name[0] = '\0';
        specialization[0] = '\0';
        contact[0] = '\0';
    }
};

struct DoctorBSTNode {
    Doctor* doctorData;
    DoctorBSTNode* left;
    DoctorBSTNode* right;

    DoctorBSTNode(Doctor* doctor) : doctorData(doctor), left(nullptr), right(nullptr) {}
};

class manageDoctors {
private:
    Doctor* head;
    DoctorBSTNode* root;
    managePatients* patientSystem;

    void insertBST(DoctorBSTNode*& node, Doctor* doctor);
    DoctorBSTNode* searchBST(DoctorBSTNode* node, int id);
    void deleteBST(DoctorBSTNode*& node, int id);
    void cleanBST(DoctorBSTNode* node);

public:
    manageDoctors(managePatients* patientSys);
    ~manageDoctors();

    manageDoctors(const manageDoctors& other) = delete;
    manageDoctors& operator=(const manageDoctors& other) = delete;

    void addDoctor();
    void editDoctor();
    void deleteDoctor();
    void searchDoctor();
    void displayAllDoctors();
    void sortDoctors();
    void assignPatient();
    void displayDoctorPatients(int doctorId);

    void saveToFile();
    void loadFromFile();
    bool valUserInput(const char* str, int type);
};

#endif
