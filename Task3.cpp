#include <iostream>
using namespace std;

class Student {
public:
    int id;
    int minTokens;
    int maxTokens;
    int category;
    int priority;
    int allocated;
    Student() {
        id = 0;
        minTokens = 0;
        maxTokens = 0;
        category = 0;
        priority = 0;
        allocated = 0;
    }
    Student(int i, int mn, int mx, int cat, int pri) {
        id = i;
        minTokens = mn;
        maxTokens = mx;
        category = cat;
        priority = pri;
        allocated = 0;
    }
};

class MessTokenSystem {
public:
    Student* students;
    int N;
    int totalTokens;
    int emergencyReserve;
    int emergencyUsed;
    int threshold;
    int* categoryTotal;
    bool found;

    MessTokenSystem(Student* s, int n, int total, int emerg, int thresh) {
        N = n;
        students = new Student[N];
        for (int i = 0; i < N; i++) {
            students[i] = s[i];
        }
        totalTokens = total;
        emergencyReserve = emerg;
        emergencyUsed = 0;
        threshold = thresh;
        categoryTotal = new int[3];
        for (int i = 0; i < 3; i++) {
            categoryTotal[i] = 0;
        }
        found = false;
    }

    ~MessTokenSystem() {
        delete[] students;
        delete[] categoryTotal;
    }

    bool sameCategoryEqual() {
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                if (students[i].category == students[j].category &&
                    students[i].allocated == students[j].allocated &&
                    students[i].allocated > 0) {
                    return true;
                }
            }
        }
        return false;
    }

    bool priorityRespected() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (students[i].priority > students[j].priority) {
                    if (students[i].allocated < students[j].allocated) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool highLowThreshold() {
        int highTotal = 0;
        int lowTotal = 0;
        for (int i = 0; i < N; i++) {
            if (students[i].priority >= 2) {
                highTotal += students[i].allocated;
            } else {
                lowTotal += students[i].allocated;
            }
        }
        return (highTotal >= lowTotal + threshold);
    }

    bool hasMinAndMax() {
        bool hasMin = false;
        bool hasMax = false;
        for (int i = 0; i < N; i++) {
            if (students[i].allocated == students[i].minTokens) hasMin = true;
            if (students[i].allocated == students[i].maxTokens) hasMax = true;
        }
        return hasMin && hasMax;
    }

    bool categoryLimitsOk() {
        for (int i = 0; i < 3; i++) {
            categoryTotal[i] = 0;
        }
        for (int i = 0; i < N; i++) {
            categoryTotal[students[i].category] += students[i].allocated;
        }
        if (categoryTotal[0] > 15) return false;
        if (categoryTotal[1] > 20) return false;
        if (categoryTotal[2] > 12) return false;
        return true;
    }

    bool studentRestrictionsOk() {
        if (students[0].allocated > 0 && students[1].allocated > 0) {
            if (students[0].allocated + students[1].allocated > 12) return false;
        }
        return true;
    }

    bool isValid() {
        if (sameCategoryEqual()) return false;
        if (!priorityRespected()) return false;
        if (!highLowThreshold()) return false;
        if (!hasMinAndMax()) return false;
        if (!categoryLimitsOk()) return false;
        if (!studentRestrictionsOk()) return false;
        return true;
    }

    void backtrack(int index, int remaining) {
        if (found) return;
        if (index == N) {
            if (remaining >= 0 && isValid()) {
                found = true;
                emergencyUsed = (remaining < 0) ? -remaining : 0;
            }
            return;
        }

        for (int t = students[index].minTokens; t <= students[index].maxTokens; t++) {
            if (t > remaining + emergencyReserve) continue;
            students[index].allocated = t;
            backtrack(index + 1, remaining - t);
            if (found) return;
            students[index].allocated = 0;
        }
    }

    void solve() {
        backtrack(0, totalTokens);
        if (!found) {
            cout << "No valid allocation found even with emergency tokens." << endl;
            return;
        }

        cout << "Final Allocation:" << endl;
        for (int i = 0; i < N; i++) {
            cout << "Student " << students[i].id 
                 << " (Cat:" << students[i].category 
                 << ", Pri:" << students[i].priority << ") -> "
                 << students[i].allocated << " tokens" << endl;
        }

        cout << "\nCategory Totals:" << endl;
        cout << "Breakfast: " << categoryTotal[0] << endl;
        cout << "Lunch: " << categoryTotal[1] << endl;
        cout << "Dinner: " << categoryTotal[2] << endl;

        cout << "\nEmergency tokens used: " << emergencyUsed << endl;
        int used = 0;
        for (int i = 0; i < N; i++) {
            used += students[i].allocated;
        }
        cout << "Unused tokens: " << (totalTokens + emergencyUsed - used) << endl;
    }
};

int main() {
    int N = 4;
    Student* students = new Student[N];
    students[0] = Student(1, 2, 5, 0, 3);
    students[1] = Student(2, 1, 4, 0, 1);
    students[2] = Student(3, 3, 6, 1, 2);
    students[3] = Student(4, 2, 5, 2, 2);

    int totalTokens = 14;
    int emergencyReserve = 3;
    int threshold = 2;

    cout << "Students:" << endl;
    for (int i = 0; i < N; i++) {
        cout << "ID:" << students[i].id 
             << " Min:" << students[i].minTokens 
             << " Max:" << students[i].maxTokens 
             << " Cat:" << students[i].category 
             << " Pri:" << students[i].priority << endl;
    }
    cout << "Total Tokens: " << totalTokens << " | Emergency Reserve: " << emergencyReserve 
         << " | Threshold: " << threshold << endl << endl;

    MessTokenSystem system(students, N, totalTokens, emergencyReserve, threshold);
    system.solve();

    delete[] students;
    return 0;
}