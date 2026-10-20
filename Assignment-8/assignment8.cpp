#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF_COST = 1000000000;

// Search space tree node
struct StateNode {
    vector<vector<int>> costMatrix;
    vector<int> route;
    int lowerBound;
};

// Priority queue comparator (min-bound priority)
struct StateComparator {
    bool operator()(const StateNode& a, const StateNode& b) const {
        return a.lowerBound > b.lowerBound;
    }
};

// Reduces the matrix by subtracting row and column minimums; returns total reduction
int reduceCostMatrix(vector<vector<int>>& mat) {
    int size = mat.size();
    int totalReduction = 0;

    // Row reduction
    for (int i = 0; i < size; i++) {
        int rowMin = *min_element(mat[i].begin(), mat[i].end());
        if (rowMin == INF_COST) continue;
        totalReduction += rowMin;
        for (int j = 0; j < size; j++) {
            if (mat[i][j] != INF_COST) {
                mat[i][j] -= rowMin;
            }
        }
    }

    // Column reduction
    for (int j = 0; j < size; j++) {
        int colMin = INF_COST;
        for (int i = 0; i < size; i++) {
            colMin = min(colMin, mat[i][j]);
        }
        if (colMin == INF_COST) continue;
        totalReduction += colMin;
        for (int i = 0; i < size; i++) {
            if (mat[i][j] != INF_COST) {
                mat[i][j] -= colMin;
            }
        }
    }

    return totalReduction;
}

int main() {
    int cityCount;
    cout << "Enter number of cities: ";
    if (!(cin >> cityCount) || cityCount < 2) return 0;

    vector<vector<int>> originalCost(cityCount, vector<int>(cityCount));
    cout << "Enter cost matrix (0 for no road):\n";
    for (int i = 0; i < cityCount; i++) {
        for (int j = 0; j < cityCount; j++) {
            cin >> originalCost[i][j];
            if (i == j || originalCost[i][j] <= 0) {
                originalCost[i][j] = INF_COST;
            }
        }
    }

    StateNode rootState{originalCost, {0}, 0};
    rootState.lowerBound = reduceCostMatrix(rootState.costMatrix);

    priority_queue<StateNode, vector<StateNode>, StateComparator> pq;
    pq.push(rootState);

    int bestCost = INF_COST;
    vector<int> bestTour;

    while (!pq.empty()) {
        StateNode curr = pq.top();
        pq.pop();

        if (curr.lowerBound >= bestCost) continue;

        int lastCity = curr.route.back();

        if ((int)curr.route.size() == cityCount) {
            if (originalCost[lastCity][0] == INF_COST) continue;

            int tourCost = originalCost[lastCity][0];
            for (int k = 1; k < cityCount; k++) {
                tourCost += originalCost[curr.route[k - 1]][curr.route[k]];
            }

            if (tourCost < bestCost) {
                bestCost = tourCost;
                bestTour = curr.route;
            }
            continue;
        }

        for (int nextCity = 1; nextCity < cityCount; nextCity++) {
            if (find(curr.route.begin(), curr.route.end(), nextCity) != curr.route.end()) {
                continue;
            }
            if (curr.costMatrix[lastCity][nextCity] == INF_COST) continue;

            StateNode nextState = curr;
            nextState.route.push_back(nextCity);
            nextState.lowerBound += curr.costMatrix[lastCity][nextCity];

            // Invalidate outward edges from lastCity and inward edges to nextCity
            for (int j = 0; j < cityCount; j++) nextState.costMatrix[lastCity][j] = INF_COST;
            for (int i = 0; i < cityCount; i++) nextState.costMatrix[i][nextCity] = INF_COST;
            if ((int)nextState.route.size() < cityCount) {
                nextState.costMatrix[nextCity][0] = INF_COST;
            }

            nextState.lowerBound += reduceCostMatrix(nextState.costMatrix);

            if (nextState.lowerBound < bestCost) {
                pq.push(nextState);
            }
        }
    }

    if (bestCost == INF_COST) {
        cout << "No possible tour\n";
    } else {
        cout << "\nMinimum cost: " << bestCost << "\nRoute: ";
        for (int c : bestTour) {
            cout << c << " -> ";
        }
        cout << "0\n";
    }

    return 0;
}
