// main.cpp
#include "patient.h"
#include "doctor.h"
#include <stdio.h>
#include <iostream>

using namespace std;

void displayMainMenu() {
    printf("\n=== Hospital Management System ===\n");
    printf("1. Patient Management\n");
    printf("2. Doctor Management\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

void displayPatientMenu() {
    printf("\n=== Patient Management ===\n");
    printf("1. Add Patient\n");
    printf("2. Edit Patient\n");
    printf("3. Delete Patient\n");
    printf("4. Search Patient\n");
    printf("5. Display All Patients\n");
    printf("6. Sort Patients\n");
    printf("7. Return to Main Menu\n");
    printf("Enter your choice: ");
}

void displayDoctorMenu() {
    printf("\n=== Doctor Management ===\n");
    printf("1. Add Doctor\n");
    printf("2. Edit Doctor\n");
    printf("3. Delete Doctor\n");
    printf("4. Search Doctor\n");
    printf("5. Display All Doctors\n");
    printf("6. Sort Doctors\n");
    printf("7. Assign Patient to Doctor\n");
    printf("8. Display Doctor's Patients\n");
    printf("9. Return to Main Menu\n");
    printf("Enter your choice: ");
}

int main() {
    managePatients hospital;
    manageDoctors doctors(&hospital);
    int choice, subChoice;

    while (1) {
        displayMainMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:  // Patient Management
                while (true) {
                    displayPatientMenu();
                    scanf("%d", &subChoice);

                    switch (subChoice) {
                        case 1:
                            hospital.addPatient();
                            break;
                        case 2:
                            hospital.editPatient();
                            break;
                        case 3:
                            hospital.deletePatient();
                            break;
                        case 4:
                            hospital.searchPatient();
                            break;
                        case 5:
                            hospital.displayAllPatients();
                            break;
                        case 6:
                            hospital.sortPatients();
                            break;
                        case 7:
                            goto main_menu;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                }
                break;

            case 2:  // Doctor Management
                while (true) {
                    displayDoctorMenu();
                    scanf("%d", &subChoice);

                    switch (subChoice) {
                        case 1:
                            doctors.addDoctor();
                            break;
                        case 2:
                            doctors.editDoctor();
                            break;
                        case 3:
                            doctors.deleteDoctor();
                            break;
                        case 4:
                            doctors.searchDoctor();
                            break;
                        case 5:
                            doctors.displayAllDoctors();
                            break;
                        case 6:
                            doctors.sortDoctors();
                            break;
                        case 7:
                            doctors.assignPatient();
                            break;
                        case 8:
                            int doctorId;
                            printf("Enter Doctor ID: ");
                            scanf("%d", &doctorId);
                            doctors.displayDoctorPatients(doctorId);
                            break;
                        case 9:
                            goto main_menu;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                }
                break;

            case 3:  // Exit
                printf("\nThank you for using Hospital Management System!\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
        main_menu:;
    }

    return 0;
}
