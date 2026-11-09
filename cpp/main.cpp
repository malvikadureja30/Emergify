#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <memory>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <sstream>

using namespace std;

// ============================================================
// EMERGIFY - SMART HOSPITAL MANAGEMENT SYSTEM
// C++17 console application demonstrating OOP concepts.
// ============================================================

// -------------------- INPUT HELPER FUNCTIONS --------------------

int readInt(const string& message) {
    int value;

    while (true) {
        cout << message;

        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Please enter a valid whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readLine(const string& message) {
    string value;
    cout << message;
    getline(cin, value);
    return value;
}

bool isValidPriority(int priority) {
    return priority >= 1 && priority <= 3;
}

// -------------------- PATIENT CLASS --------------------
// Encapsulation: patient data is private.
// Public methods control how the data is accessed and modified.

class Patient {
private:
    int id;
    string name;
    int age;
    string gender;
    string disease;
    int priority;
    string phone;

public:
    Patient(int patientId, const string& patientName, int patientAge,
            const string& patientGender, const string& patientDisease,
            int patientPriority, const string& patientPhone)
        : id(patientId),
          name(patientName),
          age(patientAge),
          gender(patientGender),
          disease(patientDisease),
          priority(patientPriority),
          phone(patientPhone) {}

    int getId() const { return id; }
    string getName() const { return name; }
    int getAge() const { return age; }
    string getGender() const { return gender; }
    string getDisease() const { return disease; }
    int getPriority() const { return priority; }
    string getPhone() const { return phone; }

    void updateDetails(const string& newName, int newAge,
                       const string& newGender, const string& newDisease,
                       int newPriority, const string& newPhone) {
        name = newName;
        age = newAge;
        gender = newGender;
        disease = newDisease;
        priority = newPriority;
        phone = newPhone;
    }

    void display() const {
        cout << "\n----------------------------------------\n";
        cout << "Patient ID       : " << id << '\n';
        cout << "Name             : " << name << '\n';
        cout << "Age              : " << age << '\n';
        cout << "Gender           : " << gender << '\n';
        cout << "Disease/Reason   : " << disease << '\n';
        cout << "Emergency Level  : ";

        if (priority == 1)
            cout << "Emergency\n";
        else if (priority == 2)
            cout << "Urgent\n";
        else
            cout << "Normal\n";

        cout << "Phone            : " << phone << '\n';
        cout << "----------------------------------------\n";
    }
};

// -------------------- ABSTRACT STAFF CLASS --------------------
// Abstraction: Staff is an abstract class because it contains
// a pure virtual function. It provides a common interface.

class Staff {
protected:
    int staffId;
    string name;

public:
    Staff(int id, const string& staffName)
        : staffId(id), name(staffName) {}

    int getStaffId() const { return staffId; }
    string getName() const { return name; }

    virtual void displayRole() const = 0;

    virtual ~Staff() = default;
};

// -------------------- DOCTOR CLASS --------------------
// Inheritance: Doctor derives from Staff.

class Doctor : public Staff {
private:
    string specialization;
    string availability;

public:
    Doctor(int id, const string& doctorName,
           const string& doctorSpecialization,
           const string& doctorAvailability)
        : Staff(id, doctorName),
          specialization(doctorSpecialization),
          availability(doctorAvailability) {}

    void displayRole() const override {
        cout << left << setw(12) << "Doctor"
             << setw(10) << staffId
             << setw(22) << name
             << setw(20) << specialization
             << availability << '\n';
    }
};

// -------------------- NURSE CLASS --------------------
// Inheritance: Nurse also derives from Staff.
// Polymorphism: Nurse provides its own displayRole() method.

class Nurse : public Staff {
private:
    string department;
    string shift;

public:
    Nurse(int id, const string& nurseName,
          const string& nurseDepartment,
          const string& nurseShift)
        : Staff(id, nurseName),
          department(nurseDepartment),
          shift(nurseShift) {}

    void displayRole() const override {
        cout << left << setw(12) << "Nurse"
             << setw(10) << staffId
             << setw(22) << name
             << setw(20) << department
             << shift << '\n';
    }
};

// -------------------- BED CLASS --------------------
// Each Bed object stores its own availability and patient assignment.

class Bed {
private:
    int bedId;
    string ward;
    bool occupied;
    int patientId;

public:
    Bed(int id, const string& bedWard)
        : bedId(id), ward(bedWard), occupied(false), patientId(-1) {}

    int getBedId() const { return bedId; }
    int getPatientId() const { return patientId; }
    string getWard() const { return ward; }
    bool isAvailable() const { return !occupied; }

    bool allocate(int id) {
        if (occupied)
            return false;

        occupied = true;
        patientId = id;
        return true;
    }

    bool release() {
        if (!occupied)
            return false;

        occupied = false;
        patientId = -1;
        return true;
    }

    void display() const {
        cout << left << setw(10) << bedId
             << setw(18) << ward;

        if (occupied)
            cout << "Occupied (Patient ID: " << patientId << ")\n";
        else
            cout << "Available\n";
    }
};

// -------------------- TREATMENT CLASS --------------------
// Stores a treatment or visit entry for a patient.

class Treatment {
private:
    int treatmentId;
    int patientId;
    string date;
    string description;
    string doctorName;
    string status;

public:
    Treatment(int tId, int pId, const string& treatmentDate,
              const string& treatmentDescription,
              const string& assignedDoctor, const string& treatmentStatus)
        : treatmentId(tId),
          patientId(pId),
          date(treatmentDate),
          description(treatmentDescription),
          doctorName(assignedDoctor),
          status(treatmentStatus) {}

    int getTreatmentId() const { return treatmentId; }
    int getPatientId() const { return patientId; }
    string getDate() const { return date; }
    string getDescription() const { return description; }
    string getDoctorName() const { return doctorName; }
    string getStatus() const { return status; }

    void display() const {
        cout << "\nTreatment ID : " << treatmentId
             << "\nPatient ID   : " << patientId
             << "\nDate         : " << date
             << "\nDescription  : " << description
             << "\nDoctor       : " << doctorName
             << "\nStatus       : " << status
             << "\n----------------------------------------\n";
    }
};

// -------------------- HOSPITAL CLASS --------------------
// Composition/aggregation: Hospital manages collections of
// Patient, Staff, Bed and Treatment objects.

class Hospital {
private:
    vector<Patient> patients;
    vector<unique_ptr<Staff>> staff;
    vector<Bed> beds;
    vector<Treatment> treatments;

    // A smaller number represents a more urgent case.
    // Pair format: {priority, patient ID}.
    using EmergencyEntry = pair<int, int>;

    priority_queue<
        EmergencyEntry,
        vector<EmergencyEntry>,
        greater<EmergencyEntry>
    > emergencyQueue;

    int nextTreatmentId = 1;

    Patient* findPatientById(int id) {
        for (auto& patient : patients) {
            if (patient.getId() == id)
                return &patient;
        }
        return nullptr;
    }

    const Patient* findPatientById(int id) const {
        for (const auto& patient : patients) {
            if (patient.getId() == id)
                return &patient;
        }
        return nullptr;
    }

    bool patientHasBed(int patientId) const {
        for (const auto& bed : beds) {
            if (bed.getPatientId() == patientId)
                return true;
        }
        return false;
    }

    void rebuildEmergencyQueue() {
        emergencyQueue = priority_queue<
            EmergencyEntry,
            vector<EmergencyEntry>,
            greater<EmergencyEntry>
        >();

        for (const auto& patient : patients) {
            emergencyQueue.push(
                {patient.getPriority(), patient.getId()}
            );
        }
    }

    void savePatients() const {
        ofstream file("patients.txt");

        if (!file) {
            cout << "Warning: Could not save patient records.\n";
            return;
        }

        for (const auto& p : patients) {
            file << p.getId() << ' '
                 << quoted(p.getName()) << ' '
                 << p.getAge() << ' '
                 << quoted(p.getGender()) << ' '
                 << quoted(p.getDisease()) << ' '
                 << p.getPriority() << ' '
                 << quoted(p.getPhone()) << '\n';
        }
    }

    void loadPatients() {
        ifstream file("patients.txt");

        if (!file)
            return;

        int id, age, priority;
        string name, gender, disease, phone;

        while (file >> id >> quoted(name) >> age
                    >> quoted(gender) >> quoted(disease)
                    >> priority >> quoted(phone)) {
            if (id > 0 && age >= 0 && isValidPriority(priority)) {
                patients.emplace_back(
                    id, name, age, gender, disease, priority, phone
                );
            }
        }

        rebuildEmergencyQueue();
    }

    void saveTreatments() const {
        ofstream file("treatments.txt");

        if (!file) {
            cout << "Warning: Could not save treatment records.\n";
            return;
        }

        for (const auto& t : treatments) {
            // Treatment records are persisted when they are added.
            // Accessors below keep the class data encapsulated.
            file << t.getTreatmentId() << ' '
                 << t.getPatientId() << ' '
                 << quoted(t.getDate()) << ' '
                 << quoted(t.getDescription()) << ' '
                 << quoted(t.getDoctorName()) << ' '
                 << quoted(t.getStatus()) << '\n';
        }
    }

    void loadTreatments() {
        ifstream file("treatments.txt");
        if (!file)
            return;

        int treatmentId, patientId;
        string date, description, doctor, status;

        while (file >> treatmentId >> patientId
                    >> quoted(date) >> quoted(description)
                    >> quoted(doctor) >> quoted(status)) {
            if (findPatientById(patientId) != nullptr) {
                treatments.emplace_back(treatmentId, patientId, date,
                                         description, doctor, status);
                nextTreatmentId = max(nextTreatmentId, treatmentId + 1);
            }
        }
    }

    void releasePatientBed(int patientId) {
        for (auto& bed : beds) {
            if (bed.getPatientId() == patientId) {
                bed.release();
                cout << "Bed " << bed.getBedId()
                     << " has been released.\n";
                return;
            }
        }
    }

public:
    Hospital() {
        // Create sample beds in different wards.
        beds.emplace_back(1, "General Ward");
        beds.emplace_back(2, "General Ward");
        beds.emplace_back(3, "Emergency");
        beds.emplace_back(4, "Emergency");
        beds.emplace_back(5, "ICU");

        // Add sample staff members.
        staff.push_back(make_unique<Doctor>(
            101, "Dr. Sharma", "Cardiology", "Available"
        ));

        staff.push_back(make_unique<Doctor>(
            102, "Dr. Mehta", "Neurology", "Available"
        ));

        staff.push_back(make_unique<Nurse>(
            201, "Riya", "Emergency", "Morning"
        ));

        staff.push_back(make_unique<Nurse>(
            202, "Ananya", "General Ward", "Evening"
        ));

        loadPatients();
        loadTreatments();
    }

    // -------------------- PATIENT REGISTRATION --------------------

    void addPatient() {
        cout << "\n========== PATIENT REGISTRATION ==========\n";

        int id = readInt("Enter patient ID: ");

        if (id <= 0) {
            cout << "Patient ID must be positive.\n";
            return;
        }

        if (findPatientById(id) != nullptr) {
            cout << "This patient ID already exists.\n";
            return;
        }

        string name = readLine("Enter patient name: ");
        int age = readInt("Enter age: ");

        if (name.empty() || age < 0 || age > 120) {
            cout << "Invalid name or age. Registration cancelled.\n";
            return;
        }

        string gender = readLine("Enter gender: ");
        string disease = readLine("Enter disease/reason for visit: ");
        string phone = readLine("Enter contact number: ");

        cout << "\nPriority levels:\n";
        cout << "1. Emergency\n2. Urgent\n3. Normal\n";

        int priority = readInt("Select priority: ");

        if (!isValidPriority(priority)) {
            cout << "Invalid priority. Registration cancelled.\n";
            return;
        }

        patients.emplace_back(
            id, name, age, gender, disease, priority, phone
        );

        emergencyQueue.push({priority, id});
        savePatients();

        cout << "\nPatient registered successfully!\n";
    }

    // -------------------- DISPLAY ALL PATIENTS --------------------

    void displayPatients() const {
        cout << "\n========== REGISTERED PATIENTS ==========\n";

        if (patients.empty()) {
            cout << "No patient records are available.\n";
            return;
        }

        for (const auto& patient : patients)
            patient.display();

        cout << "Total registered patients: " << patients.size() << '\n';
    }

    // -------------------- SEARCH PATIENT --------------------

    void searchPatient() const {
        int id = readInt("\nEnter patient ID to search: ");
        const Patient* patient = findPatientById(id);

        if (patient != nullptr) {
            cout << "\nPatient record found:\n";
            patient->display();
        } else {
            cout << "No patient found with ID " << id << ".\n";
        }
    }

    // -------------------- UPDATE PATIENT DETAILS --------------------

    void updatePatient() {
        int id = readInt("\nEnter patient ID to update: ");
        Patient* patient = findPatientById(id);

        if (patient == nullptr) {
            cout << "Patient not found.\n";
            return;
        }

        cout << "\nEnter the updated details.\n";

        string name = readLine("New name: ");
        int age = readInt("New age: ");
        string gender = readLine("New gender: ");
        string disease = readLine("New disease/reason: ");
        string phone = readLine("New contact number: ");

        cout << "1. Emergency\n2. Urgent\n3. Normal\n";
        int priority = readInt("New priority: ");

        if (name.empty() || age < 0 || age > 120 ||
            !isValidPriority(priority)) {
            cout << "Invalid information. Update cancelled.\n";
            return;
        }

        patient->updateDetails(
            name, age, gender, disease, priority, phone
        );

        rebuildEmergencyQueue();
        savePatients();

        cout << "Patient information updated successfully.\n";
    }

    // -------------------- DISCHARGE PATIENT --------------------

    void dischargePatient() {
        int id = readInt("\nEnter patient ID to discharge: ");

        auto it = find_if(
            patients.begin(),
            patients.end(),
            [id](const Patient& patient) {
                return patient.getId() == id;
            }
        );

        if (it == patients.end()) {
            cout << "Patient not found.\n";
            return;
        }

        releasePatientBed(id);

        patients.erase(it);
        rebuildEmergencyQueue();
        savePatients();

        cout << "Patient discharged successfully.\n";
    }

    // -------------------- BED ALLOCATION --------------------

    void allocateBed() {
        int patientId = readInt("\nEnter patient ID for bed allocation: ");

        if (findPatientById(patientId) == nullptr) {
            cout << "Register the patient before allocating a bed.\n";
            return;
        }

        if (patientHasBed(patientId)) {
            cout << "This patient already has an allocated bed.\n";
            return;
        }

        for (auto& bed : beds) {
            if (bed.isAvailable()) {
                bed.allocate(patientId);

                cout << "Bed " << bed.getBedId()
                     << " in " << bed.getWard()
                     << " allocated successfully.\n";
                return;
            }
        }

        cout << "No beds are currently available.\n";
    }

    // -------------------- BED RELEASE --------------------

    void releaseBed() {
        int patientId = readInt("\nEnter patient ID to release the bed: ");

        for (auto& bed : beds) {
            if (bed.getPatientId() == patientId) {
                bed.release();
                cout << "Bed " << bed.getBedId()
                     << " released successfully.\n";
                return;
            }
        }

        cout << "No bed allocation found for this patient.\n";
    }

    // -------------------- DISPLAY BED STATUS --------------------

    void displayBeds() const {
        cout << "\n========== BED AVAILABILITY ==========\n";
        cout << left << setw(10) << "Bed ID"
             << setw(18) << "Ward"
             << "Status\n";

        for (const auto& bed : beds)
            bed.display();

        int available = 0;

        for (const auto& bed : beds) {
            if (bed.isAvailable())
                available++;
        }

        cout << "\nTotal beds: " << beds.size()
             << "\nAvailable beds: " << available
             << "\nOccupied beds: " << beds.size() - available << '\n';
    }

    // -------------------- DISPLAY STAFF --------------------
    // Runtime polymorphism: Staff pointers call the overridden
    // displayRole() function of Doctor or Nurse.

    void displayStaff() const {
        cout << "\n========== HOSPITAL STAFF ==========\n";

        cout << left << setw(12) << "Role"
             << setw(10) << "ID"
             << setw(22) << "Name"
             << setw(20) << "Department"
             << "Availability/Shift\n";

        for (const auto& member : staff)
            member->displayRole();
    }

    // -------------------- ADD TREATMENT RECORD --------------------

    void addTreatment() {
        int patientId = readInt("\nEnter patient ID: ");

        if (findPatientById(patientId) == nullptr) {
            cout << "Patient not found. Add the patient first.\n";
            return;
        }

        string date = readLine("Enter date (DD-MM-YYYY): ");
        string description = readLine("Enter treatment/consultation details: ");
        string doctor = readLine("Enter doctor name: ");
        string status = readLine("Enter status (Ongoing/Completed): ");

        if (date.empty() || description.empty() ||
            doctor.empty() || status.empty()) {
            cout << "Treatment details cannot be empty.\n";
            return;
        }

        treatments.emplace_back(
            nextTreatmentId++,
            patientId,
            date,
            description,
            doctor,
            status
        );

        saveTreatments();
        cout << "Treatment record added and saved successfully.\n";
    }

    // -------------------- PATIENT TREATMENT HISTORY --------------------

    void showTreatmentHistory() const {
        int patientId = readInt("\nEnter patient ID: ");

        if (findPatientById(patientId) == nullptr) {
            cout << "Patient not found in current records.\n";
            return;
        }

        bool found = false;

        cout << "\n========== TREATMENT HISTORY ==========\n";

        for (const auto& treatment : treatments) {
            if (treatment.getPatientId() == patientId) {
                treatment.display();
                found = true;
            }
        }

        if (!found)
            cout << "No treatment history recorded for this patient.\n";
    }

    // -------------------- EMERGENCY PRIORITY QUEUE --------------------
    // Priority queue displays priority 1 before priority 2 and 3.

    void displayEmergencyQueue() const {
        if (emergencyQueue.empty()) {
            cout << "\nThe emergency queue is empty.\n";
            return;
        }

        auto tempQueue = emergencyQueue;

        cout << "\n========== EMERGENCY PRIORITY QUEUE ==========\n";

        while (!tempQueue.empty()) {
            int priority = tempQueue.top().first;
            int id = tempQueue.top().second;
            tempQueue.pop();

            const Patient* patient = findPatientById(id);

            // Ignore any stale queue entry.
            if (patient == nullptr || patient->getPriority() != priority)
                continue;

            cout << "Patient ID: " << id
                 << " | Name: " << patient->getName()
                 << " | Priority: ";

            if (priority == 1)
                cout << "Emergency";
            else if (priority == 2)
                cout << "Urgent";
            else
                cout << "Normal";

            cout << '\n';
        }
    }

    // -------------------- HOSPITAL STATISTICS --------------------

    void displayStatistics() const {
        int emergency = 0;
        int urgent = 0;
        int normal = 0;
        int availableBeds = 0;

        for (const auto& patient : patients) {
            if (patient.getPriority() == 1)
                emergency++;
            else if (patient.getPriority() == 2)
                urgent++;
            else
                normal++;
        }

        for (const auto& bed : beds) {
            if (bed.isAvailable())
                availableBeds++;
        }

        cout << "\n========== HOSPITAL STATISTICS ==========\n";
        cout << "Total registered patients : " << patients.size() << '\n';
        cout << "Emergency patients        : " << emergency << '\n';
        cout << "Urgent patients            : " << urgent << '\n';
        cout << "Normal patients            : " << normal << '\n';
        cout << "Total beds                 : " << beds.size() << '\n';
        cout << "Available beds             : " << availableBeds << '\n';
        cout << "Occupied beds              : "
             << beds.size() - availableBeds << '\n';
        cout << "Treatment records          : " << treatments.size() << '\n';
        cout << "Staff members              : " << staff.size() << '\n';
    }

    // -------------------- SAVE DATA --------------------

    void saveAllData() const {
        savePatients();
        saveTreatments();
        cout << "Patient records saved to patients.txt.\n";
        cout << "Treatment records saved to treatments.txt.\n";
    }
};

// -------------------- MAIN MENU --------------------

void displayMenu() {
    cout << "\n\n";
    cout << "====================================================\n";
    cout << "          EMERGIFY - SMART HOSPITAL SYSTEM           \n";
    cout << "====================================================\n";
    cout << "  PATIENT MANAGEMENT\n";
    cout << "  1. Register New Patient\n";
    cout << "  2. Display All Patients\n";
    cout << "  3. Search Patient\n";
    cout << "  4. Update Patient Details\n";
    cout << "  5. Discharge Patient\n";
    cout << "\n  STAFF AND BED MANAGEMENT\n";
    cout << "  6. Display Doctors and Nurses\n";
    cout << "  7. Allocate Bed\n";
    cout << "  8. Release Bed\n";
    cout << "  9. Display Bed Availability\n";
    cout << "\n  TREATMENT AND EMERGENCY MANAGEMENT\n";
    cout << " 10. Add Treatment Record\n";
    cout << " 11. View Patient Treatment History\n";
    cout << " 12. Display Emergency Priority Queue\n";
    cout << "\n  REPORTS\n";
    cout << " 13. Display Hospital Statistics\n";
    cout << " 14. Save Records\n";
    cout << "  0. Exit\n";
    cout << "====================================================\n";
}

int main() {
    Hospital hospital;

    cout << "\nWelcome to Emergify Smart Hospital Management System!\n";

    int choice;

    do {
        displayMenu();
        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:
                hospital.addPatient();
                break;

            case 2:
                hospital.displayPatients();
                break;

            case 3:
                hospital.searchPatient();
                break;

            case 4:
                hospital.updatePatient();
                break;

            case 5:
                hospital.dischargePatient();
                break;

            case 6:
                hospital.displayStaff();
                break;

            case 7:
                hospital.allocateBed();
                break;

            case 8:
                hospital.releaseBed();
                break;

            case 9:
                hospital.displayBeds();
                break;

            case 10:
                hospital.addTreatment();
                break;

            case 11:
                hospital.showTreatmentHistory();
                break;

            case 12:
                hospital.displayEmergencyQueue();
                break;

            case 13:
                hospital.displayStatistics();
                break;

            case 14:
                hospital.saveAllData();
                break;

            case 0:
                hospital.saveAllData();
                cout << "\nThank you for using Emergify!\n";
                cout << "Program closed successfully.\n";
                break;

            default:
                cout << "Invalid option. Please select from the menu.\n";
        }

    } while (choice != 0);

    return 0;
}
