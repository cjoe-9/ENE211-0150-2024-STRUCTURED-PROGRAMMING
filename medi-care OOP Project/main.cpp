/*
 * ============================================================
 *  MedTrack — Medication Reminder & Prescription Tracker
 *  Interactive Console Application
 * ============================================================
 *  OOP Concepts:
 *   - Encapsulation   : private data + public getters/setters
 *   - Abstraction     : abstract base class (Reminder)
 *   - Polymorphism    : virtual functions, runtime dispatch
 *   - Classes/Objects : Medicine, Prescription, DosageSchedule
 *   - Linear Search   : searchByName()
 *   - Binary Search   : binarySearchByID() on sorted vector
 *   - Memory Mgmt     : stack objects, heap via smart pointers,
 *                       raw new/delete demonstration
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <ctime>
#include <iomanip>
#include <limits>

using namespace std;

// ─────────────────────────────────────────────────────────────
//  HELPER UTILITIES
// ─────────────────────────────────────────────────────────────
string currentTime() {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&now));
    return string(buf);
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pause() {
    cout << "\n  Press ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printLine(char c = '-', int len = 54) {
    cout << "  " << string(len, c) << "\n";
}

void printHeader(const string& title) {
    cout << "\n";
    printLine('=');
    cout << "   " << title << "\n";
    printLine('=');
}

int getIntInput(const string& prompt, int minv = 0, int maxv = 99999) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= minv && val <= maxv) {
            clearInput();
            return val;
        }
        cout << "  Invalid. Enter a number between "
             << minv << " and " << maxv << ".\n";
        clearInput();
    }
}

string getStringInput(const string& prompt) {
    string val;
    cout << prompt;
    getline(cin, val);
    size_t s = val.find_first_not_of(' ');
    size_t e = val.find_last_not_of(' ');
    return (s == string::npos) ? "" : val.substr(s, e - s + 1);
}

// ─────────────────────────────────────────────────────────────
//  ABSTRACTION — Abstract Base Class
// ─────────────────────────────────────────────────────────────
class Reminder {
protected:
    string message;
    bool   triggered;
public:
    Reminder(const string& msg) : message(msg), triggered(false) {}
    virtual ~Reminder() {}
    virtual void trigger()         = 0;
    virtual string getType() const = 0;
    bool   isTriggered() const { return triggered; }
    string getMessage()  const { return message;   }
};

// ─────────────────────────────────────────────────────────────
//  POLYMORPHISM — Concrete Reminder types
// ─────────────────────────────────────────────────────────────
class DoseReminder : public Reminder {
    string medicineName, scheduledTime;
public:
    DoseReminder(const string& med, const string& t)
        : Reminder("Take " + med + " at " + t),
          medicineName(med), scheduledTime(t) {}
    void trigger() override {
        triggered = true;
        cout << "  [DOSE ALERT]  " << message
             << "  (" << currentTime() << ")\n";
    }
    string getType() const override { return "Dose Reminder"; }
};

class MissedDoseReminder : public Reminder {
public:
    MissedDoseReminder(const string& patient, const string& med)
        : Reminder("MISSED: " + patient + " skipped " + med) {}
    void trigger() override {
        triggered = true;
        cout << "  [!! MISSED !!] " << message
             << " -- caregiver notified (" << currentTime() << ")\n";
    }
    string getType() const override { return "Missed Dose Alert"; }
};

class RefillReminder : public Reminder {
public:
    RefillReminder(const string& med, int qty)
        : Reminder("Low stock: " + to_string(qty) +
                   " doses of " + med + " remaining") {}
    void trigger() override {
        triggered = true;
        cout << "  [REFILL]  " << message
             << "  (" << currentTime() << ")\n";
    }
    string getType() const override { return "Refill Reminder"; }
};

// ─────────────────────────────────────────────────────────────
//  ENCAPSULATION — Medicine  (stack-allocated in vector)
// ─────────────────────────────────────────────────────────────
class Medicine {
    int    id;
    string name;
    string dosage;
    string frequency;
    int    stockCount;
    bool   active;
public:
    Medicine(int id, const string& name, const string& dosage,
             const string& freq, int stock)
        : id(id), name(name), dosage(dosage),
          frequency(freq), stockCount(stock), active(true) {}

    int    getID()        const { return id;         }
    string getName()      const { return name;       }
    string getDosage()    const { return dosage;     }
    string getFrequency() const { return frequency;  }
    int    getStock()     const { return stockCount; }
    bool   isActive()     const { return active;     }

    void setActive(bool s)             { active = s;      }
    void setStock(int s)               { stockCount = s;  }
    void decrementStock()              { if (stockCount > 0) --stockCount; }
    void setFrequency(const string& f) { frequency = f;   }

    void display() const {
        cout << "  [" << setw(4) << id << "]  "
             << setw(20) << left  << name
             << setw(10) << dosage
             << setw(16) << frequency
             << "Stock: " << setw(5) << stockCount
             << (active ? "  Active" : "  Inactive") << "\n";
    }
};

// ─────────────────────────────────────────────────────────────
//  ENCAPSULATION — DosageSchedule  (heap via unique_ptr)
// ─────────────────────────────────────────────────────────────
class DosageSchedule {
    string         medicineName;
    vector<string> times;
    vector<bool>   taken;
    int            missedCount;
public:
    DosageSchedule(const string& med, const vector<string>& t)
        : medicineName(med), times(t),
          taken(t.size(), false), missedCount(0) {}

    string getMedicineName() const { return medicineName;       }
    int    getMissedCount()  const { return missedCount;        }
    int    getSlotCount()    const { return (int)times.size();  }

    bool markTaken(int slot) {
        if (slot < 0 || slot >= (int)times.size()) return false;
        if (taken[slot]) {
            cout << "  Slot " << slot << " already marked taken.\n";
            return false;
        }
        taken[slot] = true;
        return true;
    }

    vector<unique_ptr<Reminder>> checkMissed(const string& patient) {
        vector<unique_ptr<Reminder>> alerts;
        for (int i = 0; i < (int)times.size(); ++i) {
            if (!taken[i]) {
                ++missedCount;
                alerts.push_back(
                    make_unique<MissedDoseReminder>(patient, medicineName));
            }
        }
        return alerts;
    }

    void display() const {
        cout << "  Medicine : " << medicineName << "\n";
        for (int i = 0; i < (int)times.size(); ++i) {
            cout << "    Slot " << i << " | " << times[i]
                 << (taken[i] ? "  [TAKEN]" : "  [PENDING]") << "\n";
        }
    }
};

// ─────────────────────────────────────────────────────────────
//  ENCAPSULATION — Prescription
// ─────────────────────────────────────────────────────────────
class Prescription {
    int    rxID;
    string patientName;
    string doctorName;
    string issueDate;
    string expiryDate;
    vector<shared_ptr<Medicine>> medicines;
public:
    Prescription(int id, const string& patient, const string& doctor,
                 const string& issue, const string& expiry)
        : rxID(id), patientName(patient), doctorName(doctor),
          issueDate(issue), expiryDate(expiry) {}

    int    getRxID()    const { return rxID;        }
    string getPatient() const { return patientName; }

    void addMedicine(shared_ptr<Medicine> m) { medicines.push_back(m); }

    void display() const {
        printLine('=');
        cout << "  Rx #" << rxID << "  |  Patient : " << patientName << "\n";
        cout << "  Doctor  : " << doctorName << "\n";
        cout << "  Issued  : " << issueDate
             << "   Expires : " << expiryDate << "\n";
        printLine('-');
        cout << "  " << setw(6) << "ID"
             << setw(20) << "Medicine"
             << setw(10) << "Dosage"
             << "Frequency\n";
        printLine('-');
        for (auto& m : medicines) m->display();
        printLine('=');
    }
};

// ─────────────────────────────────────────────────────────────
//  SEARCH ALGORITHMS
// ─────────────────────────────────────────────────────────────
Medicine* linearSearchByName(vector<Medicine>& meds, const string& name) {
    for (auto& m : meds)
        if (m.getName() == name) return &m;
    return nullptr;
}

int binarySearchByID(const vector<Medicine>& meds, int targetID) {
    int lo = 0, hi = (int)meds.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if      (meds[mid].getID() == targetID) return mid;
        else if (meds[mid].getID() <  targetID) lo = mid + 1;
        else                                    hi = mid - 1;
    }
    return -1;
}

// ─────────────────────────────────────────────────────────────
//  MedTracker — Main Application
// ─────────────────────────────────────────────────────────────
class MedTracker {
    vector<Medicine>     medicines;       // STACK (value in vector)
    vector<Prescription> prescriptions;  // STACK

    vector<unique_ptr<Reminder>>       activeReminders; // HEAP
    vector<unique_ptr<DosageSchedule>> schedules;       // HEAP

    int nextMedID = 100;
    int nextRxID  = 1;

public:

    // 1. Add Medicine
    void addMedicine() {
        printHeader("ADD NEW MEDICINE");
        string name = getStringInput("  Medicine name        : ");
        if (name.empty()) { cout << "  Name cannot be empty.\n"; return; }
        string dosage = getStringInput("  Dosage (e.g. 500mg)  : ");
        string freq   = getStringInput("  Frequency            : ");
        int    stock  = getIntInput   ("  Stock count          : ", 0, 9999);

        medicines.emplace_back(nextMedID, name, dosage, freq, stock);
        cout << "  Added: " << name << "  (ID " << nextMedID << ")\n";
        nextMedID++;

        if (stock <= 5) {
            activeReminders.push_back(
                make_unique<RefillReminder>(name, stock));
            cout << "  Low stock detected -- refill reminder created.\n";
        }
    }

    // 2. List medicines
    void listMedicines() const {
        printHeader("MEDICINE INVENTORY");
        if (medicines.empty()) { cout << "  No medicines recorded.\n"; return; }
        cout << "  " << setw(6) << "ID"
             << setw(20) << left << "Name"
             << setw(10) << "Dosage"
             << setw(16) << "Frequency"
             << "Stock\n";
        printLine();
        for (auto& m : medicines) m.display();
    }

    // 3. Update stock
    void updateStock() {
        printHeader("UPDATE STOCK");
        if (medicines.empty()) { cout << "  No medicines.\n"; return; }
        listMedicines();
        int id = getIntInput("\n  Enter medicine ID : ", 0, 99999);
        sort(medicines.begin(), medicines.end(),
             [](const Medicine& a, const Medicine& b){
                 return a.getID() < b.getID(); });
        int idx = binarySearchByID(medicines, id);
        if (idx == -1) { cout << "  Medicine not found.\n"; return; }
        int s = getIntInput("  New stock count  : ", 0, 9999);
        medicines[idx].setStock(s);
        cout << "  Stock updated to " << s << "\n";
    }

    // 4. Add prescription
    void addPrescription() {
        printHeader("ADD PRESCRIPTION");
        if (medicines.empty()) {
            cout << "  Add medicines first.\n"; return;
        }
        listMedicines();
        string patient = getStringInput("\n  Patient name : ");
        string doctor  = getStringInput("  Doctor name  : ");
        string issue   = getStringInput("  Issue date   : ");
        string expiry  = getStringInput("  Expiry date  : ");

        Prescription rx(nextRxID, patient, doctor, issue, expiry);

        cout << "\n  Enter medicine IDs to add (0 to finish):\n";
        while (true) {
            int id = getIntInput("  Medicine ID : ", 0, 99999);
            if (id == 0) break;
            sort(medicines.begin(), medicines.end(),
                 [](const Medicine& a, const Medicine& b){
                     return a.getID() < b.getID(); });
            int idx = binarySearchByID(medicines, id); // Binary Search
            if (idx != -1) {
                rx.addMedicine(make_shared<Medicine>(medicines[idx]));
                cout << "  Added: " << medicines[idx].getName() << "\n";
            } else {
                cout << "  ID " << id << " not found.\n";
            }
        }
        prescriptions.push_back(rx);
        cout << "  Prescription Rx#" << nextRxID
             << " saved for " << patient << "\n";
        nextRxID++;
    }

    // 5. List prescriptions
    void listPrescriptions() const {
        printHeader("STORED PRESCRIPTIONS");
        if (prescriptions.empty()) {
            cout << "  No prescriptions stored.\n"; return;
        }
        for (auto& rx : prescriptions) rx.display();
    }

    // 6. Add schedule
    void addSchedule() {
        printHeader("ADD DOSAGE SCHEDULE");
        if (medicines.empty()) { cout << "  Add medicines first.\n"; return; }
        listMedicines();

        string medName = getStringInput("\n  Medicine name : ");
        Medicine* found = linearSearchByName(medicines, medName); // Linear Search
        if (!found) {
            cout << "  \"" << medName << "\" not found (linear search).\n";
            return;
        }

        int count = getIntInput("  Doses per day  : ", 1, 10);
        vector<string> times;
        for (int i = 0; i < count; ++i) {
            string t = getStringInput("  Time for dose " +
                                      to_string(i) + " (e.g. 08:00) : ");
            times.push_back(t);
            activeReminders.push_back(   // Heap allocation
                make_unique<DoseReminder>(medName, t));
        }
        schedules.push_back(make_unique<DosageSchedule>(medName, times));
        cout << "  Schedule set for " << medName << "\n";
    }

    // 7. List schedules
    void listSchedules() const {
        printHeader("DOSAGE SCHEDULES");
        if (schedules.empty()) { cout << "  No schedules set.\n"; return; }
        for (auto& s : schedules) { s->display(); cout << "\n"; }
    }

    // 8. Mark dose taken
    void markDoseTaken() {
        printHeader("MARK DOSE AS TAKEN");
        if (schedules.empty()) {
            cout << "  No schedules found.\n"; return;
        }
        listSchedules();
        string medName = getStringInput("  Medicine name : ");
        for (auto& s : schedules) {
            if (s->getMedicineName() == medName) {
                int slot = getIntInput(
                    "  Slot number   : ", 0, s->getSlotCount() - 1);
                if (s->markTaken(slot)) {
                    cout << "  Dose recorded.\n";
                    Medicine* m = linearSearchByName(medicines, medName);
                    if (m) {
                        m->decrementStock();
                        cout << "  Stock remaining: " << m->getStock() << "\n";
                        if (m->getStock() <= 3) {
                            activeReminders.push_back(
                                make_unique<RefillReminder>(
                                    medName, m->getStock()));
                            cout << "  Refill reminder triggered!\n";
                        }
                    }
                }
                return;
            }
        }
        cout << "  No schedule for \"" << medName << "\".\n";
    }

    // 9. Check missed doses
    void checkMissedDoses() {
        printHeader("CHECK MISSED DOSES");
        if (schedules.empty()) { cout << "  No schedules.\n"; return; }
        string patient = getStringInput("  Patient name : ");
        bool any = false;
        for (auto& s : schedules) {
            auto alerts = s->checkMissed(patient);
            for (auto& a : alerts) {
                a->trigger();    // polymorphic call
                any = true;
            }
        }
        if (!any) cout << "  No missed doses detected.\n";
    }

    // 10. Trigger reminders (polymorphism)
    void triggerAllReminders() {
        printHeader("ACTIVE REMINDERS");
        int count = 0;
        for (auto& r : activeReminders) {
            if (!r->isTriggered()) {
                cout << "  Type: " << r->getType() << "\n";
                r->trigger();   // virtual dispatch
                ++count;
            }
        }
        if (count == 0) cout << "  No pending reminders.\n";
    }

    // 11. Search by name (linear)
    void searchByName() {
        printHeader("SEARCH BY NAME  [Linear Search O(n)]");
        string name = getStringInput("  Enter medicine name : ");
        Medicine* r = linearSearchByName(medicines, name);
        if (r) { cout << "  Found:\n"; r->display(); }
        else   { cout << "  Not found.\n"; }
    }

    // 12. Search by ID (binary)
    void searchByID() {
        printHeader("SEARCH BY ID  [Binary Search O(log n)]");
        int id = getIntInput("  Enter medicine ID : ", 0, 99999);
        sort(medicines.begin(), medicines.end(),
             [](const Medicine& a, const Medicine& b){
                 return a.getID() < b.getID(); });
        int idx = binarySearchByID(medicines, id);
        if (idx != -1) {
            cout << "  Found at sorted index " << idx << ":\n";
            medicines[idx].display();
        } else {
            cout << "  ID " << id << " not found.\n";
        }
    }

    // 13. Memory info
    void memoryInfo() const {
        printHeader("MEMORY MANAGEMENT INFO");
        cout << "  STACK (value objects in vector):\n";
        cout << "    medicines[]     : " << medicines.size()     << " Medicine objects\n";
        cout << "    prescriptions[] : " << prescriptions.size() << " Prescription objects\n\n";
        cout << "  HEAP (smart pointers):\n";
        cout << "    activeReminders : " << activeReminders.size()
             << " unique_ptr<Reminder>  (polymorphic)\n";
        cout << "    schedules[]     : " << schedules.size()
             << " unique_ptr<DosageSchedule>\n\n";
        cout << "  RAW HEAP demo (new / delete):\n";
        Medicine* raw = new Medicine(0, "DemoRaw", "1mg", "Once", 1);
        cout << "    new Medicine @ address " << raw << "\n";
        delete raw;
        raw = nullptr;
        cout << "    delete called -- pointer set to nullptr.\n";
        cout << "  Smart pointers auto-release on scope exit.\n";
    }
};

// ─────────────────────────────────────────────────────────────
//  MAIN MENU
// ─────────────────────────────────────────────────────────────
void showMenu() {
    cout << "\n";
    printLine('=', 52);
    cout << "   M E D T R A C K  --  Main Menu\n";
    printLine('=', 52);
    cout << "   MEDICINES\n";
    cout << "    1. Add Medicine\n";
    cout << "    2. List All Medicines\n";
    cout << "    3. Update Stock\n\n";
    cout << "   PRESCRIPTIONS\n";
    cout << "    4. Add Prescription\n";
    cout << "    5. View All Prescriptions\n\n";
    cout << "   SCHEDULE & DOSAGE\n";
    cout << "    6. Add Dosage Schedule\n";
    cout << "    7. View Dosage Schedules\n";
    cout << "    8. Mark Dose as Taken\n";
    cout << "    9. Check Missed Doses\n\n";
    cout << "   REMINDERS\n";
    cout << "   10. View / Trigger All Reminders\n\n";
    cout << "   SEARCH\n";
    cout << "   11. Search by Name  (Linear Search)\n";
    cout << "   12. Search by ID    (Binary Search)\n\n";
    cout << "   SYSTEM\n";
    cout << "   13. Memory Info\n";
    cout << "    0. Exit\n";
    printLine('=', 52);
    cout << "   Choice : ";
}

int main() {
    cout << "\n";
    cout << "  +-----------------------------------------+\n";
    cout << "  |   MedTrack -- Medication Reminder       |\n";
    cout << "  |      & Prescription Tracker             |\n";
    cout << "  |   Interactive C++ Application           |\n";
    cout << "  +-----------------------------------------+\n";
    cout << "  Time: " << currentTime() << "\n";

    MedTracker tracker;   // Stack object
    int choice;

    while (true) {
        showMenu();
        if (!(cin >> choice)) { clearInput(); continue; }
        clearInput();

        switch (choice) {
            case  1: tracker.addMedicine();         break;
            case  2: tracker.listMedicines();       break;
            case  3: tracker.updateStock();         break;
            case  4: tracker.addPrescription();     break;
            case  5: tracker.listPrescriptions();   break;
            case  6: tracker.addSchedule();         break;
            case  7: tracker.listSchedules();       break;
            case  8: tracker.markDoseTaken();       break;
            case  9: tracker.checkMissedDoses();    break;
            case 10: tracker.triggerAllReminders(); break;
            case 11: tracker.searchByName();        break;
            case 12: tracker.searchByID();          break;
            case 13: tracker.memoryInfo();          break;
            case  0:
                cout << "\n  Goodbye! Stay healthy.\n\n";
                return 0;
            default:
                cout << "  Invalid option. Try again.\n";
        }
        pause();
    }
}

