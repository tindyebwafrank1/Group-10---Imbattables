
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

int main()
{
    int n, window;

    cout << "Enter number of values: ";
    cin >> n;

    cout << "Enter window size: ";
    cin >> window;

    if (n <= 0 || window <= 0 || window > n)
    {
        cout << "Invalid input." << endl;
        return 0;
    }

    vector<double> numbers(n);

    cout << "Enter " << n << " values: ";
    for (int i = 0; i < n; i++)
    {
        cin >> numbers[i];
    }

    cout << "Moving averages: ";

    for (int i = 0; i <= n - window; i++)
    {
        double sum = 0;

        for (int j = i; j < i + window; j++)
        {
            sum += numbers[j];
        }

        double average = sum / window;

        cout << fixed << setprecision(2)
             << average << " ";
    }

    cout << endl;

    return 0;
}

