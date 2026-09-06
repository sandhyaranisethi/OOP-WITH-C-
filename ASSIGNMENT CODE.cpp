/*
 * ==============================================================
 *  Smart Hospital Bed & Patient Monitoring Management System
 *  DSA01 - Object Oriented Programming with C++
 * ==============================================================
 *  Demonstrates: Encapsulation, Inheritance, Polymorphism,
 *  Constructors (default/parameterized/copy), Destructors,
 *  Static Members, Collections of Objects, Operator Overloading,
 *  Control Structures, Menu-Driven Console Application.
 * ==============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
using namespace std;

// ==================================================================
// CLASS: Patient (Abstract Base Class)
// ==================================================================
class Patient {
protected:
    int patientID;
    string name;
    int age;
    string diagnosis;

    // Static members: shared hospital-level data across ALL patients
    static int totalPatients;

public:
    // Default constructor
    Patient() : patientID(0), name("Unknown"), age(0), diagnosis("N/A") {
        totalPatients++;
    }
    // Parameterized constructor
    Patient(int id, string n, int a, string d)
        : patientID(id), name(n), age(a), diagnosis(d) {
        totalPatients++;
    }
    // Copy constructor
    Patient(const Patient& other)
        : patientID(other.patientID), name(other.name),
          age(other.age), diagnosis(other.diagnosis) {
        totalPatients++;
    }

    int getPatientID() const { return patientID; }
    string getName() const { return name; }

    // Pure virtual functions -> overridden per patient category so the
    // correct version executes at RUN TIME through a base-class pointer.
    virtual double getDailyBedCharge() const = 0;
    virtual double getMonitoringUnitCharge() const = 0;
    virtual string getMonitoringRequirement() const = 0;
    virtual int getPriorityLevel() const = 0;      // 1 = highest priority
    virtual string getCategory() const = 0;

    virtual void display() const {
        cout << "  ID: " << patientID << " | Name: " << name
             << " | Age: " << age << " | Diagnosis: " << diagnosis
             << " | Category: " << getCategory()
             << " | Priority: " << getPriorityLevel();
    }

    // Static member functions
    static int getTotalPatients() { return totalPatients; }

    virtual ~Patient() {}   // virtual destructor -> correct derived cleanup
};
int Patient::totalPatients = 0;

// ------------------------------------------------------------------
class GeneralPatient : public Patient {
public:
    GeneralPatient() : Patient() {}
    GeneralPatient(int id, string n, int a, string d) : Patient(id, n, a, d) {}
    GeneralPatient(const GeneralPatient& other) : Patient(other) {}

    double getDailyBedCharge() const override { return 2000.0; }
    double getMonitoringUnitCharge() const override { return 300.0; }
    string getMonitoringRequirement() const override { return "Standard ward observation checks"; }
    int getPriorityLevel() const override { return 3; }
    string getCategory() const override { return "General"; }
};

class ICUPatient : public Patient {
public:
    ICUPatient() : Patient() {}
    ICUPatient(int id, string n, int a, string d) : Patient(id, n, a, d) {}
    ICUPatient(const ICUPatient& other) : Patient(other) {}

    double getDailyBedCharge() const override { return 7500.0; }
    double getMonitoringUnitCharge() const override { return 1000.0; }
    string getMonitoringRequirement() const override { return "Continuous vitals monitoring"; }
    int getPriorityLevel() const override { return 1; }
    string getCategory() const override { return "ICU"; }
};

class EmergencyPatient : public Patient {
public:
    EmergencyPatient() : Patient() {}
    EmergencyPatient(int id, string n, int a, string d) : Patient(id, n, a, d) {}
    EmergencyPatient(const EmergencyPatient& other) : Patient(other) {}

    double getDailyBedCharge() const override { return 5000.0; }
    double getMonitoringUnitCharge() const override { return 750.0; }
    string getMonitoringRequirement() const override { return "Rapid triage monitoring"; }
    int getPriorityLevel() const override { return 2; }
    string getCategory() const override { return "Emergency"; }
};

// ==================================================================
// CLASS: HospitalBed
// ==================================================================
class HospitalBed {
private:
    int bedNumber;
    string wardType;       // "General" | "ICU" | "Emergency"
    bool occupied;
    int occupantAdmissionID;

    static int occupiedBedCount;   // hospital-level shared static data

public:
    HospitalBed() : bedNumber(0), wardType("General"), occupied(false), occupantAdmissionID(-1) {}
    HospitalBed(int num, string ward)
        : bedNumber(num), wardType(ward), occupied(false), occupantAdmissionID(-1) {}

    int getBedNumber() const { return bedNumber; }
    string getWardType() const { return wardType; }
    bool isOccupied() const { return occupied; }

    void occupy(int admissionID) {
        occupied = true;
        occupantAdmissionID = admissionID;
        occupiedBedCount++;
    }
    void release() {
        occupied = false;
        occupantAdmissionID = -1;
        occupiedBedCount--;
    }

    void display() const {
        cout << "  Bed " << bedNumber << " [" << wardType << "] - "
             << (occupied ? "OCCUPIED" : "Available") << endl;
    }

    static int getOccupiedBedCount() { return occupiedBedCount; }
};
int HospitalBed::occupiedBedCount = 0;

// ==================================================================
// CLASS: Admission
// ==================================================================
class Admission {
private:
    int admissionID;
    int patientID;
    int bedNumber;
    int daysStayed;
    int monitoringUnits;
    bool active;
    Patient* patientRef;      // non-owning pointer to patient object
    double finalBill;

    static int totalAdmissions;    // static: shared hospital-level counter
    static int nextAdmissionID;

public:
    // Default constructor
    Admission()
        : admissionID(0), patientID(0), bedNumber(0), daysStayed(0),
          monitoringUnits(0), active(false), patientRef(nullptr), finalBill(0.0) {}

    // Parameterized constructor
    Admission(int pID, int bedNum, Patient* pRef)
        : patientID(pID), bedNumber(bedNum), daysStayed(1),
          monitoringUnits(0), active(true), patientRef(pRef), finalBill(0.0) {
        admissionID = nextAdmissionID++;
        totalAdmissions++;
    }

    // Copy constructor
    Admission(const Admission& other)
        : admissionID(other.admissionID), patientID(other.patientID),
          bedNumber(other.bedNumber), daysStayed(other.daysStayed),
          monitoringUnits(other.monitoringUnits), active(other.active),
          patientRef(other.patientRef), finalBill(other.finalBill) {}

    int getAdmissionID() const { return admissionID; }
    int getPatientID() const { return patientID; }
    int getBedNumber() const { return bedNumber; }
    bool isActive() const { return active; }

    void addMonitoringUnits(int units) { monitoringUnits += units; }
    void addDays(int days) { daysStayed += days; }

    double calculateCurrentBill() const {
        if (!patientRef) return 0.0;
        return (patientRef->getDailyBedCharge() * daysStayed) +
               (patientRef->getMonitoringUnitCharge() * monitoringUnits);
    }

    void dischargeAdmission() {
        finalBill = calculateCurrentBill();
        active = false;
    }

    double getFinalBill() const { return active ? calculateCurrentBill() : finalBill; }
    int getDaysStayed() const { return daysStayed; }

    // Static member functions
    static int getTotalAdmissions() { return totalAdmissions; }

    // Operator overload #1: compare two admissions by current/final bill,
    // tie-broken by length of stay -> used by "Compare Two Admissions"
    bool operator>(const Admission& other) const {
        double b1 = getFinalBill();
        double b2 = other.getFinalBill();
        if (b1 != b2) return b1 > b2;
        return daysStayed > other.daysStayed;
    }

    // Operator overload #2: format admission-record report output
    friend ostream& operator<<(ostream& os, const Admission& a);

    // Destructor: finalizes the admission and releases the bed as a
    // safety net if the record is still active when destroyed.
    ~Admission() {
        if (active) {
            active = false;
        }
    }
};
int Admission::totalAdmissions = 0;
int Admission::nextAdmissionID = 1;

ostream& operator<<(ostream& os, const Admission& a) {
    os << "  Admission #" << a.admissionID
       << " | Patient ID: " << a.patientID
       << " | Bed: " << a.bedNumber
       << " | Days: " << a.daysStayed
       << " | Monitoring Units: " << a.monitoringUnits
       << " | Status: " << (a.active ? "Active" : "Discharged")
       << " | Bill: Rs. " << fixed << setprecision(2) << a.getFinalBill();
    return os;
}

// ==================================================================
// CLASS: Hospital (aggregation / control logic / menu)
// ==================================================================
class Hospital {
private:
    vector<Patient*> patients;
    vector<HospitalBed> beds;
    vector<Admission*> admissions;
    int nextPatientID;

    Patient* findPatient(int id) {
        for (Patient* p : patients)
            if (p->getPatientID() == id) return p;
        return nullptr;
    }
    Admission* findAdmission(int id) {
        for (Admission* a : admissions)
            if (a->getAdmissionID() == id) return a;
        return nullptr;
    }
    HospitalBed* findFreeBed(const string& wardType) {
        for (auto& b : beds)
            if (b.getWardType() == wardType && !b.isOccupied()) return &b;
        return nullptr;
    }
    HospitalBed* findBedByNumber(int bedNum) {
        for (auto& b : beds)
            if (b.getBedNumber() == bedNum) return &b;
        return nullptr;
    }
    static int readInt(const string& prompt) {
        int val;
        while (true) {
            cout << prompt;
            if (cin >> val) { return val; }
            cout << "  Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

public:
    Hospital() : nextPatientID(501) {
        for (int i = 101; i <= 104; i++) beds.push_back(HospitalBed(i, "General"));
        for (int i = 201; i <= 203; i++) beds.push_back(HospitalBed(i, "ICU"));
        for (int i = 301; i <= 303; i++) beds.push_back(HospitalBed(i, "Emergency"));
    }

    ~Hospital() {
        for (Patient* p : patients) delete p;
        for (Admission* a : admissions) delete a;
    }

    // 1. Patient Registration
    void registerPatient() {
        cout << "\n-- Register Patient --\n";
        int cat = readInt("  Category (1-General, 2-ICU, 3-Emergency): ");
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Name: ";
        string name; getline(cin, name);
        int age = readInt("  Age: ");
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Diagnosis: ";
        string diag; getline(cin, diag);

        int id = nextPatientID++;
        Patient* p = nullptr;
        if (cat == 1) p = new GeneralPatient(id, name, age, diag);
        else if (cat == 2) p = new ICUPatient(id, name, age, diag);
        else if (cat == 3) p = new EmergencyPatient(id, name, age, diag);
        else { cout << "  Invalid category. Registration cancelled.\n"; nextPatientID--; return; }

        patients.push_back(p);
        cout << "  Patient registered successfully. Assigned Patient ID: " << id << endl;
    }

    // 2. Display All Patients
    void displayAllPatients() const {
        cout << "\n-- All Registered Patients --\n";
        if (patients.empty()) { cout << "  No patients registered yet.\n"; return; }
        for (Patient* p : patients) { p->display(); cout << endl; }
    }

    // 3. Display All Beds
    void displayAllBeds() const {
        cout << "\n-- Bed Status --\n";
        for (const auto& b : beds) b.display();
    }

    // 4. Admit Patient / Allocate Bed
    void admitPatient() {
        cout << "\n-- Admit Patient --\n";
        int id = readInt("  Enter Patient ID: ");
        Patient* p = findPatient(id);
        if (!p) { cout << "  Patient not found.\n"; return; }

        HospitalBed* bed = findFreeBed(p->getCategory());
        if (!bed) { cout << "  No available beds in " << p->getCategory() << " ward.\n"; return; }

        Admission* adm = new Admission(id, bed->getBedNumber(), p);
        bed->occupy(adm->getAdmissionID());
        admissions.push_back(adm);
        cout << "  Patient admitted. Admission ID: " << adm->getAdmissionID()
             << " | Bed Allocated: " << bed->getBedNumber() << endl;
    }

    // 5. Update Monitoring or Service Usage
    void updateMonitoring() {
        cout << "\n-- Update Monitoring / Service Usage --\n";
        int id = readInt("  Enter Admission ID: ");
        Admission* a = findAdmission(id);
        if (!a || !a->isActive()) { cout << "  Active admission not found.\n"; return; }
        int units = readInt("  Additional monitoring/service units to add: ");
        a->addMonitoringUnits(units);
        cout << "  Monitoring updated.\n";
    }

    // 6. Calculate Current Bill
    void calculateCurrentBill() {
        cout << "\n-- Calculate Current Bill --\n";
        int id = readInt("  Enter Admission ID: ");
        Admission* a = findAdmission(id);
        if (!a) { cout << "  Admission not found.\n"; return; }
        cout << "  Current Bill: Rs. " << fixed << setprecision(2)
             << a->calculateCurrentBill() << endl;
    }

    // 7. Patient Discharge
    void dischargePatient() {
        cout << "\n-- Discharge Patient --\n";
        int id = readInt("  Enter Admission ID: ");
        Admission* a = findAdmission(id);
        if (!a || !a->isActive()) { cout << "  Active admission not found.\n"; return; }
        a->dischargeAdmission();
        HospitalBed* bed = findBedByNumber(a->getBedNumber());
        if (bed) bed->release();
        cout << "  Patient discharged. Final Bill: Rs. " << fixed << setprecision(2)
             << a->getFinalBill() << " | Bed " << a->getBedNumber() << " released.\n";
    }

    // 8. Display Admission Records
    void displayAdmissionRecords() const {
        cout << "\n-- Admission Records --\n";
        if (admissions.empty()) { cout << "  No admission records yet.\n"; return; }
        for (Admission* a : admissions) cout << *a << endl;
    }

    // 9. Compare Two Admissions (operator> overload)
    void compareTwoAdmissions() {
        cout << "\n-- Compare Two Admissions --\n";
        int id1 = readInt("  Enter first Admission ID: ");
        int id2 = readInt("  Enter second Admission ID: ");
        Admission* a1 = findAdmission(id1);
        Admission* a2 = findAdmission(id2);
        if (!a1 || !a2) { cout << "  One or both admissions not found.\n"; return; }
        cout << "  " << *a1 << endl << "  " << *a2 << endl;
        if (*a1 > *a2) cout << "  Result: Admission #" << id1 << " has the higher bill/stay.\n";
        else if (*a2 > *a1) cout << "  Result: Admission #" << id2 << " has the higher bill/stay.\n";
        else cout << "  Result: Both admissions are equivalent.\n";
    }

    // 10. Generate Bed Utilization Report
    void generateUtilizationReport() const {
        cout << "\n-- Bed Utilization Report --\n";
        int total = (int)beds.size();
        int occupied = HospitalBed::getOccupiedBedCount();
        int available = total - occupied;
        double utilization = total > 0 ? (occupied * 100.0 / total) : 0.0;

        int activeAdmissions = 0;
        double revenue = 0.0;
        for (Admission* a : admissions) {
            if (a->isActive()) activeAdmissions++;
            revenue += a->getFinalBill();
        }

        cout << "  Total Beds            : " << total << endl;
        cout << "  Occupied Beds         : " << occupied << endl;
        cout << "  Available Beds        : " << available << endl;
        cout << "  Utilization %         : " << fixed << setprecision(1) << utilization << "%\n";
        cout << "  Active Admissions     : " << activeAdmissions << endl;
        cout << "  Total Revenue         : Rs. " << fixed << setprecision(2) << revenue << endl;
        cout << "  Total Patients (all-time)   : " << Patient::getTotalPatients() << endl;
        cout << "  Total Admissions (all-time) : " << Admission::getTotalAdmissions() << endl;
    }

    // Menu-driven interface
    void runMenu() {
        int choice;
        do {
            cout << "\n============ SMART HOSPITAL MANAGEMENT SYSTEM ============\n";
            cout << " 1. Register Patient\n";
            cout << " 2. Display All Patients\n";
            cout << " 3. Display All Beds\n";
            cout << " 4. Admit Patient / Allocate Bed\n";
            cout << " 5. Update Monitoring or Service Usage\n";
            cout << " 6. Calculate Current Bill\n";
            cout << " 7. Discharge Patient\n";
            cout << " 8. Display Admission Records\n";
            cout << " 9. Compare Two Admissions\n";
            cout << "10. Generate Bed Utilization Report\n";
            cout << " 0. Exit\n";
            choice = readInt("Enter your choice: ");

            switch (choice) {
                case 1: registerPatient(); break;
                case 2: displayAllPatients(); break;
                case 3: displayAllBeds(); break;
                case 4: admitPatient(); break;
                case 5: updateMonitoring(); break;
                case 6: calculateCurrentBill(); break;
                case 7: dischargePatient(); break;
                case 8: displayAdmissionRecords(); break;
                case 9: compareTwoAdmissions(); break;
                case 10: generateUtilizationReport(); break;
                case 0: cout << "Exiting. Thank you.\n"; break;
                default: cout << "Invalid choice. Please select a valid menu option.\n";
            }
        } while (choice != 0);
    }
};

// ==================================================================
// MAIN
// ==================================================================
int main() {
    Hospital hospital;
    hospital.runMenu();
    return 0;
}
