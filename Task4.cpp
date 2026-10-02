#include <iostream>
using namespace std;

class DisasterRescue {
public:
    int** people;
    int** units;
    int N;
    int M;
    int capacity;
    bool** selected;
    int maxPeople;
    bool** bestSelected;
    int bestUnitsUsed;
    int highRiskThreshold;

    DisasterRescue(int** p, int** u, int n, int m, int cap) {
        N = n;
        M = m;
        capacity = cap;
        highRiskThreshold = 10;
        people = new int*[N];
        units = new int*[N];
        selected = new bool*[N];
        bestSelected = new bool*[N];
        for (int i = 0; i < N; i++) {
            people[i] = new int[M];
            units[i] = new int[M];
            selected[i] = new bool[M];
            bestSelected[i] = new bool[M];
            for (int j = 0; j < M; j++) {
                people[i][j] = p[i][j];
                units[i][j] = u[i][j];
                selected[i][j] = false;
                bestSelected[i][j] = false;
            }
        }
        maxPeople = -1;
        bestUnitsUsed = 0;
    }

    ~DisasterRescue() {
        for (int i = 0; i < N; i++) {
            delete[] people[i];
            delete[] units[i];
            delete[] selected[i];
            delete[] bestSelected[i];
        }
        delete[] people;
        delete[] units;
        delete[] selected;
        delete[] bestSelected;
    }

    bool isHighRisk(int i, int j) {
        return people[i][j] >= highRiskThreshold;
    }

    int countHighRiskSelected() {
        int count = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (selected[i][j] && isHighRisk(i, j)) {
                    count++;
                }
            }
        }
        return count;
    }

    bool rowHasSelection(int row) {
        bool hasAffected = false;
        for (int j = 0; j < M; j++) {
            if (people[row][j] > 0) {
                hasAffected = true;
                break;
            }
        }
        if (!hasAffected) return true;
        for (int j = 0; j < M; j++) {
            if (selected[row][j]) return true;
        }
        return false;
    }

    bool allRowsSatisfied() {
        for (int i = 0; i < N; i++) {
            if (!rowHasSelection(i)) return false;
        }
        return true;
    }

    void backtrack(int row, int col, int currentPeople, int currentUnits) {
        if (row == N) {
            if (allRowsSatisfied() && currentPeople > maxPeople) {
                maxPeople = currentPeople;
                bestUnitsUsed = currentUnits;
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < M; j++) {
                        bestSelected[i][j] = selected[i][j];
                    }
                }
            }
            return;
        }

        int nextRow = row;
        int nextCol = col + 1;
        if (nextCol == M) {
            nextRow = row + 1;
            nextCol = 0;
        }

        if (people[row][col] == -1) {
            backtrack(nextRow, nextCol, currentPeople, currentUnits);
            return;
        }

        backtrack(nextRow, nextCol, currentPeople, currentUnits);

        int extra = 0;
        if (isHighRisk(row, col) && countHighRiskSelected() >= 1) {
            extra = 1;
        }
        int needed = units[row][col] + extra;
        if (currentUnits + needed <= capacity) {
            selected[row][col] = true;
            backtrack(nextRow, nextCol, currentPeople + people[row][col], currentUnits + needed);
            selected[row][col] = false;
        }
    }

    void solve() {
        backtrack(0, 0, 0, 0);

        if (maxPeople == -1) {
            cout << "No valid allocation exists." << endl;
            return;
        }

        cout << "Selected Positions (row, col):" << endl;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (bestSelected[i][j]) {
                    cout << "(" << i << "," << j << ") ";
                }
            }
        }
        cout << endl;

        cout << "Total People Rescued: " << maxPeople << endl;
        cout << "Total Rescue Units Used: " << bestUnitsUsed << endl;
        cout << "Remaining Units: " << (capacity - bestUnitsUsed) << endl;
    }
};

int main() {
    int N = 4;
    int M = 4;
    int capacity = 15;

    int** people = new int*[N];
    int** units = new int*[N];
    for (int i = 0; i < N; i++) {
        people[i] = new int[M];
        units[i] = new int[M];
    }

    people[0][0] = 8;  people[0][1] = 12; people[0][2] = -1; people[0][3] = 6;
    people[1][0] = 5;  people[1][1] = 10; people[1][2] = 7;  people[1][3] = -1;
    people[2][0] = -1; people[2][1] = 9;  people[2][2] = 15; people[2][3] = 4;
    people[3][0] = 6;  people[3][1] = -1; people[3][2] = 8;  people[3][3] = 11;

    units[0][0] = 2;  units[0][1] = 3;  units[0][2] = -1; units[0][3] = 2;
    units[1][0] = 1;  units[1][1] = 3;  units[1][2] = 2;  units[1][3] = -1;
    units[2][0] = -1; units[2][1] = 2;  units[2][2] = 4;  units[2][3] = 1;
    units[3][0] = 2;  units[3][1] = -1; units[3][2] = 2;  units[3][3] = 3;

    cout << "People Matrix:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cout << people[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\nRescue Units Required:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cout << units[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\nCapacity: " << capacity << endl << endl;

    DisasterRescue system(people, units, N, M, capacity);
    system.solve();

    for (int i = 0; i < N; i++) {
        delete[] people[i];
        delete[] units[i];
    }
    delete[] people;
    delete[] units;

    return 0;
}