// rolling_min.cpp
// A minimal, human-readable re-implementation of Polars' DataFrame.rolling_min()
// for a single numeric column.

#include <algorithm>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

// A Column is just a name + a list of optional doubles.
// std::nullopt represents a missing value (Polars "null").
struct Column {
    std::string name;
    std::vector<std::optional<double>> values;
};

class DataFrame {
public:
    explicit DataFrame(std::vector<Column> cols) : columns_(std::move(cols)) {}

    // Find a column by name. Throws if it does not exist.
    Column& column(const std::string& name) {
        for (auto& c : columns_) {
            if (c.name == name) return c;
        }
        throw std::invalid_argument("Column not found: " + name);
    }

    const Column& column(const std::string& name) const {
        for (const auto& c : columns_) {
            if (c.name == name) return c;
        }
        throw std::invalid_argument("Column not found: " + name);
    }

    // Append a new column to the frame.
    void add_column(Column col) { columns_.push_back(std::move(col)); }

    const std::vector<Column>& columns() const { return columns_; }

private:
    std::vector<Column> columns_;
};

// For each index i in the input column, we look at the window
//     values[max(0, i - window + 1) .. i]
// and take the minimum of the values that are not null.
//
// Behaviour mirrors Polars' rolling_min:
//   * If the window contains only nulls, the result is null.
//   * Nulls inside the window are simply skipped (not treated as zero).
//   * min_periods controls how many non-null values must be present for a
//     result to be produced; otherwise the result is null. Default = 1.
// 
std::vector<std::optional<double>> rolling_min(
    const std::vector<std::optional<double>>& values,
    std::size_t window,
    std::size_t min_periods = 1)
{
    if (window == 0) {
        throw std::invalid_argument("window must be >= 1");
    }
    if (min_periods == 0) {
        throw std::invalid_argument("min_periods must be >= 1");
    }
    if (min_periods > window) {
        throw std::invalid_argument("min_periods cannot exceed window");
    }

    const std::size_t n = values.size();
    std::vector<std::optional<double>> result;
    result.reserve(n);

    for (std::size_t i = 0; i < n; ++i) {
        // Window is [start, i] inclusive.
        const std::size_t start = (i + 1 >= window) ? (i + 1 - window) : 0;

        double current_min = std::numeric_limits<double>::infinity();
        std::size_t seen = 0;

        for (std::size_t j = start; j <= i; ++j) {
            if (values[j].has_value()) {
                current_min = std::min(current_min, *values[j]);
                ++seen;
            }
        }

        if (seen >= min_periods && seen > 0) {
            result.push_back(current_min);
        } else {
            result.push_back(std::nullopt);
        }
    }

    return result;
}


// Convenience wrapper: rolling_min on a named column, producing a new column.

Column rolling_min_column(
    const DataFrame& df,
    const std::string& column_name,
    std::size_t window,
    std::size_t min_periods = 1,
    const std::string& result_name = "")
{
    const Column& src = df.column(column_name);
    Column out;
    out.name = result_name.empty()
                   ? column_name + "_rolling_min_" + std::to_string(window)
                   : result_name;
    out.values = rolling_min(src.values, window, min_periods);
    return out;
}


void print_column(const Column& col) {
    std::cout << col.name << ": [";
    for (std::size_t i = 0; i < col.values.size(); ++i) {
        if (col.values[i].has_value()) {
            std::cout << *col.values[i];
        } else {
            std::cout << "null";
        }
        if (i + 1 < col.values.size()) std::cout << ", ";
    }
    std::cout << "]\n";
}


int main() {
    DataFrame df({
        Column{"a", {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0}}
    });

    std::cout << "Input:\n";
    print_column(df.column("a"));

    // window = 3, default min_periods = 1
    Column r1 = rolling_min_column(df, "a", 3);
    std::cout << "\nrolling_min(window=3):\n";
    print_column(r1);

    // window = 3, min_periods = 3 (no output until a full window is seen)
    Column r2 = rolling_min_column(df, "a", 3, 3);
    std::cout << "\nrolling_min(window=3, min_periods=3):\n";
    print_column(r2);

    // Demonstrate null handling.
    DataFrame df2({
        Column{"a", {1.0, std::nullopt, 3.0, std::nullopt, 5.0}}
    });
    std::cout << "\nInput with nulls:\n";
    print_column(df2.column("a"));

    Column r3 = rolling_min_column(df2, "a", 3);
    std::cout << "\nrolling_min(window=3) with nulls:\n";
    print_column(r3);

    return 0;
}