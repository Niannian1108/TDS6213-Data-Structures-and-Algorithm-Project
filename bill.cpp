#include "bill.h"
#incluie <iostream>
#include <string>
#include <iomanip>
using namespace std;

void insertPatientBill(PatientBill bills[], int& currentRowCount) {
    if (currentRowCount >= MAX_ROWS) {
        cout << "Maximum number of patients reached!" << endl;
        return;
    }

    PatientBill& currentBill = bills[currentRowCount];

    cout << "\nEnter Patient ID: ";
    cin >> currentBill.patientID;

    cout << "Enter number of bill descriptions for Patient " << currentBill.patientID << ": ";
    cin >> currentBill.numBills;

    if (currentBill.numBills <= 0 || currentBill.numBills > MAX_BILLS) {
        cout << "Invalid number of bill items. Must be between 1 and " << MAX_BILLS << ".\n";
        return;
    }

    currentBill.totalBill = 0.0;

    for (int i = 0; i < currentBill.numBills; i++)
    {

        cout << "Enter description for bill item" << (i + 1) << ": ";
        cin >> currentBill.bills[i];

        cout << "Enter cost for " << currentBill.bills[i] << ": ";
        cin >> currentBill.costs[i];

        currentBill.totalBill += currentBill.costs[i];

    }

    cout << "Enter issued bill date (e.g., DD/MM/YYYY): ";
    cin >> currentBill.issueDate;

    cout << "Enter paid status (Paid/Unpaid): ";
    cin >> currentBill.paidStatus;

    currentRowCount++;
}

void editPatientBill(PatientBill bills[], int currentRowCount) {
    if (currentRowCount == 0) {
        cout << "No bills to edit.\n";
        return;
    }

    string editID;
    cout << "\nEnter Patient ID to edit: ";
    cin >> editID;

    PatientBill* editBill = nullptr;
    for (int i = 0; i < currentRowCount; ++i) {
        if (bills[i].patientID == editID) {
            editBill = &bills[i];
            break;
        }
    }

    if (editBill == nullptr) {
        cout << "Patient ID not found!" << endl;
        return;
    }

    cout << "\nEditing bill for Patient ID: " << editBill->patientID << endl;

    cout << "Current Issued Bill Date: " << editBill->issueDate << endl;
    cout << "Enter new Issued Bill Date: ";
    cin.ignore();
    getline(cin, editBill->issueDate);

    cout << "Current Paid Status: " << editBill->paidStatus << endl;
    cout << "Enter new Paid Status: ";
    getline(cin, editBill->paidStatus);

    if (editBill->numBills > 0) {
        cout << "Current bill details:\n";
        for (int i = 0; i < editBill->numBills; ++i) {
            cout << i + 1 << ". " << editBill->bills[i]
                 << " - RM" << fixed << setprecision(2) << editBill->costs[i] << endl;
        }

        cout << "Do you want to edit any bill item? (y/n): ";
        char editChoice;
        cin >> editChoice;

        if (editChoice == 'y' || editChoice == 'Y') {
            int billIndex;
            cout << "Enter the index of the bill item to edit (1-based): ";
            cin >> billIndex;

            if (billIndex > 0 && billIndex <= editBill->numBills) {
                cout << "Current Description: " << editBill->bills[billIndex - 1] << endl;
                cout << "Enter new description: ";
                cin.ignore();
                getline(cin, editBill->bills[billIndex - 1]);

                cout << "Current Cost: RM" << editBill->costs[billIndex - 1] << endl;
                cout << "Enter new cost: ";
                cin >> editBill->costs[billIndex - 1];

                editBill->totalBill = 0.0;
                for (int i = 0; i < editBill->numBills; ++i) {
                    editBill->totalBill += editBill->costs[i];
                }
            } else {
                cout << "Invalid index!" << endl;
            }
        }
    }

    cout << "Bill edited successfully." << endl;
}

void deletePatientBill(PatientBill bills[], int& currentRowCount) {
    if (currentRowCount == 0) {
        cout << "No bills to delete.\n";
        return;
    }

    string patientIDToDelete;
    cout << "Enter the Patient ID to delete: ";
    cin >> patientIDToDelete;

    int indexToDelete = -1;
    for (int i = 0; i < currentRowCount; ++i) {
        if (bills[i].patientID == patientIDToDelete) {
            indexToDelete = i;
            break;
        }
    }

    if (indexToDelete == -1) {
        cout << "Patient ID not found!" << endl;
        return;
    }

    for (int i = indexToDelete; i < currentRowCount - 1; ++i) {
        bills[i] = bills[i + 1];
    }

    currentRowCount--;
    cout << "Patient bill deleted." << endl;
}

void viewDefaultSort(PatientBill bills[], int currentRowCount) {
    cout << "\n--- Patient Bills (Newest to Oldest Issued Date) ---\n";
    for (int i = 0; i < currentRowCount; ++i) {
        PatientBill& row = bills[i];
        cout << "\nPatient ID: " << row.patientID << endl;
        cout << "Total Bill Amount: RM" << fixed << setprecision(2) << row.totalBill << endl;
        for (int j = 0; j < row.numBills; ++j) {
            cout << row.bills[j] << "\tRM" << row.costs[j] << endl;
        }
        cout << "Issued Bill Date: " << row.issueDate << endl;
        cout << "Paid Status: " << row.paidStatus << endl;
    }
}

void viewSelectionSort(PatientBill bills[], int currentRowCount) {
    for (int i = 0; i < currentRowCount - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < currentRowCount; ++j) {
            if (bills[j].issueDate < bills[minIndex].issueDate) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(bills[i], bills[minIndex]);
        }
    }

    cout << "\n--- Patient Bills (Oldest to Newest Issued Date) ---\n";
    for (int i = 0; i < currentRowCount; ++i) {
        PatientBill& row = bills[i];
        cout << "\nPatient ID: " << row.patientID << endl;
        cout << "Total Bill Amount: RM" << fixed << setprecision(2) << row.totalBill << endl;
        for (int j = 0; j < row.numBills; ++j) {
            cout << row.bills[j] << "\tRM" << row.costs[j] << endl;
        }
        cout << "Issued Bill Date: " << row.issueDate << endl;
        cout << "Paid Status: " << row.paidStatus << endl;
    }
}
