#include <iostream>
#include <string>
using namespace std;

class LockCodeGenerator {
public:
    string targetCode;
    int N;
    int targetSum;
    bool found;
    bool used[10];

    LockCodeGenerator(int n, int sum, string target) {
        N = n;
        targetSum = sum;
        targetCode = target;
        found = false;
        for (int i = 0; i < 10; i++) {
            used[i] = false;
        }
    }

    void resetUsed(int index) {
        if (index >= 10) return;
        used[index] = false;
        resetUsed(index + 1);
    }

    void generate(string current, int position, int currentSum) {
        if (position == N) {
            if (currentSum == targetSum) {
                cout << current << endl;
                if (current == targetCode) {
                    found = true;
                }
            }
            return;
        }

        tryDigit(current, position, currentSum, 0);
    }

    void tryDigit(string current, int position, int currentSum, int digit) {
        if (digit > 9) return;

        if (!used[digit]) {
            if (position == 0 && digit == 0) {
                tryDigit(current, position, currentSum, digit + 1);
                return;
            }

            used[digit] = true;
            generate(current + char('0' + digit), position + 1, currentSum + digit);
            used[digit] = false;
        }

        tryDigit(current, position, currentSum, digit + 1);
    }

    void solve() {
        cout << "Valid Lock Codes:" << endl;
        generate("", 0, 0);
        cout << endl;
        if (found) {
            cout << "Target code " << targetCode << " CAN be formed." << endl;
        } else {
            cout << "Target code " << targetCode << " CANNOT be formed." << endl;
        }
    }
};

int main() {
    int N = 3;
    int targetSum = 6;
    string targetCode = "123";
    cout << "N = " << N << endl;
    cout << "target sum = " << targetSum << endl;
    cout << "target code = " << targetCode << endl;
    LockCodeGenerator generator(N, targetSum, targetCode);
    generator.solve();

    return 0;
}