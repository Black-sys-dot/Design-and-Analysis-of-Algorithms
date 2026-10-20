#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// Computes shortest path from source vertex to all other vertices using Dijkstra's algorithm
void dijkstraShortestPath(int srcNode,
                          const vector<vector<pair<int, int>>>& adjList,
                          vector<int>& shortestDist) {
    int totalVertices = adjList.size();

    shortestDist.assign(totalVertices, INT_MAX);
    shortestDist[srcNode] = 0;

    // Min-heap storing {distance, vertex}
    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > minHeap;

    minHeap.push({0, srcNode});

    while (!minHeap.empty()) {
        int currDist = minHeap.top().first;
        int uNode = minHeap.top().second;
        minHeap.pop();

        if (currDist > shortestDist[uNode]) {
            continue;
        }

        for (const auto& edge : adjList[uNode]) {
            int neighborNode = edge.first;
            int edgeWeight = edge.second;

            // Edge relaxation step
            if (shortestDist[neighborNode] > shortestDist[uNode] + edgeWeight) {
                shortestDist[neighborNode] = shortestDist[uNode] + edgeWeight;
                minHeap.push({shortestDist[neighborNode], neighborNode});
            }
        }
    }
}

int main() {
    int numVertices, numEdges;

    cout << "Enter number of intersections (vertices): ";
    if (!(cin >> numVertices) || numVertices <= 0) return 0;

    cout << "Enter number of roads (edges): ";
    cin >> numEdges;

    vector<vector<pair<int, int>>> roadNetwork(numVertices);

    cout << "Enter edges (u v w):\n";
    for (int i = 0; i < numEdges; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        if (u >= 0 && u < numVertices && v >= 0 && v < numVertices) {
            roadNetwork[u].push_back({v, w});
            roadNetwork[v].push_back({u, w});
        }
    }

    int ambulanceSource;
    cout << "Enter ambulance start location (source): ";
    cin >> ambulanceSource;

    int hospitalCount;
    cout << "Enter number of hospitals: ";
    cin >> hospitalCount;

    vector<int> hospitals(hospitalCount);
    cout << "Enter hospital nodes: ";
    for (int i = 0; i < hospitalCount; i++) {
        cin >> hospitals[i];
    }

    vector<int> shortestDistances;
    dijkstraShortestPath(ambulanceSource, roadNetwork, shortestDistances);

    int minTravelTime = INT_MAX;
    int closestHospital = -1;

    for (int hosp : hospitals) {
        if (hosp >= 0 && hosp < numVertices && shortestDistances[hosp] < minTravelTime) {
            minTravelTime = shortestDistances[hosp];
            closestHospital = hosp;
        }
    }

    if (closestHospital == -1 || minTravelTime == INT_MAX) {
        cout << "No hospital reachable.\n";
    } else {
        cout << "Nearest hospital is at node "
             << closestHospital
             << " with travel time "
             << minTravelTime
             << " minutes.\n";
    }

    return 0;
}
