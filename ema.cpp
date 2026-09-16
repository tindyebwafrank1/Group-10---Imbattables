#include <iostream>
#include <vector>
#include <iomanip>

// Computes the Exponential Moving Average (EMA) for a series of prices.
//
// Formula:
//   multiplier = 2 / (period + 1)
//   EMA[0]     = SMA of the first `period` values (seed value)
//   EMA[i]     = (price[i] - EMA[i-1]) * multiplier + EMA[i-1]
//
// Returns a vector the same length as `prices`, where the first
// (period - 1) entries are left as 0.0 because there isn't enough
// data yet to seed the average.
std::vector<double> computeEMA(const std::vector<double>& prices, int period) {
    std::vector<double> ema(prices.size(), 0.0);

    if (period <= 0 || prices.size() < static_cast<size_t>(period)) {
        std::cerr << "Error: not enough data points for the given period.\n";
        return ema;
    }

    double multiplier = 2.0 / (period + 1);

    // Seed EMA with the Simple Moving Average (SMA) of the first `period` values
    double sum = 0.0;
    for (int i = 0; i < period; ++i) {
        sum += prices[i];
    }
    double sma = sum / period;
    ema[period - 1] = sma;

    // Apply the EMA formula for the rest of the series
    for (size_t i = period; i < prices.size(); ++i) {
        ema[i] = (prices[i] - ema[i - 1]) * multiplier + ema[i - 1];
    }

    return ema;
}

void printResults(const std::vector<double>& prices,
                   const std::vector<double>& ema,
                   int period) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nIndex | Price   | EMA(" << period << ")\n";
    std::cout << "------|---------|--------\n";
    for (size_t i = 0; i < prices.size(); ++i) {
        std::cout << std::setw(5) << i << " | "
                  << std::setw(7) << prices[i] << " | ";
        if (i < static_cast<size_t>(period) - 1) {
            std::cout << "  --\n";
        } else {
            std::cout << ema[i] << "\n";
        }
    }
}

int main() {
    // Example input: daily closing prices
    std::vector<double> prices = {
        22.27, 22.19, 22.08, 22.17, 22.18,
        22.13, 22.23, 22.43, 22.24, 22.29,
        22.15, 22.39, 22.38, 22.61, 23.36
    };

    int period = 5; // e.g., 5-day EMA

    std::vector<double> ema = computeEMA(prices, period);
    printResults(prices, ema, period);

    return 0;
}
