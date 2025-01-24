#ifndef BILL_H_INCLUDED
#define BILL_H_INCLUDED
using namespace std;

#define MAX_ROWS 100
#define MAX_BILLS 10

struct PatientBill {
    string patientID;
    string bills[MAX_BILLS];
    float costs[MAX_BILLS];
    int numBills;
    float totalBill;
    string issueDate;
    string paidStatus;
};

void insertPatientBill(PatientBill bills[], int &currentRowCount);
void editPatientBill(PatientBill bills[], int &currentRowCount);
void deletePatientBill(PatientBill bills[], int &currentRowCount);
void viewDefaultSort(PatientBill bills[], int currentRowCount);
void viewSelectionSort(PatientBill bills[], int currentRowCount);

#endif // BILL_H_INCLUDED
