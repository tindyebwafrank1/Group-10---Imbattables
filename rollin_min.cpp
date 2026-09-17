

#include <algorithm>
#include <cmath>       // for isnan, quiet_NaN
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// A Column is just a name + a list of doubles.
// A NaN value represents a missing value (Polars "null").
struct Column {
    string name;
    vector<double> values;
};

class DataFrame {
public:
    explicit DataFrame(vector<Column> cols) : columns_(move(cols)) {}

    // Find a column by name. Throws if it does not exist.
    Column& column(const string& name) {
        for (auto& c : columns_) {
            if (c.name == name) return c;
        }
        throw invalid_argument("Column not found: " + name);
    }

    const Column& column(const string& name) const {
        for (const auto& c : columns_) {
            if (c.name == name) return c;
        }
        throw invalid_argument("Column not found: " + name);
    }

    // Append a new column to the frame.
    void add_column(Column col) { columns_.push_back(move(col)); }

    const vector<Column>& columns() const { return columns_; }

private:
    vector<Column> columns_;
};

// For each index i in the input column, we look at the window
//     values[max(0, i - window + 1) .. i]
// and take the minimum of the values that are not null (NaN).
//
// Behaviour mirrors Polars' rolling_min:
//   * If the window contains only nulls, the result is null (NaN).
//   * Nulls inside the window are simply skipped (not treated as zero).
//   * min_periods controls how many non-null values must be present for a
//     result to be produced; otherwise the result is null. Default = 1.
//
vector<double> rolling_min(
    const vector<double>& values,
    size_t window,
    size_t min_periods = 1)
{
    if (window == 0) {
        throw invalid_argument("window must be >= 1");
    }
    if (min_periods == 0) {
        throw invalid_argument("min_periods must be >= 1");
    }
    if (min_periods > window) {
        throw invalid_argument("min_periods cannot exceed window");
    }

    const size_t n = values.size();
    vector<double> result;
    result.reserve(n);

    for (size_t i = 0; i < n; ++i) {
        // Window is [start, i] inclusive.
        const size_t start = (i + 1 >= window) ? (i + 1 - window) : 0;

        double current_min = numeric_limits<double>::infinity();
        size_t seen = 0;

        for (size_t j = start; j <= i; ++j) {
            // A NaN value means "null", so we skip it.
            if (!isnan(values[j])) {
                current_min = min(current_min, values[j]);
                ++seen;
            }
        }

        if (seen >= min_periods && seen > 0) {
            result.push_back(current_min);
        } else {
            result.push_back(numeric_limits<double>::quiet_NaN());
        }
    }

    return result;
}


// Convenience wrapper: rolling_min on a named column, producing a new column.
Column rolling_min_column(
    const DataFrame& df,
    const string& column_name,
    size_t window,
    size_t min_periods = 1,
    const string& result_name = "")
{
    const Column& src = df.column(column_name);
    Column out;
    out.name = result_name.empty()
                   ? column_name + "_rolling_min_" + to_string(window)
                   : result_name;
    out.values = rolling_min(src.values, window, min_periods);
    return out;
}


void print_column(const Column& col) {
    cout << col.name << ": [";
    for (size_t i = 0; i < col.values.size(); ++i) {
        if (!isnan(col.values[i])) {
            cout << col.values[i];
        } else {
            cout << "null";
        }
        if (i + 1 < col.values.size()) cout << ", ";
    }
    cout << "]\n";
}


int main() {
    // NaN is our "null" marker — defined once for readability.
    const double NULL_VALUE = numeric_limits<double>::quiet_NaN();

    DataFrame df({
        Column{"a", {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0}}
    });

    cout << "Input:\n";
    print_column(df.column("a"));

    // window = 3, default min_periods = 1
    Column r1 = rolling_min_column(df, "a", 3);
    cout << "\nrolling_min(window=3):\n";
    print_column(r1);

    // window = 3, min_periods = 3 (no output until a full window is seen)
    Column r2 = rolling_min_column(df, "a", 3, 3);
    cout << "\nrolling_min(window=3, min_periods=3):\n";
    print_column(r2);

    // Demonstrate null handling.
    DataFrame df2({
        Column{"a", {1.0, NULL_VALUE, 3.0, NULL_VALUE, 5.0}}
    });
    cout << "\nInput with nulls:\n";
    print_column(df2.column("a"));

    Column r3 = rolling_min_column(df2, "a", 3);
    cout << "\nrolling_min(window=3) with nulls:\n";
    print_column(r3);

    return 0;
}