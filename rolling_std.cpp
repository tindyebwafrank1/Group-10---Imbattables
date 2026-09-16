#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Function to calculate the standard deviation of a window
double rolling_std(const vector<double>& data, int start, int windowSize)
{
    double sum = 0.0;

    // Calculate the sum of values in the window
    for (int i = start; i < start + windowSize; i++)
    {
        sum += data[i];
    }

    // Calculate the mean
    double mean = sum / windowSize;

    // Calculate the sum of squared differences
    double squaredSum = 0.0;

    for (int i = start; i < start + windowSize; i++)
    {
        squaredSum += pow(data[i] - mean, 2);
    }

    // Calculate population standard deviation
    double standardDeviation = sqrt(squaredSum / windowSize);

    return standardDeviation;
}

int main()
{
    int n;
    int windowSize;

    cout << "Enter the number of values: ";
    cin >> n;

    vector<double> data(n);

    cout << "Enter " << n << " values: ";

    for (int i = 0; i < n; i++)
    {
        cin >> data[i];
    }

    cout << "Enter the window size: ";
    cin >> windowSize;

    // Check whether the window size is valid
    if (windowSize <= 0 || windowSize > n)
    {
        cout << "Invalid window size." << endl;
        return 1;
    }

    cout << "\nRolling standard deviations:\n";

    // Move the window through the data
    for (int i = 0; i <= n - windowSize; i++)
    {
        double result = rolling_std(data, i, windowSize);

        cout << "Window ";

        for (int j = i; j < i + windowSize; j++)
        {
            cout << data[j] << " ";
        }

        cout << "-> Standard deviation = " << result << endl;
    }

    return 0;
}