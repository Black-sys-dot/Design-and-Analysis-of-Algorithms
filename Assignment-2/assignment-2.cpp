#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

// Structure to store car specifications and price
struct VehicleRecord {
    string name;
    string fuel;
    string bodyStyle;
    int horsepower;
    int cityMpg;
    int highwayMpg;
    float price;
};

// Partition function for descending Quick Sort based on price
int partition(vector<VehicleRecord>& vehicleList, int lowIdx, int highIdx) {
    float pivotPrice = vehicleList[highIdx].price;
    int boundaryIdx = lowIdx - 1;

    for (int curr = lowIdx; curr < highIdx; curr++) {
        // Sort descending: larger price comes first
        if (vehicleList[curr].price > pivotPrice) {
            boundaryIdx++;
            swap(vehicleList[boundaryIdx], vehicleList[curr]);
        }
    }

    swap(vehicleList[boundaryIdx + 1], vehicleList[highIdx]);
    return boundaryIdx + 1;
}

// QuickSort algorithm
void quickSort(vector<VehicleRecord>& vehicleList, int lowIdx, int highIdx) {
    if (lowIdx < highIdx) {
        int pivotIdx = partition(vehicleList, lowIdx, highIdx);
        quickSort(vehicleList, lowIdx, pivotIdx - 1);
        quickSort(vehicleList, pivotIdx + 1, highIdx);
    }
}

int main() {
    vector<VehicleRecord> vehicleList;
    ifstream dataFile("CarPrice_Assignment.csv");

    if (!dataFile.is_open()) {
        cout << "File not found!" << endl;
        return 0;
    }

    string fileRow;
    // Skip header row
    getline(dataFile, fileRow);

    while (getline(dataFile, fileRow)) {
        if (fileRow.empty()) continue;
        stringstream ss(fileRow);
        string fields[26];

        for (int i = 0; i < 26; i++) {
            getline(ss, fields[i], ',');
        }

        try {
            VehicleRecord vehicle;
            vehicle.name = fields[2];
            vehicle.fuel = fields[3];
            vehicle.bodyStyle = fields[6];
            vehicle.horsepower = stoi(fields[21]);
            vehicle.cityMpg = stoi(fields[23]);
            vehicle.highwayMpg = stoi(fields[24]);
            vehicle.price = stof(fields[25]);

            vehicleList.push_back(vehicle);
        } catch (...) {
            continue;
        }
    }

    dataFile.close();

    if (!vehicleList.empty()) {
        quickSort(vehicleList, 0, vehicleList.size() - 1);
    }

    cout << "\nCars Sorted By Price (Highest to Lowest)\n\n";

    cout << left
         << setw(5) << "No."
         << setw(28) << "Car Name"
         << setw(10) << "Fuel"
         << setw(15) << "Body"
         << setw(12) << "HP"
         << setw(10) << "City"
         << setw(10) << "Highway"
         << setw(12) << "Price"
         << endl;

    cout << "---------------------------------------------------------------------------------------------\n";

    int displayLimit = 20;
    if ((int)vehicleList.size() < displayLimit) {
        displayLimit = vehicleList.size();
    }

    for (int i = 0; i < displayLimit; i++) {
        cout << left
             << setw(5) << i + 1
             << setw(28) << vehicleList[i].name.substr(0, 27)
             << setw(10) << vehicleList[i].fuel
             << setw(15) << vehicleList[i].bodyStyle
             << setw(12) << vehicleList[i].horsepower
             << setw(10) << vehicleList[i].cityMpg
             << setw(10) << vehicleList[i].highwayMpg
             << setw(12) << fixed << setprecision(1) << vehicleList[i].price
             << endl;
    }

    cout << "\nTotal Cars : " << vehicleList.size() << endl;

    return 0;
}
