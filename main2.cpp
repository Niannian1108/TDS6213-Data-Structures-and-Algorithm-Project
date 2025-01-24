
#include "patient.h"
#include "doctor.h"
#include "bill.cpp"
#include <iostream>
using namespace std;

void displayMainMenu()
{
    cout << "\n=== Hospital Management System ===\n";
    cout << "1. Patient Management\n";
    cout << "2. Doctor Management\n";
    cout << "3. Patient Bills\n";
    cout << "4. Exit\n";
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

void displayFinanceMenu()
{
    cout << "1. Insert Patient Bills\n";
    cout << "2. Edit Patient Bills\n";
    cout << "3. Delete Patient Bills\n";
    cout << "4. View All Patient Bills\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}


int main()
{
    managePatients hospital;
    manageDoctors doctors(&hospital);
    int choice, subChoice;
    PatientBill bills[MAX_ROWS];
    int currentRowCount = 0;

    while (true)
    {
        displayMainMenu();
        std::cin >> choice;

        switch (choice)
        {
            case 1:
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
                            goto main_menu;
                        default:
                            cout << "Invalid choice. Please try again.\n";
                    }
                }
                break;

            case 2:
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
                            goto main_menu;
                        default:
                            cout << "Invalid choice. Please try again.\n";
                    }
                }
                break;

            case 3:
                while (true)
                    {
                        displayFinanceMenu();
                        std::cin >> subChoice;

                        switch (subChoice)
                        {
                            case 1:
                                insertPatientBill(bills, currentRowCount);
                                break;
                            case 2:
                                editPatientBill(bills, currentRowCount);
                                break;
                            case 3:
                                deletePatientBill(bills, currentRowCount);
                                break;
                            case 4:
                                cout << "1. View Newest to Oldest Issued Date\n";
                                cout << "2. View Oldest to Newest Issued Date\n";
                                cout << "Enter your choice: ";
                                int sortChoice;
                                cin >> sortChoice;
                                if (sortChoice == 1) {
                                    viewDefaultSort(bills, currentRowCount);
                                } else if (sortChoice == 2) {
                                    viewSelectionSort(bills, currentRowCount);
                                }
                            case 5:
                                goto main_menu();
                            default:
                                cout << "Invalid choice. Please try again.\n";
                        }
                    }
                    break;

            case 4:
                cout << "\nThank you for using Hospital Management System!\n";
                return 0;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
        main_menu:;
    }

    return 0;
}
