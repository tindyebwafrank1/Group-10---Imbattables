 Moving Average Program in C++

 Description

This C++ program calculates the **moving average** of a set of numerical values using a user-defined window size.

A moving average is calculated by taking a fixed number of consecutive values, finding their average, and then moving the window one position at a time.

Source Code


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

 How the Program Works

 1. Include Libraries

- `iostream` is used for input and output.
- `iomanip` is used to format the output to two decimal places.
- `vector` is used to store the numerical values.

 2. Get Input

The program asks the user for:

- The number of values (`n`)
- The moving-average window size (`window`)

3. Validate Input

The program checks that:

- `n` is greater than 0.
- `window` is greater than 0.
- `window` is not larger than `n`.

If any condition is invalid, the program displays `Invalid input.` and stops.

4. Store the Values

A vector called `numbers` is created:

vector<double> numbers(n);

The user then enters the required values.

 5. Calculate the Moving Average

The outer loop moves the window through the vector:

for (int i = 0; i <= n - window; i++)

The inner loop adds the values inside the current window:

for (int j = i; j < i + window; j++)
{
    sum += numbers[j];
}

The average is then calculated using:


double average = sum / window;

 6. Format the Output

The result is displayed with two decimal places:

cout << fixed << setprecision(2)
     << average << " ";

 Example

Input

Enter number of values: 5
Enter window size: 3
Enter 5 values: 10 20 30 40 50
```

 Calculation

For a window size of 3:

- `(10 + 20 + 30) / 3 = 20.00`
- `(20 + 30 + 40) / 3 = 30.00`
- `(30 + 40 + 50) / 3 = 40.00`

 Output

Moving averages: 20.00 30.00 40.00

Formula

For a window containing `w` values:

\[
Moving\ Average = \frac{x_1+x_2+\cdots+x_w}{w}
\]

The window then moves one position forward and the calculation is repeated.

 Time Complexity

The program uses nested loops.

- **Time complexity:** `O(n × window)`
- **Space complexity:** `O(n)`

The space is `O(n)` because the program stores all input values in a vector.

Requirements

To compile and run the program, you need a C++ compiler such as:

- GCC / G++
- MinGW
- Microsoft Visual C++
- Clang

For example, using `g++`:

g++ moving_average.cpp -o moving_average

Then run:

./moving_average

On Windows, you can run:
moving_average.exe

