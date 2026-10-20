#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

// Structure representing an emergency relief supply item
struct ReliefItem {
    string name;
    double weight;
    double utility;
    bool isDivisible;
    int priority;

    ReliefItem(string n, double w, double u, bool d, int p)
        : name(n), weight(w), utility(u), isDivisible(d), priority(p) {}

    double utilityDensity() const {
        return utility / weight;
    }
};

// Comparator: primary by priority (ascending, 1=High), secondary by utility/weight (descending)
bool compareReliefItems(const ReliefItem& a, const ReliefItem& b) {
    if (a.priority == b.priority) {
        return a.utilityDensity() > b.utilityDensity();
    }
    return a.priority < b.priority;
}

// Solves fractional knapsack respecting priority and divisibility constraints
double computeKnapsack(vector<ReliefItem>& items, double maxCapacity, double& carriedWeight) {
    sort(items.begin(), items.end(), compareReliefItems);

    cout << "\nSorted Items (by Priority, then Value/Weight):\n";
    cout << left << setw(20) << "Item"
         << setw(10) << "Weight"
         << setw(10) << "Value"
         << setw(12) << "Priority"
         << setw(15) << "Value/Weight"
         << setw(15) << "Type" << "\n";

    for (const auto& item : items) {
        cout << left << setw(20) << item.name
             << setw(10) << fixed << setprecision(2) << item.weight
             << setw(10) << fixed << setprecision(2) << item.utility
             << setw(12) << item.priority
             << setw(15) << fixed << setprecision(2) << item.utilityDensity()
             << setw(15) << (item.isDivisible ? "Divisible" : "Indivisible") << "\n";
    }

    double totalUtility = 0.0;
    carriedWeight = 0.0;
    double remainingCapacity = maxCapacity;

    cout << "\nItems selected for transport:\n";

    for (const auto& item : items) {
        if (remainingCapacity <= 0) break;

        if (item.isDivisible) {
            double loadWeight = min(item.weight, remainingCapacity);
            double loadUtility = item.utilityDensity() * loadWeight;
            totalUtility += loadUtility;
            remainingCapacity -= loadWeight;
            carriedWeight += loadWeight;

            cout << " - " << item.name << ": " << fixed << setprecision(2) << loadWeight
                 << " kg, Utility = " << loadUtility
                 << ", Priority = " << item.priority << ", Type = Divisible\n";
        } else {
            if (item.weight <= remainingCapacity) {
                totalUtility += item.utility;
                remainingCapacity -= item.weight;
                carriedWeight += item.weight;

                cout << " - " << item.name << ": " << fixed << setprecision(2) << item.weight
                     << " kg, Utility = " << item.utility
                     << ", Priority = " << item.priority << ", Type = Indivisible\n";
            }
        }
    }

    return totalUtility;
}

int main() {
    int itemCount;
    cout << "Enter number of relief items: ";
    if (!(cin >> itemCount) || itemCount <= 0) {
        return 0;
    }

    vector<ReliefItem> reliefList;
    for (int i = 0; i < itemCount; ++i) {
        string itemName;
        double itemWeight, itemUtility;
        int divisibleFlag, itemPriority;

        cout << "\nItem #" << (i + 1) << ":\n";
        cout << "Name: ";
        cin.ignore();
        getline(cin, itemName);
        cout << "Weight (kg): ";
        cin >> itemWeight;
        cout << "Utility Value: ";
        cin >> itemUtility;

        cout << "Is it divisible? (1 = Yes, 0 = No): ";
        cin >> divisibleFlag;
        cout << "Priority (1 = High, 2 = Medium, 3 = Low): ";
        cin >> itemPriority;

        reliefList.emplace_back(itemName, itemWeight, itemUtility, divisibleFlag == 1, itemPriority);
    }

    double boatCapacity;
    cout << "\nEnter maximum weight capacity of the boat (in kg): ";
    cin >> boatCapacity;

    double finalCarriedWeight = 0.0;
    double maxUtility = computeKnapsack(reliefList, boatCapacity, finalCarriedWeight);

    cout << "\n===== Final Report =====\n";
    cout << "Total weight carried: " << fixed << setprecision(2) << finalCarriedWeight << " kg\n";
    cout << "Total utility value carried: " << fixed << setprecision(2) << maxUtility << " units\n";

    return 0;
}
