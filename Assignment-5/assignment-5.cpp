#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <iomanip>

using namespace std;

// Structure to represent a directed stage edge with base and dynamic cost
struct StageEdge {
    int targetNode;
    double baseWeight;
    double activeCost;
};

int main() {
    int numStages, totalEdges;

    cout << "Enter number of stages: ";
    if (!(cin >> numStages) || numStages <= 1) return 0;

    vector<int> stageCount(numStages);
    vector<int> stageOffset(numStages);
    int totalNodes = 0;

    cout << "Enter nodes in each stage: ";
    for (int s = 0; s < numStages; s++) {
        cin >> stageCount[s];
        stageOffset[s] = totalNodes;
        totalNodes += stageCount[s];
    }

    vector<vector<StageEdge>> forwardGraph(totalNodes);
    vector<vector<int>> reverseGraph(totalNodes);

    cout << "Enter number of edges: ";
    cin >> totalEdges;

    cout << "Enter edges (u v cost):\n";
    for (int i = 0; i < totalEdges; i++) {
        int fromU, toV;
        double costVal;
        cin >> fromU >> toV >> costVal;
        if (fromU >= 0 && fromU < totalNodes && toV >= 0 && toV < totalNodes) {
            forwardGraph[fromU].push_back({toV, costVal, costVal});
            reverseGraph[toV].push_back(fromU);
        }
    }

    const double INF_VAL = 1e18;
    vector<double> minCost(totalNodes, INF_VAL);
    vector<int> nextHop(totalNodes, -1);

    // Terminal stage nodes have 0 cost to reach target
    for (int i = stageOffset[numStages - 1]; i < totalNodes; i++) {
        minCost[i] = 0;
    }

    // Function to calculate optimal path from node u
    auto calculateOptimal = [&](int u) {
        double optimalCost = INF_VAL;
        int optimalNext = -1;

        for (const auto& e : forwardGraph[u]) {
            if (e.activeCost + minCost[e.targetNode] < optimalCost) {
                optimalCost = e.activeCost + minCost[e.targetNode];
                optimalNext = e.targetNode;
            }
        }

        minCost[u] = optimalCost;
        nextHop[u] = optimalNext;
    };

    // Backward pass DP across stages
    for (int s = numStages - 2; s >= 0; s--) {
        for (int i = stageOffset[s]; i < stageOffset[s] + stageCount[s]; i++) {
            calculateOptimal(i);
        }
    }

    cout << "\nMinimum costs:\n";
    for (int i = 0; i < stageCount[0]; i++) {
        if (minCost[i] >= INF_VAL / 2) {
            cout << i << ": Unreachable\n";
        } else {
            cout << i << ": " << minCost[i] << "\n";
        }
    }

    int sourceNode;
    cout << "\nEnter source node (-1 to skip): ";
    cin >> sourceNode;

    if (sourceNode >= 0 && sourceNode < stageCount[0]) {
        if (minCost[sourceNode] >= INF_VAL / 2) {
            cout << "No path\n";
        } else {
            cout << "Path: ";
            for (int curr = sourceNode; curr != -1; curr = nextHop[curr]) {
                cout << curr;
                if (nextHop[curr] != -1) cout << " -> ";
            }
            cout << "\nCost: " << minCost[sourceNode] << "\n";
        }
    }

    int updateCount;
    cout << "\nEnter number of updates: ";
    cin >> updateCount;

    while (updateCount--) {
        int uEdge, vEdge;
        double multiplierFactor;

        cout << "Enter u v multiplier: ";
        cin >> uEdge >> vEdge >> multiplierFactor;

        for (auto &edge : forwardGraph[uEdge]) {
            if (edge.targetNode == vEdge) {
                edge.activeCost = edge.baseWeight * multiplierFactor;
            }
        }

        // Propagate updates backward through affected nodes
        queue<int> updateQueue;
        updateQueue.push(uEdge);

        while (!updateQueue.empty()) {
            int current = updateQueue.front();
            updateQueue.pop();

            double priorCost = minCost[current];
            int priorHop = nextHop[current];
            calculateOptimal(current);

            if (abs(priorCost - minCost[current]) > 1e-9 || priorHop != nextHop[current]) {
                for (int pred : reverseGraph[current]) {
                    updateQueue.push(pred);
                }
            }
        }
    }

    cout << "\nUpdated minimum costs:\n";
    for (int i = 0; i < stageCount[0]; i++) {
        if (minCost[i] >= INF_VAL / 2) {
            cout << i << ": Unreachable\n";
        } else {
            cout << i << ": " << minCost[i] << "\n";
        }
    }

    return 0;
}
