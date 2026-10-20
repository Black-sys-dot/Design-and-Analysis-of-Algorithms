#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure representing cargo to be loaded onto trucks
struct CargoItem {
    int weight;
    int utility;
    int priority;
    bool isLoaded = false;
};

int main() {
    int itemCount, numTrucks;

    cout << "Enter number of items: ";
    if (!(cin >> itemCount) || itemCount <= 0) return 0;

    vector<CargoItem> cargoList(itemCount);

    for (int i = 0; i < itemCount; i++) {
        cout << "\nItem " << i + 1 << ":\n";
        cout << "Enter weight, utility, priority (1/0): ";
        cin >> cargoList[i].weight >> cargoList[i].utility >> cargoList[i].priority;
    }

    cout << "\nEnter number of trucks: ";
    cin >> numTrucks;

    for (int t = 1; t <= numTrucks; t++) {
        int maxCapacity;
        cout << "\nEnter capacity of truck " << t << ": ";
        cin >> maxCapacity;

        // DP Table: dp[i][w] stores maximum utility using first i items within weight w
        vector<vector<int>> dp(itemCount + 1, vector<int>(maxCapacity + 1, 0));

        for (int i = 1; i <= itemCount; i++) {
            for (int w = 0; w <= maxCapacity; w++) {
                dp[i][w] = dp[i - 1][w];

                if (!cargoList[i - 1].isLoaded && cargoList[i - 1].weight <= w) {
                    int effectiveValue = cargoList[i - 1].utility;

                    // High priority bonus (+10 utility)
                    if (cargoList[i - 1].priority == 1) {
                        effectiveValue += 10;
                    }

                    dp[i][w] = max(
                        dp[i][w],
                        effectiveValue + dp[i - 1][w - cargoList[i - 1].weight]
                    );
                }
            }
        }

        cout << "\nTruck " << t;
        cout << "\nMaximum Utility: " << dp[itemCount][maxCapacity];
        cout << "\nSelected items:\n";

        // Backtrack to find items loaded onto this truck
        int remainingCap = maxCapacity;

        for (int i = itemCount; i >= 1; i--) {
            if (dp[i][remainingCap] != dp[i - 1][remainingCap]) {
                cout << "Item " << i
                     << " (Weight: " << cargoList[i - 1].weight
                     << " kg)\n";

                cargoList[i - 1].isLoaded = true;
                remainingCap -= cargoList[i - 1].weight;
            }
        }

        cout << "\n";
    }

    return 0;
}
