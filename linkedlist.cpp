#include "resident.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

// Node structure for singly linked list

struct Node {
    Resident data;
    Node* next;

    Node(const Resident& r) {
        data = r;
        next = nullptr;
    }
};

// Linked list class for storing records

class SinglyLinkedList {
private:
    Node* head;

// Constructor

public:
    SinglyLinkedList() {
        head = nullptr;
    }

    // Destructor to free allocated memory

    ~SinglyLinkedList() {
        clear();
    }

    // Return the head pointer

    Node* getHead() {
        return head;
    }

    // Checks if the list is empty

    bool isEmpty() const {
        return head == nullptr;
    }

    // Insert a new record at the end of list

    void insertAtEnd(const Resident& r) {
        Node* newNode = new Node(r);

        if (head == nullptr) {
            head = newNode;
            return;
        }

        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    // Count total nodes in list

    int countNodes() const {
        int count = 0;
        Node* temp = head;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }
        return count;
    }

    // Display all records in table format

    void displayList(const string& title) const {
        cout << "\n==============================================================\n";
        cout << title << "\n";
        cout << "==============================================================\n";

        if (head == nullptr) {
            cout << "List is empty.\n";
            return;
        }

        cout << left
             << setw(15) << "Resident ID"
             << setw(8)  << "Age"
             << setw(18) << "Mode"
             << setw(12) << "Distance"
             << setw(18) << "Carbon Emission"
             << setw(10) << "Avg Days"
             << "\n";

        cout << "--------------------------------------------------------------------------\n";

        Node* temp = head;
        while (temp != nullptr) {
            cout << left
                 << setw(15) << temp->data.residentID
                 << setw(8)  << temp->data.age
                 << setw(18) << temp->data.modeOfTransport
                 << setw(12) << temp->data.dailyDistance
                 << setw(18) << fixed << setprecision(2) << temp->data.carbonEmission
                 << setw(10) << temp->data.averageDayPerMonth
                 << "\n";

            temp = temp->next;
        }
    }

    // Delete all nodes and clear the list

    void clear() {
        Node* temp = head;
        while (temp != nullptr) {
            Node* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
        head = nullptr;
    }
};

bool isHeaderLine(const string& line) {
    return line.find("Resident") != string::npos ||
           line.find("resident") != string::npos ||
           line.find("Age") != string::npos ||
           line.find("Mode") != string::npos;
}

string getAgeGroup(int age) {
    if (age >= 6  && age <= 17) return "Children & Teenagers";
    if (age >= 18 && age <= 25) return "University Students / Young Adults";
    if (age >= 26 && age <= 45) return "Working Adults (Early Career)";
    if (age >= 46 && age <= 60) return "Working Adults (Late Career)";
    return "Senior Citizens / Retirees";
}

Resident parseResident(string line, char delimiter) {
    Resident r;
    stringstream ss(line);
    string token;

    getline(ss, r.residentID, delimiter);

    getline(ss, token, delimiter);
    r.age = stoi(token);
    r.ageGroup = getAgeGroup(r.age);

    getline(ss, r.modeOfTransport, delimiter);

    getline(ss, token, delimiter);
    r.dailyDistance = stoi(token);

    getline(ss, token, delimiter);
    r.carbonEmissionFactor = stod(token);

    getline(ss, token);
    r.averageDayPerMonth = stoi(token);

    r.carbonEmission = r.dailyDistance * r.carbonEmissionFactor * r.averageDayPerMonth;

    return r;
}

bool loadCSVIntoLinkedList(const string& filename, SinglyLinkedList& list) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open file -> " << filename << "\n";
        return false;
    }

    string line;
    bool firstLine = true;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        if (firstLine && isHeaderLine(line)) {
            firstLine = false;
            continue;
        }
        firstLine = false;

        Resident r;

        try {
            r = parseResident(line, ',');
        } catch (...) {
            cout << "Warning: Skipping malformed row in " << filename << "\n";
            continue;
        }

        list.insertAtEnd(r);
    }

    file.close();
    return true;
}

string toLowerCase(string text) {
    for (int i = 0; i < text.length(); i++) {
        text[i] = tolower(text[i]);
    }
    return text;
}

void showResident(const Resident& r) {
    cout << r.residentID << " | "
         << r.age << " | "
         << r.modeOfTransport << " | "
         << r.dailyDistance << " km | "
         << fixed << setprecision(2) << r.carbonEmission << " CO2 | "
         << r.averageDayPerMonth << " days" << endl;
}

void searchByAgeGroupList(Node* head, int minAge, int maxAge) {
    cout << "\nResidents aged " << minAge << " to " << maxAge << ":\n";
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data.age >= minAge && temp->data.age <= maxAge) {
            showResident(temp->data);
        }
        temp = temp->next;
    }
}

void searchByTransportList(Node* head, string mode) {
    cout << "\nTransport mode: " << mode << "\n";
    string inputMode = toLowerCase(mode);

    Node* temp = head;
    while (temp != nullptr) {
        if (toLowerCase(temp->data.modeOfTransport) == inputMode) {
            showResident(temp->data);
        }
        temp = temp->next;
    }
}

void searchByDistanceList(Node* head, int limit) {
    cout << "\nResidents with daily distance more than " << limit << " km:\n";
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data.dailyDistance > limit) {
            showResident(temp->data);
        }
        temp = temp->next;
    }
}

void showEmissionSummaryList(Node* head, const string& cityName) {
    int count = 0;
    double totalEmission = 0.0;

    Node* temp = head;
    while (temp != nullptr) {
        totalEmission += temp->data.carbonEmission;
        count++;
        temp = temp->next;
    }

    cout << "\n" << cityName << " Summary\n";
    cout << "Total Residents: " << count << "\n";
    cout << "Total Carbon Emission: " << fixed << setprecision(2) << totalEmission << "\n";

    if (count > 0) {
        cout << "Average Carbon Emission per Resident: " << fixed << setprecision(2)
             << (totalEmission / count) << "\n";
    }
}

Node* getCityHead(char city, SinglyLinkedList& cityAList, SinglyLinkedList& cityBList, SinglyLinkedList& cityCList) {
    if (city == 'A' || city == 'a') return cityAList.getHead();
    if (city == 'B' || city == 'b') return cityBList.getHead();
    if (city == 'C' || city == 'c') return cityCList.getHead();
    return nullptr;
}

void showMemoryFootprintLinkedList(SinglyLinkedList& cityAList, SinglyLinkedList& cityBList, SinglyLinkedList& cityCList) {
    int sizeResident = sizeof(Resident);
    int sizeNode = sizeof(Node);

    int cityAMemory = cityAList.countNodes() * sizeNode;
    int cityBMemory = cityBList.countNodes() * sizeNode;
    int cityCMemory = cityCList.countNodes() * sizeNode;
    int totalMemory = cityAMemory + cityBMemory + cityCMemory;

    cout << "\nMEMORY FOOTPRINT\n";
    cout << "------------------------------------------------------------\n";
    cout << "Resident struct size       : " << sizeResident << " bytes\n";
    cout << "ResidentNode size (+ ptr)  : " << sizeNode
         << " bytes (" << (sizeNode - sizeResident) << " bytes pointer overhead per node)\n";
    cout << "City A (" << cityAList.countNodes() << " nodes) : " << cityAMemory << " bytes (scattered on heap)\n";
    cout << "City B (" << cityBList.countNodes() << " nodes) : " << cityBMemory << " bytes (scattered on heap)\n";
    cout << "City C (" << cityCList.countNodes() << " nodes) : " << cityCMemory << " bytes (scattered on heap)\n";
    cout << "All Combined (" << (cityAList.countNodes() + cityBList.countNodes() + cityCList.countNodes())
         << " nodes) : " << totalMemory << " bytes (scattered on heap)\n";
    cout << "------------------------------------------------------------\n";
}

void showEmissionByTransportList(Node* head, const string& cityName) {
    double carTotal = 0, busTotal = 0, bicycleTotal = 0, walkingTotal = 0, schoolBusTotal = 0, carpoolTotal = 0;
    int carCount = 0, busCount = 0, bicycleCount = 0, walkingCount = 0, schoolBusCount = 0, carpoolCount = 0;

    Node* temp = head;
    while (temp != nullptr) {
        string mode = toLowerCase(temp->data.modeOfTransport);

        if (mode == "car") {
            carTotal += temp->data.carbonEmission;
            carCount++;
        } else if (mode == "bus") {
            busTotal += temp->data.carbonEmission;
            busCount++;
        } else if (mode == "bicycle") {
            bicycleTotal += temp->data.carbonEmission;
            bicycleCount++;
        } else if (mode == "walking") {
            walkingTotal += temp->data.carbonEmission;
            walkingCount++;
        } else if (mode == "school bus") {
            schoolBusTotal += temp->data.carbonEmission;
            schoolBusCount++;
        } else if (mode == "carpool") {
            carpoolTotal += temp->data.carbonEmission;
            carpoolCount++;
        }

        temp = temp->next;
    }

    cout << "\nCarbon Emission by Transport Mode - " << cityName << "\n";
    cout << "------------------------------------------------------------\n";
    cout << "Car        : " << carTotal << "\n";
    cout << "Bus        : " << busTotal << "\n";
    cout << "Bicycle    : " << bicycleTotal << "\n";
    cout << "Walking    : " << walkingTotal << "\n";
    cout << "School Bus : " << schoolBusTotal << "\n";
    cout << "Carpool    : " << carpoolTotal << "\n";
    cout << "------------------------------------------------------------\n";
}

int getTransportIndex(string mode) {
    mode = toLowerCase(mode);

    if (mode == "car") return 0;
    if (mode == "bus") return 1;
    if (mode == "bicycle") return 2;
    if (mode == "walking") return 3;
    if (mode == "school bus") return 4;
    if (mode == "carpool") return 5;

    return -1;
}

void showOneAgeGroupList(Node* head, int minAge, int maxAge, const string& groupLabel) {
    string transportNames[6] = {"Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"};
    int counts[6] = {0, 0, 0, 0, 0, 0};
    double totals[6] = {0, 0, 0, 0, 0, 0};

    int residentCount = 0;
    double totalEmission = 0.0;

    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data.age >= minAge && temp->data.age <= maxAge) {
            residentCount++;
            totalEmission += temp->data.carbonEmission;

            int index = getTransportIndex(temp->data.modeOfTransport);
            if (index != -1) {
                counts[index]++;
                totals[index] += temp->data.carbonEmission;
            }
        }
        temp = temp->next;
    }

    if (residentCount == 0) {
        return;
    }

    int preferredIndex = -1;
    int maxCount = 0;
    for (int i = 0; i < 6; i++) {
        if (counts[i] > maxCount) {
            maxCount = counts[i];
            preferredIndex = i;
        }
    }

    cout << "\nAge Group: " << minAge << "-" << maxAge << "  (" << groupLabel << ")\n";
    cout << "--------------------------------------------------------------------------\n";
    cout << left
         << setw(15) << "Transport"
         << setw(10) << "Count"
         << setw(24) << "Total Emission(kgCO2)"
         << setw(22) << "Avg Emission/Resident"
         << "\n";
    cout << "--------------------------------------------------------------------------\n";

    for (int i = 0; i < 6; i++) {
        if (counts[i] > 0) {
            double avg = totals[i] / counts[i];

            cout << left
                 << setw(15) << transportNames[i]
                 << setw(10) << counts[i]
                 << setw(24) << fixed << setprecision(2) << totals[i]
                 << setw(22) << fixed << setprecision(2) << avg;

            if (i == preferredIndex) {
                cout << " <-- Most Preferred";
            }
            cout << "\n";
        }
    }

    cout << "--------------------------------------------------------------------------\n";
    cout << "Residents: " << residentCount
         << " | Total Emission: " << fixed << setprecision(2) << totalEmission << " kg CO2"
         << " | Avg/Resident: " << fixed << setprecision(2) << (totalEmission / residentCount) << " kg CO2\n";

    if (preferredIndex != -1) {
        cout << "Most Preferred: " << transportNames[preferredIndex]
             << " (" << counts[preferredIndex] << " users)\n";
    }

    cout << "--------------------------------------------------------------------------\n";
}

void showAgeGroupAnalysisList(Node* head, const string& cityName) {
    cout << "\nAGE GROUP ANALYSIS -- " << cityName << "\n";
    cout << "==========================================================================\n";

    showOneAgeGroupList(head, 6, 17, "Children & Teenagers");
    showOneAgeGroupList(head, 18, 25, "University Students / Young Adults");
    showOneAgeGroupList(head, 26, 45, "Working Adults, Early Career");
    showOneAgeGroupList(head, 46, 60, "Working Adults, Late Career");
    showOneAgeGroupList(head, 61, 120, "Senior Citizens / Retirees");
}

void sortListByAge(Node* head) {
    if (head == nullptr) return;

    bool swapped;
    do {
        swapped = false;
        Node* temp = head;

        while (temp->next != nullptr) {
            if (temp->data.age > temp->next->data.age) {
                swap(temp->data, temp->next->data);
                swapped = true;
            }
            temp = temp->next;
        }
    } while (swapped);
}

void sortListByDistance(Node* head) {
    if (head == nullptr) return;

    bool swapped;
    do {
        swapped = false;
        Node* temp = head;

        while (temp->next != nullptr) {
            if (temp->data.dailyDistance > temp->next->data.dailyDistance) {
                swap(temp->data, temp->next->data);
                swapped = true;
            }
            temp = temp->next;
        }
    } while (swapped);
}

void sortListByCarbon(Node* head) {
    if (head == nullptr) return;

    bool swapped;
    do {
        swapped = false;
        Node* temp = head;

        while (temp->next != nullptr) {
            if (temp->data.carbonEmission > temp->next->data.carbonEmission) {
                swap(temp->data, temp->next->data);
                swapped = true;
            }
            temp = temp->next;
        }
    } while (swapped);
}

int main() {
    SinglyLinkedList cityAList;
    SinglyLinkedList cityBList;
    SinglyLinkedList cityCList;

    bool loadedA = loadCSVIntoLinkedList(CITY_A_FILE, cityAList);
    bool loadedB = loadCSVIntoLinkedList(CITY_B_FILE, cityBList);
    bool loadedC = loadCSVIntoLinkedList(CITY_C_FILE, cityCList);

    if (!loadedA && !loadedB && !loadedC) {
        cout << "No files loaded. Please check your CSV files.\n";
        return 1;
    }

    int choice;
    do {
        cout << "\n================ LINKED LIST MENU ================\n";
        cout << "1. Display City A\n";
        cout << "2. Display City B\n";
        cout << "3. Display City C\n";
        cout << "4. Show node counts\n";
        cout << "5. Sort a City by Age\n";
        cout << "6. Sort a City by Distance\n";
        cout << "7. Sort a City by Carbon Emission\n";
        cout << "8. Show Age Group Analysis\n";
        cout << "9. Search a City by Age Group\n";
        cout << "10. Search a City by Transport Mode\n";
        cout << "11. Search a City by Distance Threshold\n";
        cout << "12. Show Carbon Emission Summary\n";
        cout << "13. Show Carbon Emission by Transport Mode\n";
        cout << "14. Show Memory Footprint\n";
        cout << "15. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

    switch (choice) {
    case 1:
        cityAList.displayList("City A - Singly Linked List");
        break;

    case 2:
        cityBList.displayList("City B - Singly Linked List");
        break;

    case 3:
        cityCList.displayList("City C - Singly Linked List");
        break;

    case 4:
        cout << "\nNode Counts:\n";
        cout << "City A: " << cityAList.countNodes() << "\n";
        cout << "City B: " << cityBList.countNodes() << "\n";
        cout << "City C: " << cityCList.countNodes() << "\n";
        break;

    case 5: {
        char city;
        Node* head;

        cout << "Enter city (A/B/C): ";
        cin >> city;

        head = getCityHead(city, cityAList, cityBList, cityCList);

        if (head == nullptr) {
            cout << "Invalid city.\n";
            break;
        }

        sortListByAge(head);
        cout << "\nCity " << city << " sorted by age:\n";

        if (city == 'A' || city == 'a')
            cityAList.displayList("City A - Sorted by Age");
        else if (city == 'B' || city == 'b')
            cityBList.displayList("City B - Sorted by Age");
        else
            cityCList.displayList("City C - Sorted by Age");

        break;
    }

    case 6: {
        char city;
        Node* head;

        cout << "Enter city (A/B/C): ";
        cin >> city;

        head = getCityHead(city, cityAList, cityBList, cityCList);

        if (head == nullptr) {
            cout << "Invalid city.\n";
            break;
        }

        sortListByDistance(head);
        cout << "\nCity " << city << " sorted by distance:\n";

        if (city == 'A' || city == 'a')
            cityAList.displayList("City A - Sorted by Distance");
        else if (city == 'B' || city == 'b')
            cityBList.displayList("City B - Sorted by Distance");
        else
            cityCList.displayList("City C - Sorted by Distance");

        break;
    }

    case 7: {
        char city;
        Node* head;

        cout << "Enter city (A/B/C): ";
        cin >> city;

        head = getCityHead(city, cityAList, cityBList, cityCList);

        if (head == nullptr) {
            cout << "Invalid city.\n";
            break;
        }

        sortListByCarbon(head);
        cout << "\nCity " << city << " sorted by carbon emission:\n";

        if (city == 'A' || city == 'a')
            cityAList.displayList("City A - Sorted by Carbon Emission");
        else if (city == 'B' || city == 'b')
            cityBList.displayList("City B - Sorted by Carbon Emission");
        else
            cityCList.displayList("City C - Sorted by Carbon Emission");

        break;
    }

    case 8: {
        char city;
        Node* head;

        cout << "Enter city (A/B/C): ";
        cin >> city;

        head = getCityHead(city, cityAList, cityBList, cityCList);

        if (head == nullptr) {
            cout << "Invalid city.\n";
            break;
        }

        if (city == 'A' || city == 'a')
            showAgeGroupAnalysisList(head, "City A - Metropolitan City");
        else if (city == 'B' || city == 'b')
            showAgeGroupAnalysisList(head, "City B - University Town");
        else
            showAgeGroupAnalysisList(head, "City C - Suburban / Rural Area");

        break;
    }

    case 9: {
        char city;
        int minAge, maxAge;
        Node* head;

        cout << "Enter city (A/B/C): ";
        cin >> city;

        head = getCityHead(city, cityAList, cityBList, cityCList);

        if (head == nullptr) {
            cout << "Invalid city.\n";
            break;
        }

        cout << "Enter minimum age: ";
        cin >> minAge;
        cout << "Enter maximum age: ";
        cin >> maxAge;

        searchByAgeGroupList(head, minAge, maxAge);
        break;
    }

    case 10: {
        char city;
        string mode;
        Node* head;

        cout << "Enter city (A/B/C): ";
        cin >> city;

        head = getCityHead(city, cityAList, cityBList, cityCList);

        if (head == nullptr) {
            cout << "Invalid city.\n";
            break;
        }

        cin.ignore(10000, '\n');
        cout << "Enter transport mode: ";
        getline(cin, mode);

        searchByTransportList(head, mode);
        break;
    }

    case 11: {
        char city;
        int limit;
        Node* head;

        cout << "Enter city (A/B/C): ";
        cin >> city;

        head = getCityHead(city, cityAList, cityBList, cityCList);

        if (head == nullptr) {
            cout << "Invalid city.\n";
            break;
        }

        cout << "Enter distance threshold: ";
        cin >> limit;

        searchByDistanceList(head, limit);
        break;
    }

    case 12:
        showEmissionSummaryList(cityAList.getHead(), "City A");
        showEmissionSummaryList(cityBList.getHead(), "City B");
        showEmissionSummaryList(cityCList.getHead(), "City C");
        break;

    case 13:
        showEmissionByTransportList(cityAList.getHead(), "City A");
        showEmissionByTransportList(cityBList.getHead(), "City B");
        showEmissionByTransportList(cityCList.getHead(), "City C");
        break;

    case 14:
        showMemoryFootprintLinkedList(cityAList, cityBList, cityCList);
        break;

    case 15:
        cout << "Exiting program...\n";
        break;

    default:
        cout << "Invalid choice. Try again.\n";
}

    } while (choice != 15);

    return 0;
}