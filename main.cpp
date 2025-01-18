#include "patient.h"
#include "doctor.h"
#include <iostream>
using namespace std;

void displayMainMenu()
{
    cout << "\n=== Hospital Management System ===\n";
    cout << "1. Patient Management\n";
    cout << "2. Doctor Management\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
}

void displayPatientMenu()
{
    cout << "\n=== Patient Management ===\n";
    cout << "1. Add Patient\n";
    cout << "2. Edit Patient\n";
    cout << "3. Delete Patient\n";
    cout << "4. Search Patient\n";
    cout << "5. Display All Patients\n";
    cout << "6. Sort Patients\n";
    cout << "7. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

void displayDoctorMenu()
{
    cout << "\n=== Doctor Management ===\n";
    cout << "1. Add Doctor\n";
    cout << "2. Edit Doctor\n";
    cout << "3. Delete Doctor\n";
    cout << "4. Search Doctor\n";
    cout << "5. Display All Doctors\n";
    cout << "6. Sort Doctors\n";
    cout << "7. Assign Patient to Doctor\n";
    cout << "8. Display Doctor's Patients\n";
    cout << "9. Return to Main Menu\n";
    cout << "Enter your choice: ";
}

int main()
{
    managePatients hospital;
    manageDoctors doctors(&hospital);  // Pass pointer to patient system
    int choice, subChoice;

    while (true)
    {
        displayMainMenu();
        std::cin >> choice;

        switch (choice)
        {
            case 1:  // Patient Management
                while (true)
                {
                    displayPatientMenu();
                    std::cin >> subChoice;

                    switch (subChoice)
                    {
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
                            goto main_menu;  // Return to main menu
                        default:
                            cout << "Invalid choice. Please try again.\n";
                    }
                }
                break;

            case 2:  // Doctor Management
                while (true)
                {
                    displayDoctorMenu();
                    std::cin >> subChoice;

                    switch (subChoice)
                    {
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
                            cout << "Enter Doctor ID: ";
                            std::cin >> doctorId;
                            doctors.displayDoctorPatients(doctorId);
                            break;
                        case 9:
                            goto main_menu;  // Return to main menu
                        default:
                            cout << "Invalid choice. Please try again.\n";
                    }
                }
                break;

            case 3:  // Exit
                cout << "\nThank you for using Hospital Management System!\n";
                return 0;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
        main_menu:;  // Label for returning to main menu
    }

    return 0;
}
