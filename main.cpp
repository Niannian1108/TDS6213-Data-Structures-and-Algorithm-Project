#include "patient.h"

void displayMenu()  //YX: only amend if u wanna add to the menu
                    //YX: If possible, please amend this function
                    //where page1>> chooses to interact with patient, doctor or hospital etc
                    //after choose, then only move into respective funstions on pg2 menu
{
    std::cout << "\n=== Hospital Management System ===\n";
    std::cout << "1. Add Patient\n";
    std::cout << "2. Edit Patient\n";
    std::cout << "3. Delete Patient\n";
    std::cout << "4. Search Patient\n";
    std::cout << "5. Display All Patients\n";
    std::cout << "6. Sort Patients\n";
    std::cout << "7. Exit\n";
    std::cout << "Enter your choice: ";
}

int main()
{
    managePatients hospital;
    int choice;

    while (true)
    {
        displayMenu();
        std::cin >> choice;

        switch (choice)
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
                std::cout << "Patients sorted successfully!\n";
                break;
            case 7:
                std::cout << "\n\nThank you for using Hospital Management System!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
