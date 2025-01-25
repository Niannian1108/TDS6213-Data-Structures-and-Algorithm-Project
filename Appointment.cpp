#include "Appointment.h"
#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;

// Constructor & Destructor

//Take and link the two system which are doctors and patients system with the appointment, and then load existing appointments from a file
manageAppointments::manageAppointments(manageDoctors* docSys, managePatients* patSys)
{
    head = nullptr;
    doctorAppointmentsHead = nullptr; //This function is create an empty list for doctor's appointment
    patientAppointmentsHead = nullptr; //This funtion is create an empty list for patient appointment

    doctorSystem = docSys; //this is use to link the doctor sys
    patientSystem = patSys; //this is use to link the patient sys

    loadFromFile(); //Read the existing appointments from a saved file
}

manageAppointments::~manageAppointments()
{
    // Clean up main appointments list

    Appointment* current = head; //points to first appointment in list

    while (current != nullptr) //loops until end of list
    {
        Appointment* next = current->next;
        delete current;
        current = next;
    }

    // This is use to clean up doctor appointments map

    //Create a pointer to track the current doctor appointmentmap
    AppointmentMap* currentDoctorMap = doctorAppointmentsHead;

    while (currentDoctorMap != nullptr)
    {
        AppointmentNode* currentNode = currentDoctorMap->appointments; //get pointer to current dostor appointment list

        while (currentNode != nullptr)
        {
            AppointmentNode* nextNode = currentNode->next; //save next node
            delete currentNode; //delete current node
            currentNode = nextNode; //move to next node
        }

        AppointmentMap* nextMap = currentDoctorMap->next; //save next map before delete
        delete currentDoctorMap; //delete current doctor map
        currentDoctorMap = nextMap; //move to next doctor map
    }

    // Clean up patient appointments map

    //Start with first patient map
    AppointmentMap* currentPatientMap = patientAppointmentsHead;

    while (currentPatientMap != nullptr)
    {
        AppointmentNode* currentNode = currentPatientMap->appointments; //get pointer to current patient appointment

        while (currentNode != nullptr)
        {
            AppointmentNode* nextNode = currentNode->next;
            delete currentNode;
            currentNode = nextNode;
        }

        AppointmentMap* nextMap = currentPatientMap->next;
        delete currentPatientMap;
        currentPatientMap = nextMap;
    }
}

// Validation Functions

bool manageAppointments::validateDate(const char* date)
{
    if (strlen(date) != 10) return false; //this is use to check if the date is exactly 10 characters, which is YYYY-MM-DD
    if (date[4] != '-' || date[7] != '-') return false; //this is use to check the characters are in the correct positions YYYY-MM-DD

    int year = (date[0] - '0') * 1000 + (date[1] - '0') * 100 +
               (date[2] - '0') * 10 + (date[3] - '0'); //this is use to convert first 4 characters to year number

    int month = (date[5] - '0') * 10 + (date[6] - '0');
    int day = (date[8] - '0') * 10 + (date[9] - '0');

    if (month < 1 || month > 12) return false; //this is use to check the month is valid or not valid, 0 or >12 is return false
    if (day < 1 || day > 31) return false; //this is use to check the date is valid or not valid, 0 and >31 is not valid and return false
    if (year < 2024) return false; //since right now is year 2025, it will start with 2025 and <2024 is not valid.

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //store how many dsys each month have

    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
        daysInMonth[1] = 29; //adjust February to 29 days because of leap year

    return day <= daysInMonth[month - 1];
    //check if day is valid or not for that specific month
}

bool manageAppointments::validateTime(const char* time)
{
    if (strlen(time) != 5 || time[2] != ':') return false; //check if time is 5 characters which is HH:MM

    int hours = (time[0] - '0') * 10 + (time[1] - '0'); //this is use to convert first two characters to hour number
    int minutes = (time[3] - '0') * 10 + (time[4] - '0'); //this is use to convert last two characters to minit number

    if (hours < 9 || hours > 17) return false; //This is because we define the operating working time is start with 9am till 5pm
    if (minutes != 0 && minutes != 30) return false; // This is use to allow the appointments on an hour or half hour only
    if (hours == 17 && minutes == 30) return false; //As o explain, after 5.30pm is not valid and return false

    return true;
}

//doctor verification
bool manageAppointments::verifyDoctorExists(int doctorId)
{
    Doctor* current = doctorSystem->head; //start at first doctor

    while (current != nullptr)  //loop through doctor list
    {
        if (current->id == doctorId) return true; //this is use to found the matching ID
        current = current->next;
    }
    return false; //this means not match found in doctor list
}

//Patient verification (same with doctor verification)
bool manageAppointments::verifyPatientExists(int patientId)
{
    Patient* current = patientSystem->head;

    while (current != nullptr)
    {
        if (current->id == patientId) return true;
        current = current->next;
    }
    return false;
}

//This is a merge sorting algorithm for display the appointment; it will display follow the date and time earlier first
bool manageAppointments::isEarlierDateTime(const char* date1, const char* time1,
                                         const char* date2, const char* time2)

{
    int result = strcmp(date1, date2); //this is use to compare dates
    if (result < 0) return true; //First date is earlier
    if (result > 0) return false; //second date is earlier

    return strcmp(time1, time2) < 0; // in a same date compare times
}

Appointment* manageAppointments::getMiddle(Appointment* head)
{
    if (!head) return nullptr; //empty list check

    Appointment* slow = head; //this use for slower pointer move on step
    Appointment* fast = head->next; //this use for fast pointer moves two steps

    while (fast && fast->next) //this is use to find the middle using twopointer technique

    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

Appointment* manageAppointments::mergeSortedLists(Appointment* a, Appointment* b)

{
    if (!a) return b; //if the first list is empty, return to second list
    if (!b) return a; //if the second empty, return first list

    Appointment* result = nullptr; //initialize pointer

    if (isEarlierDateTime(a->date, a->time, b->date, b->time) ||
       (strcmp(a->date, b->date) == 0 && strcmp(a->time, b->time) == 0 &&
        a->appointmentId < b->appointmentId))
    {
        result = a;
        result->next = mergeSortedLists(a->next, b);
    }
    else
    {
        result = b;
        result->next = mergeSortedLists(a, b->next);
    }

    return result;
}

Appointment* manageAppointments::mergeSort(Appointment* head) {
    if (!head || !head->next) return head;

    //find the middile point
    Appointment* middle = getMiddle(head);
    Appointment* nextToMiddle = middle->next;

    //split list into two half
    middle->next = nullptr;

    //This is recursively sort both half
    Appointment* left = mergeSort(head);
    Appointment* right = mergeSort(nextToMiddle);

    //merge sorted helf
    return mergeSortedLists(left, right);
}

//This is the Helper Functions

void manageAppointments::getDoctorName(int doctorId, char* name)
{
    Doctor* current = doctorSystem->head;

    while (current != nullptr) //this is search through list
    {
        if (current->id == doctorId) //found mataching ID
        {
            strcpy(name, current->name); //and then copy doctor name
            return;
        }
        current = current->next;
    }

    strcpy(name, "Unknown Doctor"); //not found
}
//same with the getdoctorname, but this is use to find the patient name with find the matching ID
void manageAppointments::getPatientName(int patientId, char* name)
{
    Patient* current = patientSystem->head;
    while (current != nullptr)
    {
        if (current->id == patientId)
        {
            strcpy(name, current->name);
            return;
        }

        current = current->next;
    }

    strcpy(name, "Unknown Patient");
}

bool manageAppointments::isDoctorAvailable(int doctorId, const char* date, const char* time)
{
    AppointmentMap* currentMap = doctorAppointmentsHead;

    while (currentMap != nullptr) //search for doctor
    {
        if (currentMap->id == doctorId) //found doctor
        {
            AppointmentNode* currentNode = currentMap->appointments;

            while (currentNode != nullptr) //check all apointment
            {
                if (strcmp(currentNode->appointmentPtr->date, date) == 0 &&
                    strcmp(currentNode->appointmentPtr->time, time) == 0 &&
                    strcmp(currentNode->appointmentPtr->status, "Cancelled") != 0) {
                    return false; //show that the doctor is not available
                }

                currentNode = currentNode->next;
            }

            break;
        }
        currentMap = currentMap->next;
    }
    return true; //doctor is available
}

bool manageAppointments::hasPatientAppointment(int patientId, const char* date, const char* time)
{
    //same with the patient and return true if conflict found for patient
    AppointmentMap* currentMap = patientAppointmentsHead;

    while (currentMap != nullptr)
    {
        if (currentMap->id == patientId)
        {
            AppointmentNode* currentNode = currentMap->appointments;

            while (currentNode != nullptr)
            {
                if (strcmp(currentNode->appointmentPtr->date, date) == 0 &&
                    strcmp(currentNode->appointmentPtr->time, time) == 0 &&
                    strcmp(currentNode->appointmentPtr->status, "Cancelled") != 0) {
                    return true; //Patient has confict
                }

                currentNode = currentNode->next;
            }

            break;
        }
        currentMap = currentMap->next;
    }
    return false;
}

// Main Operations which is create appointment

//This function is use to create the appointment as well
void manageAppointments::createAppointment()
{
    Appointment* newAppointment = new Appointment();

    cout << "\n--------------------------\n";
    cout << "Enter Patient ID:";

    cin >> newAppointment->patientId;
    cin.ignore();

    if (!verifyPatientExists(newAppointment->patientId))
    {
        cout << "Error: Patient ID does not exist.\n";
        delete newAppointment;

        return;
    }

    cout << "\n-------------------------\n";
    cout << "Enter Doctor ID:";

    cin >> newAppointment->doctorId;
    cin.ignore();

    if (!verifyDoctorExists(newAppointment->doctorId))
    {
        cout << "Error: Doctor ID does not exist.\n";
        delete newAppointment;

        return;
    }

    char date[11];
    do
    {
        cout << "\n----------------------------------\n";
        cout << "Enter Date (YYYY-MM-DD):";

        cin.getline(date, 11);
    }

    while (!validateDate(date));
    strcpy(newAppointment->date, date);

    char time[6];
    do
    {
        //display the available operating time for user to choose
        cout << "\n-----------------------------------------------------------------\n";
        cout << "----- Enter Time (HH:MM) - Available slots: ---------------------\n";
        cout << "----- Morning: 09:00, 09:30, 10:00, 10:30, 11:00, 11:30 ---------\n";
        cout << "----- Afternoon: 12:00, 12:30, 13:00, 13:30, 14:00, 14:30 -------\n";
        cout << "----- Evening: 15:00, 15:30, 16:00, 16:30, 17:00 ----------------\n";
        cout << "\n-----------------------------------------------------------------\n";
        cout << "\nEnter time:";

        cin.getline(time, 6);
    }
    while (!validateTime(time));
    strcpy(newAppointment->time, time);

    if (!isDoctorAvailable(newAppointment->doctorId, date, time))
    {
        cout << "\nDoctor is not available at this time.\n";
        delete newAppointment;
        return;
    }

    if (hasPatientAppointment(newAppointment->patientId, date, time))
    {
        cout << "\nPatient already has an appointment at this time.\n";
        delete newAppointment;
        return;
    }

    // This is use to find highest existing ID
    static int nextId = 1;
    Appointment* current = head;

    while (current != nullptr)

    {
        if (current->appointmentId >= nextId)
        {
            nextId = current->appointmentId + 1;
        }

        current = current->next;
    }

    newAppointment->appointmentId = nextId++;
    strcpy(newAppointment->status, "Scheduled");

    // this is use to add to main list
    newAppointment->next = head;
    head = newAppointment;

    // this is use to add to doctor's appointments
    AppointmentMap* doctorMap = doctorAppointmentsHead;

    while (doctorMap && doctorMap->id != newAppointment->doctorId)
    {
        doctorMap = doctorMap->next;
    }

    if (!doctorMap)
    {
        doctorMap = new AppointmentMap(newAppointment->doctorId);
        doctorMap->next = doctorAppointmentsHead;
        doctorAppointmentsHead = doctorMap;
    }

    AppointmentNode* newDoctorNode = new AppointmentNode(newAppointment);
    newDoctorNode->next = doctorMap->appointments;
    doctorMap->appointments = newDoctorNode;

    // Add to patient's appointments
    AppointmentMap* patientMap = patientAppointmentsHead;

    while (patientMap && patientMap->id != newAppointment->patientId)
    {
        patientMap = patientMap->next;
    }

    if (!patientMap)
    {
        patientMap = new AppointmentMap(newAppointment->patientId);
        patientMap->next = patientAppointmentsHead;
        patientAppointmentsHead = patientMap;
    }

    AppointmentNode* newPatientNode = new AppointmentNode(newAppointment);
    newPatientNode->next = patientMap->appointments;
    patientMap->appointments = newPatientNode;

    cout << "\nAppointment created successfully!\n";
    saveToFile();
}

//This function is use to display the appointment
void manageAppointments::displayAppointments()
{
    if (head == nullptr)
    {
        cout << "\nNo appointments found.\n";
        return;
    }

    head = mergeSort(head);

    cout << "\n=== Appointments List (Sorted by Date & Time) ===\n";
    cout << "ID\tPatient\t\tDoctor\t\tDate\t\tTime\tStatus\n";
    cout << "------------------------------------------------------------------------\n";

    Appointment* current = head;

    while (current != nullptr)
    {
        char patientName[MAX_STRING_LENGTH];
        char doctorName[MAX_STRING_LENGTH];
        getPatientName(current->patientId, patientName);
        getDoctorName(current->doctorId, doctorName);

        cout << current->appointmentId << "\t"
             << patientName << "\t\t"
             << doctorName << "\t\t"
             << current->date << "\t"
             << current->time << "\t"
             << current->status << "\n";

        current = current->next;
    }
}

//This function is use to cancel appointment, you may insert the status of the appointment such as cancel or completed
void manageAppointments::cancelAppointment()
{
    int appointmentId;
    cout << "\n-------------------------------";
    cout << "\nEnter Appointment ID to cancel: ";
    cin >> appointmentId;

    Appointment* current = head;
    while (current != nullptr)
    {
        if (current->appointmentId == appointmentId)
        {
            if (strcmp(current->status, "Cancelled") == 0)
            {
                cout << "\nAppointment is already cancelled.\n";
                return;
            }

            strcpy(current->status, "Cancelled");
            cout << "\nAppointment cancelled successfully!\n";

            saveToFile();
            return;
        }
        current = current->next;
    }

    cout << "\n***Appointment not found.***\n";
}
//This is the function to update the appointment status
void manageAppointments::updateAppointmentStatus()
{
    int appointmentId;
    cout << "\n-------------------------------\n";
    cout << "\nEnter Appointment ID to update: ";

    cin >> appointmentId;
    cin.ignore();

    Appointment* current = head;
    while (current != nullptr)
    {
        if (current->appointmentId == appointmentId)
        {
            cout << "Enter new status: ";

            char newStatus[20];
            cin.getline(newStatus, 20);

            strcpy(current->status, newStatus);
            cout << "\nStatus updated successfully!\n";

            saveToFile();
            return;
        }
        current = current->next;
    }

    cout << "\n***Appointment not found.***\n";
}

//This is use to searching the specific appointment user choose
void manageAppointments::searchAppointment()
{
    int searchId;
    cout << "\n-------------------------------";
    cout << "\nEnter Appointment ID to search: ";
    cin >> searchId;

Appointment* current = head;
    while (current != nullptr)
    {
        if (current->appointmentId == searchId)
        {
            char patientName[MAX_STRING_LENGTH];
            char doctorName[MAX_STRING_LENGTH];
            getPatientName(current->patientId, patientName);
            getDoctorName(current->doctorId, doctorName);

            system("cls"); //Change to the new pages
            cout << "\nAppointment Details:\n";
            cout << "ID: " << current->appointmentId << "\n";
            cout << "Patient: " << patientName << " (ID: " << current->patientId << ")\n";
            cout << "Doctor: " << doctorName << " (ID: " << current->doctorId << ")\n";
            cout << "Date: " << current->date << "\n";
            cout << "Time: " << current->time << "\n";
            cout << "Status: " << current->status << "\n";

            return;
        }
        current = current->next;
    }

    cout << "\n***Appointment not found.***\n";
}

// This is the File Operations for the appointment function

//use to save the file as well
void manageAppointments::saveToFile()
{
    FILE* file = fopen("appointments.dat", "wb");

    if (file == nullptr)
    {
        cout << "\nError opening file for writing.\n";
        return;
    }

    Appointment* current = head;

    while (current != nullptr)
    {
        fwrite(current, sizeof(Appointment), 1, file);
        current = current->next;
    }

    fclose(file);
}

//this function is use to load the file as well
void manageAppointments::loadFromFile()
{
    FILE* file = fopen("appointments.dat", "rb");

    if (file == nullptr) return;

    while (head != nullptr)
    {
        Appointment* temp = head;
        head = head->next;
        delete temp;
    }

    Appointment temp;
    while (fread(&temp, sizeof(Appointment), 1, file) == 1)
    {
        Appointment* newAppointment = new Appointment();
        *newAppointment = temp;
        newAppointment->next = nullptr;
        newAppointment->next = head;
        head = newAppointment;
    }

    fclose(file);
}
