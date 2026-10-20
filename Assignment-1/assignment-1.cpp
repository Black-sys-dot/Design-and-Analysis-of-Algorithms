#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// Structure to store individual customer order details
struct OrderRecord {
    string orderId;
    string orderDate;
};

// Merges two sorted subarrays into the main vector
void merge(vector<OrderRecord>& orderList, int low, int mid, int high) {
    int nLeft = mid - low + 1;
    int nRight = high - mid;

    vector<OrderRecord> leftArr(nLeft);
    vector<OrderRecord> rightArr(nRight);

    for (int i = 0; i < nLeft; i++) {
        leftArr[i] = orderList[low + i];
    }
    for (int j = 0; j < nRight; j++) {
        rightArr[j] = orderList[mid + 1 + j];
    }

    int idxLeft = 0;
    int idxRight = 0;
    int idxMerged = low;

    while (idxLeft < nLeft && idxRight < nRight) {
        if (leftArr[idxLeft].orderDate <= rightArr[idxRight].orderDate) {
            orderList[idxMerged] = leftArr[idxLeft];
            idxLeft++;
        } else {
            orderList[idxMerged] = rightArr[idxRight];
            idxRight++;
        }
        idxMerged++;
    }

    while (idxLeft < nLeft) {
        orderList[idxMerged] = leftArr[idxLeft];
        idxLeft++;
        idxMerged++;
    }

    while (idxRight < nRight) {
        orderList[idxMerged] = rightArr[idxRight];
        idxRight++;
        idxMerged++;
    }
}

// Recursive Merge Sort implementation
void mergeSort(vector<OrderRecord>& orderList, int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;

        mergeSort(orderList, low, mid);
        mergeSort(orderList, mid + 1, high);
        merge(orderList, low, mid, high);
    }
}

int main() {
    ifstream csvFile("ecommerce_dataset_+1m.csv");
    if (!csvFile.is_open()) {
        cout << "Error opening file!" << endl;
        return 1;
    }

    cout << "File opened successfully!" << endl;

    string row;
    // Skip the header row
    getline(csvFile, row);

    vector<OrderRecord> orderRecords;
    while (getline(csvFile, row)) {
        if (row.empty()) continue;
        stringstream ss(row);
        OrderRecord currentOrder;
        getline(ss, currentOrder.orderId, ',');
        getline(ss, currentOrder.orderDate, ',');
        orderRecords.push_back(currentOrder);
    }

    cout << "Total Orders Read : " << orderRecords.size() << endl;

    if (!orderRecords.empty()) {
        mergeSort(orderRecords, 0, orderRecords.size() - 1);
    }

    cout << "\nFirst 20 Sorted Customer Orders:\n\n";
    cout << "Order ID\tOrder Date\n";
    cout << "-----------------------------------------------\n";
    for (size_t i = 0; i < 20 && i < orderRecords.size(); i++) {
        cout << orderRecords[i].orderId << "\t" << orderRecords[i].orderDate << endl;
    }

    csvFile.close();
    return 0;
}
