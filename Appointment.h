#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include "doctor.h"
#include "patient.h"
#include <cstdio>
#include <cstring>

#define MAX_STRING_LENGTH 100
#define MAX_APPOINTMENTS 1000

struct Appointment {
    int appointmentId;
    int patientId;
    int doctorId;
    char date[11];
    char time[6];
    char status[20];
    Appointment* next;

    Appointment() : appointmentId(0), patientId(0), doctorId(0), next(nullptr) {
        strcpy(status, "Scheduled");
        date[0] = '\0';
        time[0] = '\0';
    }
};

struct AppointmentNode {
    Appointment* appointmentPtr;
    AppointmentNode* next;
    AppointmentNode(Appointment* apt) : appointmentPtr(apt), next(nullptr) {}
};

struct AppointmentMap {
    int id;
    AppointmentNode* appointments;
    AppointmentMap* next;
    AppointmentMap(int userId) : id(userId), appointments(nullptr), next(nullptr) {}
};

class manageAppointments {
private:
    Appointment* head;
    AppointmentMap* doctorAppointmentsHead;
    AppointmentMap* patientAppointmentsHead;
    manageDoctors* doctorSystem;
    managePatients* patientSystem;

    bool validateDate(const char* date);
    bool validateTime(const char* time);
    bool isDoctorAvailable(int doctorId, const char* date, const char* time);
    bool hasPatientAppointment(int patientId, const char* date, const char* time);
    void getDoctorName(int doctorId, char* name);
    void getPatientName(int patientId, char* name);
    bool verifyDoctorExists(int doctorId);
    bool verifyPatientExists(int patientId);

private:
    Appointment* mergeSortedLists(Appointment* a, Appointment* b);
    Appointment* getMiddle(Appointment* head);
    Appointment* mergeSort(Appointment* head);
    bool isEarlierDateTime(const char* date1, const char* time1, const char* date2, const char* time2);

public:
    manageAppointments(manageDoctors* docSys, managePatients* patSys);
    ~manageAppointments();

    void createAppointment();
    void displayAppointments();
    void cancelAppointment();
    void updateAppointmentStatus();
    void searchAppointment();
    void saveToFile();
    void loadFromFile();
};

#endif
