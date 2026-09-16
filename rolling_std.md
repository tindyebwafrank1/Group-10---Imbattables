Rolling Standard Deviation (rolling_std)

1. Task Requirements
The task is to implement a C++ program that calculates the rollingstandard deviation of a set of numerical values.
The program allows the user to enter a number of values and specify the size of the rolling window. The standard 
deviation is then calculated for each consecutive window as the window movesthrough the data.
The program also displays the values in each window togetherwith their calculated standard deviation.

2. Implementation Strategy
The program stores the input values in a vector of type 'double'.
A function called 'rolling_std()' was created to calculate the standard deviation for one particular window.
For each window, the program first calculates the mean of the values.
It then calculates the squared difference between each value and the mean. These squared differences are added 
together and divided by the window size.
Finally, the square root of the result is calculated to obtain the standard deviation.

The main function moves the window one position at a time until all possible windows have been processed.

3. Key Decisions and Approaches
A vector was used to store the input values because the number of values is entered by the user at run time.
The 'rolling_std()' function was separated from the 'main()' function to make the program easier to understand and reuse.

The program uses the population standard deviation formula:

standard deviation = sqrt(sum of squared differences / window size)

The '<cmath>' library was used for the 'sqrt()' and 'pow()' functions.

The program also checks whether the window size is greater than zero and does not exceed the number of input values.

4. Testing
The program was tested using a set of seven(7) numerical values and a window size of 4.
For example, the input numerical values were:

10 25 30 45 50 65 75

with a window size of:
4

The program successfully moved the window through the data and calculated the standard deviation for every possible group 
of three consecutive values.
The results were checked by manually calculating the standard deviation of the first window.

5. Working Example
Input:
Enter the number of values: 7
Enter 7 values: 10 25 30 45 50 65 75
Enter the window size: 4

Output:
Rolling standard deviations:
Window 10 25 30 45 -> Standard deviation = 12.5
Window 25 30 45 50 -> Standard deviation = 10.3078
Window 30 45 50 65 -> Standard deviation = 12.5
Window 45 50 65 75 -> Standard deviation = 11.9242