#include <iostream>
using namespace std;

class Runner {
public:
    int id;
    int stamina;
    int speed;
    Runner() {
        id = 0;
        stamina = 0;
        speed = 0;
    }
    Runner(int i, int st, int sp) {
        id = i;
        stamina = st;
        speed = sp;
    }
};

class RelayArranger {
public:
    Runner* runners;
    int N;
    double avgStamina;
    int* currentOrder;
    bool* used;
    int** results;
    int resultCount;
    int resultCapacity;

    RelayArranger(Runner* r, int n) {
        N = n;
        runners = new Runner[N];
        for (int i = 0; i < N; i++) {
            runners[i] = r[i];
        }
        used = new bool[N];
        for (int i = 0; i < N; i++) {
            used[i] = false;
        }
        currentOrder = new int[N];
        int total = 0;
        for (int i = 0; i < N; i++) {
            total += runners[i].stamina;
        }
        avgStamina = (double)total / N;
        resultCount = 0;
        resultCapacity = 10;
        results = new int*[resultCapacity];
        for (int i = 0; i < resultCapacity; i++) {
            results[i] = new int[N];
        }
    }

    ~RelayArranger() {
        delete[] runners;
        delete[] used;
        delete[] currentOrder;
        for (int i = 0; i < resultCapacity; i++) {
            delete[] results[i];
        }
        delete[] results;
    }

    bool canHandover(int currIdx, int nextIdx) {
        int currStamina = runners[currIdx].stamina;
        int currSpeed = runners[currIdx].speed;
        int nextStamina = runners[nextIdx].stamina;
        return (currStamina + currSpeed) >= nextStamina;
    }

    bool finalTwoValid() {
        if (N < 2) return true;
        int last = currentOrder[N - 1];
        int secondLast = currentOrder[N - 2];
        if (runners[last].stamina < avgStamina) return false;
        if (runners[secondLast].stamina < avgStamina) return false;
        return true;
    }

    void addResult() {
        if (resultCount >= resultCapacity) {
            int newCap = resultCapacity * 2;
            int** newResults = new int*[newCap];
            for (int i = 0; i < newCap; i++) {
                newResults[i] = new int[N];
            }
            for (int i = 0; i < resultCount; i++) {
                for (int j = 0; j < N; j++) {
                    newResults[i][j] = results[i][j];
                }
            }
            for (int i = 0; i < resultCapacity; i++) {
                delete[] results[i];
            }
            delete[] results;
            results = newResults;
            resultCapacity = newCap;
        }
        for (int j = 0; j < N; j++) {
            results[resultCount][j] = currentOrder[j];
        }
        resultCount++;
    }

    void backtrack(int position) {
        if (position == N) {
            if (finalTwoValid()) {
                addResult();
            }
            return;
        }

        for (int i = 0; i < N; i++) {
            if (used[i]) continue;

            if (position > 0) {
                int prev = currentOrder[position - 1];
                if (!canHandover(prev, i)) continue;
            }

            used[i] = true;
            currentOrder[position] = i;
            backtrack(position + 1);
            used[i] = false;
        }
    }

    void solve() {
        backtrack(0);
        cout << "Valid Running Orders:" << endl;
        if (resultCount == 0) {
            cout << "None" << endl;
            return;
        }
        for (int i = 0; i < resultCount; i++) {
            for (int j = 0; j < N; j++) {
                cout << "R" << runners[results[i][j]].id;
                if (j < N - 1) cout << " -> ";
            }
            cout << endl;
        }
    }
};

int main() {
    int N = 3;
    Runner* runners = new Runner[N];
    runners[0] = Runner(1, 5, 6);
    runners[1] = Runner(2, 4, 5);
    runners[2] = Runner(3, 6, 4);

    cout << "Runners Details:" << endl;
    for (int i = 0; i < N; i++) {
        cout << "R" << runners[i].id << " -> Stamina: " << runners[i].stamina 
             << ", Speed: " << runners[i].speed << endl;
    }
    cout << endl;

    RelayArranger arranger(runners, N);
    arranger.solve();

    delete[] runners;
    return 0;
}