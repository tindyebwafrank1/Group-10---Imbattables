Rolling Maximum
Description
The rolling_max() function finds the maximum value in each consecutive window of a given size.
For example, if the data is:
{1, 3, 2, 5, 4, 6, 2}
and the window size is 3, the function checks:
{1, 3, 2} → 3
{3, 2, 5} → 5
{2, 5, 4} → 5
{5, 4, 6} → 6
{4, 6, 2} → 6
The final result is:
3 5 5 6 6
Code
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
How It Works
A vector called result is created to store the maximum values.
The first for loop moves the rolling window through the data.
maximum starts as the first value in the current window.
The second for loop checks every value in that window.
If a larger value is found, maximum is updated.
The maximum value is added to result.
The result is printed at the end.
Example Input
Data = {1, 3, 2, 5, 4, 6, 2}
Window size = 3
Output
3 5 5 6 6
Time Complexity
The algorithm has a time complexity of O(n × k) because each rolling window checks k elements.
Space Complexity
The extra space used for the result is O(n).
