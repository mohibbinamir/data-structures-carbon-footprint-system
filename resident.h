#ifndef RESIDENT_H
#define RESIDENT_H

#include <string>
using namespace std;

// Resident record structure
struct Resident {
    string residentID;
    int age;
    string modeOfTransport;
    int dailyDistance;
    double carbonEmissionFactor;
    int averageDayPerMonth;
    double carbonEmission;
    string ageGroup;
};

// Shared dataset file names
const string CITY_A_FILE = "dataset1-cityA.csv";
const string CITY_B_FILE = "dataset2-cityB.csv";
const string CITY_C_FILE = "dataset3-cityC.csv";

#endif