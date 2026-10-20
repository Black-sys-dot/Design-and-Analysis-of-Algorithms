#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    vector<string> courseSubjects = {
        "DAA", "AIML", "SPM", "EAC", "MDM-3"
    };

    int totalSubjects = courseSubjects.size();
    vector<vector<int>> conflictMatrix(totalSubjects, vector<int>(totalSubjects, 0));

    int conflictCount;
    cout << "Enter number of subject conflicts: ";
    if (!(cin >> conflictCount)) return 0;

    cout << "Subject IDs:\n";
    for (int i = 0; i < totalSubjects; i++) {
        cout << i << " - " << courseSubjects[i] << "\n";
    }

    cout << "\nEnter conflicting subject pairs (u v):\n";
    for (int i = 0; i < conflictCount; i++) {
        int u, v;
        cin >> u >> v;

        if (u >= 0 && u < totalSubjects && v >= 0 && v < totalSubjects && u != v) {
            conflictMatrix[u][v] = 1;
            conflictMatrix[v][u] = 1;
        }
    }

    // Greedy Graph Coloring for time slot assignment
    vector<int> assignedColor(totalSubjects, -1);

    for (int u = 0; u < totalSubjects; u++) {
        vector<bool> colorOccupied(totalSubjects, false);

        for (int v = 0; v < totalSubjects; v++) {
            if (conflictMatrix[u][v] && assignedColor[v] != -1) {
                colorOccupied[assignedColor[v]] = true;
            }
        }

        int availableColor = 0;
        while (colorOccupied[availableColor]) {
            availableColor++;
        }

        assignedColor[u] = availableColor;
    }

    // Room Allocation logic
    int roomCount, roomCapacity;
    cout << "\nEnter number of rooms: ";
    cin >> roomCount;

    cout << "Enter capacity of each room: ";
    cin >> roomCapacity;

    vector<int> studentCount(totalSubjects);
    cout << "\nEnter student count for each subject:\n";

    for (int i = 0; i < totalSubjects; i++) {
        cout << courseSubjects[i] << ": ";
        cin >> studentCount[i];
    }

    int totalSlots = *max_element(assignedColor.begin(), assignedColor.end()) + 1;

    cout << "\n----- EXAM TIMETABLE -----\n";

    for (int slot = 0; slot < totalSlots; slot++) {
        cout << "\nTime Slot " << slot + 1 << ":\n";

        int availableSeats = roomCount * roomCapacity;

        for (int i = 0; i < totalSubjects; i++) {
            if (assignedColor[i] == slot) {
                cout << courseSubjects[i];

                if (studentCount[i] <= availableSeats) {
                    cout << " - Allocated\n";
                    availableSeats -= studentCount[i];
                } else {
                    cout << " - Insufficient rooms\n";
                }
            }
        }
    }

    cout << "\nTotal time slots required: " << totalSlots << "\n";

    return 0;
}
