# Task: Exponential Moving Average (EMA)

## Task Requirements
Implement a C++ program that computes the Exponential Moving Average (EMA)
for a series of numeric data (e.g., daily stock closing prices), given a
specified period (window size). The EMA gives more weight to recent data
points than a Simple Moving Average (SMA).

## Implementation Strategy
- `computeEMA(prices, period)` takes a vector of prices and an integer
  period, and returns a vector of EMA values of the same length.
- The multiplier (smoothing factor) is calculated as `2 / (period + 1)`.
- Since EMA needs a starting point, the first EMA value is seeded using
  the Simple Moving Average (SMA) of the first `period` prices. This is
  the standard, widely-used approach (e.g., used by Investopedia and most
  trading platforms).
- From that point on, each EMA value is calculated recursively:
  `EMA[i] = (price[i] - EMA[i-1]) * multiplier + EMA[i-1]`
- Indices before the seed point (i.e., before `period - 1`) don't have
  enough data to compute an EMA, so they are left blank (`--`) in the
  output rather than reporting a misleading value.
- Basic input validation guards against an invalid period (`<= 0`) or a
  dataset smaller than the period.

## Key Decisions / Approaches
- **Seeding with SMA** instead of just using the first price value, since
  this is the conventional and more statistically sound method.
- **Separation of concerns**: `computeEMA` only calculates the data;
  `printResults` handles display. This makes `computeEMA` reusable/testable
  independent of any console output.
- Used `std::vector<double>` for flexibility with any dataset size.

## How the Solution Was Tested
- Compiled with `g++ -std=c++17 -o ema ema.cpp` and run directly.
- Verified manually against known EMA values for a standard 5-day EMA
  example dataset (closing prices), cross-checked against the textbook
  EMA formula.
- Tested edge cases:
  - Period larger than the dataset size → prints an error, returns a
    vector of zeros instead of crashing.
  - Period of exactly 1 → EMA equals the raw price series.

## Working Example

**Input** (hardcoded in `main()`, period = 5):
```
22.27, 22.19, 22.08, 22.17, 22.18, 22.13, 22.23, 22.43, 22.24, 22.29,
22.15, 22.39, 22.38, 22.61, 23.36
```

**Output:**
```
Index | Price   | EMA(5)
------|---------|--------
    0 |   22.27 |   --
    1 |   22.19 |   --
    2 |   22.08 |   --
    3 |   22.17 |   --
    4 |   22.18 | 22.18
    5 |   22.13 | 22.16
    6 |   22.23 | 22.18
    7 |   22.43 | 22.27
    8 |   22.24 | 22.26
    9 |   22.29 | 22.27
   10 |   22.15 | 22.23
   11 |   22.39 | 22.28
   12 |   22.38 | 22.32
   13 |   22.61 | 22.41
   14 |   23.36 | 22.73
```

## How to Run
```bash
g++ -std=c++17 -o ema ema.cpp
./ema
```
