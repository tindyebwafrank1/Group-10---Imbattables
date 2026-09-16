#include <iostream>
#include <vector>

using namespace std;


vector<int> rolling_max(vector<int> data, int k) {
    vector<int> result;

    for (int i = 0; i <= data.size() - k; i++) {
        int maximum = data[i];
        

        for (int j = i; j < i + k; j++) {
            if (data[j] > maximum) {
                maximum = data[j];
            }
        }

        result.push_back(maximum);
    }

    return result;
}

int main() {
    vector<int> data = {1, 3, 2, 5, 4, 6, 2};
    int k = 3;

    vector<int> result = rolling_max(data, k);

    for (int x : result) {
        cout << x << " ";
    }

    return 0;
}
