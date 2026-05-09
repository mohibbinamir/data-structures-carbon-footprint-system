#include "resident.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cctype>

using namespace std;

// Array class for storing records

class ResidentArray {
private:
    Resident* data;
    int size;
    int capacity;

    void resizeArray() {
        int newCapacity = capacity * 2;
        Resident* newData = new Resident[newCapacity];

        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    ResidentArray(int initialCapacity = 100) {
        capacity = initialCapacity;
        size = 0;
        data = new Resident[capacity];
    }

    ~ResidentArray() {
        delete[] data;
    }

    // Adds a new resident record

    void add(const Resident& r) {
        if (size == capacity) {
            resizeArray();
        }
        data[size] = r;
        size++;
    }

    // Returns the total number of stored records

    int getSize() const {
        return size;
    }

    // Checks if the array is empty

    bool isEmpty() const {
        return size == 0;
    }

    // Returns pointer to array data

    Resident* getData() {
        return data;
    }

    const Resident* getData() const {
        return data;
    }

    // Displays all records in table format

    void display(const string& title) const {
        cout << "\n==============================================================\n";
        cout << title << "\n";
        cout << "==============================================================\n";

        if (size == 0) {
            cout << "Array is empty.\n";
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

        for (int i = 0; i < size; i++) {
            cout << left
                 << setw(15) << data[i].residentID
                 << setw(8)  << data[i].age
                 << setw(18) << data[i].modeOfTransport
                 << setw(12) << data[i].dailyDistance
                 << setw(18) << fixed << setprecision(2) << data[i].carbonEmission
                 << setw(10) << data[i].averageDayPerMonth
                 << "\n";
        }
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
    r.carbonEmission = stod(token);

    getline(ss, token);
    r.averageDayPerMonth = stoi(token);

    r.carbonEmission = r.dailyDistance * r.carbonEmission * r.averageDayPerMonth;

    return r;
}

bool loadCSVIntoArray(const string& filename, ResidentArray& arr) {
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

        arr.add(r);

    }

    file.close();
    return true;
}

void showResident(Resident r) {
    cout << r.residentID << " | "
        << r.age << " | "
        << r.modeOfTransport << " | "
        << r.dailyDistance << " km | "
        << r.carbonEmission << " CO2 | "
        << r.averageDayPerMonth << " days" << endl;
}

void showAll(Resident arr[], int n) {
    for (int i = 0; i < n; i++) {
        showResident(arr[i]);
    }
}

void sortByAge(Resident arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (arr[j].age > arr[j + 1].age) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void sortByDistance(Resident arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (arr[j].dailyDistance > arr[j + 1].dailyDistance) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void sortByCarbon(Resident arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (arr[j].carbonEmission > arr[j + 1].carbonEmission) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void searchByAgeGroup(Resident arr[], int n, int minAge, int maxAge) {
    cout << "\nAge group " << minAge << " to " << maxAge << ":\n";

    for (int i = 0; i < n; i++) {
        if (arr[i].age >= minAge && arr[i].age <= maxAge) {
            showResident(arr[i]);
        }
    }
}

string toLowerCase(string text) {
    for (int i = 0; i < text.length(); i++) {
        text[i] = tolower(text[i]);
    }
    return text;
}

void searchByTransport(Resident arr[], int n, string mode) {
    cout << "\nTransport mode: " << mode << endl;

    string inputMode = toLowerCase(mode);

    for (int i = 0; i < n; i++) {
        if (toLowerCase(arr[i].modeOfTransport) == inputMode) {
            showResident(arr[i]);
        }
    }
}

void searchByDistance(Resident arr[], int n, int limit) {
    cout << "\nDistance more than " << limit << " km:\n";

    for (int i = 0; i < n; i++) {
        if (arr[i].dailyDistance > limit) {
            showResident(arr[i]);
        }
    }
}

Resident* getCityData(char city, ResidentArray& cityA, ResidentArray& cityB, ResidentArray& cityC, int& n) {
    if (city == 'A' || city == 'a') {
        n = cityA.getSize();
        return cityA.getData();
    }
    else if (city == 'B' || city == 'b') {
        n = cityB.getSize();
        return cityB.getData();
    }
    else if (city == 'C' || city == 'c') {
        n = cityC.getSize();
        return cityC.getData();
    }

    n = 0;
    return nullptr;
}

void showMemoryFootprintArray(ResidentArray& cityA, ResidentArray& cityB, ResidentArray& cityC) {
    int sizeResident = sizeof(Resident);

    int cityAMemory = cityA.getSize() * sizeResident;
    int cityBMemory = cityB.getSize() * sizeResident;
    int cityCMemory = cityC.getSize() * sizeResident;
    int totalMemory = cityAMemory + cityBMemory + cityCMemory;

    cout << "\nMEMORY FOOTPRINT\n";
    cout << "------------------------------------------------------------\n";
    cout << "Resident struct size       : " << sizeResident << " bytes\n";
    cout << "City A (" << cityA.getSize() << " records) : " << cityAMemory << " bytes (contiguous block)\n";
    cout << "City B (" << cityB.getSize() << " records) : " << cityBMemory << " bytes (contiguous block)\n";
    cout << "City C (" << cityC.getSize() << " records) : " << cityCMemory << " bytes (contiguous block)\n";
    cout << "All Combined (" << (cityA.getSize() + cityB.getSize() + cityC.getSize())
         << " records) : " << totalMemory << " bytes (contiguous block)\n";
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

void showEmissionSummaryArray(Resident arr[], int n, const string& cityName) {
    int count = 0;
    double totalEmission = 0.0;

    for (int i = 0; i < n; i++) {
        totalEmission += arr[i].carbonEmission;
        count++;
    }

    cout << "\n" << cityName << " Summary\n";
    cout << "Total Residents: " << count << "\n";
    cout << "Total Carbon Emission: " << fixed << setprecision(2) << totalEmission << "\n";

    if (count > 0) {
        cout << "Average Carbon Emission per Resident: "
             << fixed << setprecision(2)
             << (totalEmission / count) << "\n";
    }
}

void showEmissionByTransportArray(Resident arr[], int n, const string& cityName) {
    double carTotal = 0, busTotal = 0, bicycleTotal = 0, walkingTotal = 0, schoolBusTotal = 0, carpoolTotal = 0;
    int carCount = 0, busCount = 0, bicycleCount = 0, walkingCount = 0, schoolBusCount = 0, carpoolCount = 0;

    for (int i = 0; i < n; i++) {
        string mode = toLowerCase(arr[i].modeOfTransport);

        if (mode == "car") {
            carTotal += arr[i].carbonEmission;
            carCount++;
        } else if (mode == "bus") {
            busTotal += arr[i].carbonEmission;
            busCount++;
        } else if (mode == "bicycle") {
            bicycleTotal += arr[i].carbonEmission;
            bicycleCount++;
        } else if (mode == "walking") {
            walkingTotal += arr[i].carbonEmission;
            walkingCount++;
        } else if (mode == "school bus") {
            schoolBusTotal += arr[i].carbonEmission;
            schoolBusCount++;
        } else if (mode == "carpool") {
            carpoolTotal += arr[i].carbonEmission;
            carpoolCount++;
        }
    }

    cout << "\nCarbon Emission by Transport Mode - " << cityName << "\n";
    cout << "------------------------------------------------------------\n";
    cout << "Car        : " << fixed << setprecision(2) << carTotal << "\n";
    cout << "Bus        : " << fixed << setprecision(2) << busTotal << "\n";
    cout << "Bicycle    : " << fixed << setprecision(2) << bicycleTotal << "\n";
    cout << "Walking    : " << fixed << setprecision(2) << walkingTotal << "\n";
    cout << "School Bus : " << fixed << setprecision(2) << schoolBusTotal << "\n";
    cout << "Carpool    : " << fixed << setprecision(2) << carpoolTotal << "\n";
    cout << "------------------------------------------------------------\n";
}

void showOneAgeGroupArray(Resident arr[], int n, int minAge, int maxAge, const string& groupLabel) {
    string transportNames[6] = {"Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"};
    int counts[6] = {0, 0, 0, 0, 0, 0};
    double totals[6] = {0, 0, 0, 0, 0, 0};

    int residentCount = 0;
    double totalEmission = 0.0;

    for (int i = 0; i < n; i++) {
        if (arr[i].age >= minAge && arr[i].age <= maxAge) {
            residentCount++;
            totalEmission += arr[i].carbonEmission;

            int index = getTransportIndex(arr[i].modeOfTransport);
            if (index != -1) {
                counts[index]++;
                totals[index] += arr[i].carbonEmission;
            }
        }
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

void showAgeGroupAnalysisArray(Resident arr[], int n, const string& cityName) {
    cout << "\nAGE GROUP ANALYSIS -- " << cityName << "\n";
    cout << "==========================================================================\n";

    showOneAgeGroupArray(arr, n, 6, 17, "Children & Teenagers");
    showOneAgeGroupArray(arr, n, 18, 25, "University Students / Young Adults");
    showOneAgeGroupArray(arr, n, 26, 45, "Working Adults, Early Career");
    showOneAgeGroupArray(arr, n, 46, 60, "Working Adults, Late Career");
    showOneAgeGroupArray(arr, n, 61, 120, "Senior Citizens / Retirees");
}

int main() {
    ResidentArray cityA(250);
    ResidentArray cityB(250);
    ResidentArray cityC(250);

    bool loadedA = loadCSVIntoArray(CITY_A_FILE, cityA);
    bool loadedB = loadCSVIntoArray(CITY_B_FILE, cityB);
    bool loadedC = loadCSVIntoArray(CITY_C_FILE, cityC);

    if (!loadedA && !loadedB && !loadedC) {
        cout << "No files loaded. Please check your CSV files.\n";
        return 1;
    }

    int choice = 0;
    do {

        cout << "\n================ ARRAY MENU ================\n";
        cout << "1. Display City A\n";
        cout << "2. Display City B\n";
        cout << "3. Display City C\n";
        cout << "4. Show record counts\n";
        cout << "5. Sort a City by Age\n";
        cout << "6. Sort a City by Distance\n";
        cout << "7. Sort a City by Carbon Emission\n";
        cout << "8. Search a City by Age Group\n";
        cout << "9. Search a City by Transport Mode\n";
        cout << "10. Search a City by Distance Threshold\n";
        cout << "11. Show Age Group Analysis\n";
        cout << "12. Show Carbon Emission Summary\n";
        cout << "13. Show Carbon Emission by Transport Mode\n";
        cout << "14. Show Memory Footprint\n";
        cout << "15. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice. Try again.\n";
            continue;
}

switch (choice) {
    case 1:
        cityA.display("City A - Array Implementation");
        break;

    case 2:
        cityB.display("City B - Array Implementation");
        break;

    case 3:
        cityC.display("City C - Array Implementation");
        break;

    case 4:
        cout << "\nRecord Counts:\n";
        cout << "City A: " << cityA.getSize() << "\n";
        cout << "City B: " << cityB.getSize() << "\n";
        cout << "City C: " << cityC.getSize() << "\n";
        break;

    case 5: {
        char city;
        int n;
        Resident* data;

        cout << "Enter city (A/B/C): ";
        cin >> city;

        data = getCityData(city, cityA, cityB, cityC, n);

        if (data == nullptr) {
            cout << "Invalid city.\n";
            break;
        }

        sortByAge(data, n);
        cout << "\nCity " << city << " sorted by age:\n";
        showAll(data, n);
        break;
    }

    case 6: {
        char city;
        int n;
        Resident* data;

        cout << "Enter city (A/B/C): ";
        cin >> city;

        data = getCityData(city, cityA, cityB, cityC, n);

        if (data == nullptr) {
            cout << "Invalid city.\n";
            break;
        }

        sortByDistance(data, n);
        cout << "\nCity " << city << " sorted by distance:\n";
        showAll(data, n);
        break;
    }

    case 7: {
        char city;
        int n;
        Resident* data;

        cout << "Enter city (A/B/C): ";
        cin >> city;

        data = getCityData(city, cityA, cityB, cityC, n);

        if (data == nullptr) {
            cout << "Invalid city.\n";
            break;
        }

        sortByCarbon(data, n);
        cout << "\nCity " << city << " sorted by carbon emission:\n";
        showAll(data, n);
        break;
    }

    case 8: {
        char city;
        int minAge, maxAge, n;
        Resident* data;

        cout << "Enter city (A/B/C): ";
        cin >> city;

        data = getCityData(city, cityA, cityB, cityC, n);

        if (data == nullptr) {
            cout << "Invalid city.\n";
            break;
        }

        cout << "Enter minimum age: ";
        cin >> minAge;
        cout << "Enter maximum age: ";
        cin >> maxAge;

        searchByAgeGroup(data, n, minAge, maxAge);
        break;
    }

    case 9: {
        char city;
        string mode;
        int n;
        Resident* data;

        cout << "Enter city (A/B/C): ";
        cin >> city;

        data = getCityData(city, cityA, cityB, cityC, n);

        if (data == nullptr) {
            cout << "Invalid city.\n";
            break;
        }

        cin.ignore(10000, '\n');
        cout << "Enter transport mode: ";
        getline(cin, mode);

        searchByTransport(data, n, mode);
        break;
    }

    case 10: {
        char city;
        int limit, n;
        Resident* data;

        cout << "Enter city (A/B/C): ";
        cin >> city;

        data = getCityData(city, cityA, cityB, cityC, n);

        if (data == nullptr) {
            cout << "Invalid city.\n";
            break;
        }

        cout << "Enter distance threshold: ";
        cin >> limit;

        searchByDistance(data, n, limit);
        break;
    }

    case 11: {
        char city;
        int n;
        Resident* data;

        cout << "Enter city (A/B/C): ";
        cin >> city;

        data = getCityData(city, cityA, cityB, cityC, n);

        if (data == nullptr) {
            cout << "Invalid city.\n";
            break;
        }

        if (city == 'A' || city == 'a')
            showAgeGroupAnalysisArray(data, n, "City A - Metropolitan City");
        else if (city == 'B' || city == 'b')
            showAgeGroupAnalysisArray(data, n, "City B - University Town");
        else
            showAgeGroupAnalysisArray(data, n, "City C - Suburban / Rural Area");

        break;
    }

    case 12:
        showEmissionSummaryArray(cityA.getData(), cityA.getSize(), "City A");
        showEmissionSummaryArray(cityB.getData(), cityB.getSize(), "City B");
        showEmissionSummaryArray(cityC.getData(), cityC.getSize(), "City C");
        break;

    case 13:
        showEmissionByTransportArray(cityA.getData(), cityA.getSize(), "City A");
        showEmissionByTransportArray(cityB.getData(), cityB.getSize(), "City B");
        showEmissionByTransportArray(cityC.getData(), cityC.getSize(), "City C");
        break;

    case 14:
        showMemoryFootprintArray(cityA, cityB, cityC);
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