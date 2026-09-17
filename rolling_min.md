# Rolling Minimum — Documentation

**Group 10 — Rolling Operations**
**Operation covered:** `rolling_min()`
**Language:** C++
**Reference:** [Polars DataFrame API](https://docs.pola.rs/api/python/stable/reference/dataframe/index.html)

---

## 1. OOP Concepts Used

The solution models a small DataFrame-like structure in C++ and re-implements the `rolling_min()` operation on top of it. The object-oriented concepts applied are:

### 1.1 Encapsulation
The `DataFrame` class bundles together its internal state (a `std::vector<Column>`) and the behaviour that operates on it (`column()`, `add_column()`, `columns()`). The internal vector is kept private — external code cannot touch it directly, and any lookup goes through the `column()` method, which validates the name and throws if it is missing. The `Column` struct also encapsulates its own data (`name` and `values`) as a single unit.

### 1.2 Abstraction
A user calling `rolling_min_column(df, "a", 3)` does not need to know how the window is sliced, how nulls are skipped, or how `min_periods` gates the result. The method presents one coherent, high-level operation, hiding the loop and the optional-value bookkeeping. The `std::optional<double>` type itself is an abstraction: it hides the sentinel mechanism used to represent a missing value.

### 1.3 Composition
A `DataFrame` *has-a* collection of `Column` objects. Rather than inheriting from a column, the frame is composed of them — the correct choice whenever the relationship is "part-of" rather than "is-a". This mirrors how real data engines (Polars, Pandas) store a frame as a set of parallel columns.

### 1.4 Value Objects
`Column` is a lightweight value object: it carries a name and a list of nullable doubles, with no complex behaviour of its own. It can be copied, moved, and compared by identity of its contents, and it serves as the currency passed between `DataFrame` and the rolling functions.

### 1.5 Separation of Concerns
The rolling logic is deliberately kept as a *free function* (`rolling_min`) that operates purely on a `std::vector<std::optional<double>>`. This is a small but important OOP principle: pure computation is decoupled from the object that stores the data, so it can be reused, tested, and reasoned about in isolation. The `DataFrame` method `rolling_min_column` is a thin adapter that wires the free function to the object graph.

---

## 2. Algorithm

### 2.1 Problem Statement
Given a column of nullable doubles `V[0..n-1]`, a window size `k ≥ 1`, and a `min_periods` threshold `m` (with `1 ≤ m ≤ k`), produce a new column `R[0..n-1]` where `R[i]` is the minimum of the non-null values in the window `V[max(0, i-k+1) .. i]`, or `null` if fewer than `m` non-null values are present.


### 2.3 Worked Example

Input column: `[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]`, window `k = 3`, `min_periods = 1`.

| i | Window slice | Non-null values | Min | Result |
|---|---|---|---|---|
| 0 | `[1]`         | {1}    | 1 | 1 |
| 1 | `[1,2]`       | {1,2}  | 1 | 1 |
| 2 | `[1,2,3]`     | {1,2,3}| 1 | 1 |
| 3 | `[2,3,4]`     | {2,3,4}| 2 | 2 |
| 4 | `[3,4,5]`     | {3,4,5}| 3 | 3 |
| … | …             | …      | … | … |
| 9 | `[8,9,10]`    | {8,9,10}| 8 | 8 |

Result: `[1, 1, 1, 2, 3, 4, 5, 6, 7, 8]`.

### 2.4 Null Handling Example

Input column: `[1, null, 3, null, 5]`, window `k = 3`, `min_periods = 1`.

| i | Window slice     | Non-null values | Min | Result |
|---|---|---|---|---|
| 0 | `[1]`             | {1}    | 1 | 1 |
| 1 | `[1, null]`       | {1}    | 1 | 1 |
| 2 | `[1, null, 3]`    | {1,3}  | 1 | 1 |
| 3 | `[null, 3, null]` | {3}    | 3 | 3 |
| 4 | `[3, null, 5]`    | {3,5}  | 3 | 3 |

Result: `[1, 1, 1, 3, 3]`.

### 2.5 Complexity
- **Time:** `O(n · k)` in the direct form. Each of the `n` output positions scans up to `k` inputs.
- **Space:** `O(n)` for the output column.
- **Optimisation note:** A monotonic deque reduces the time to `O(n)` by maintaining the running minimum as the window slides. This is not implemented here because it obscures the logic; the straightforward version is preferred for readability and matches Polars' *semantics*, not its internal implementation.

---

## 3. Possible Error Points

1. **Invalid window size**
   - `window == 0` must be rejected with an exception. A zero-length window is undefined and would cause either an infinite loop or an empty result.

2. **Invalid `min_periods`**
   - `min_periods == 0` must be rejected.
   - `min_periods > window` is logically impossible — a window cannot contain more values than its size — and must also be rejected.

3. **Off-by-one errors in window slicing**
   - The window must be `V[i-k+1 .. i]` (inclusive at both ends). Using `i-k` as the start, or `i` as the end-exclusive bound, shifts every result by one position. This is the single most common bug in rolling implementations.

4. **Null vs NaN confusion**
   - Polars distinguishes `null` (missing) from `NaN` (not-a-number). In this implementation `std::optional<double>` is used exclusively for nulls; mixing the two would break the `seen` counter and produce wrong minima.

5. **All-null windows**
   - When every value in a window is null, `seen == 0` and the result must be null. Forgetting to gate on `seen > 0` will emit `+infinity` as the minimum, which is silently wrong.

6. **The `min_periods` gate**
   - If `seen < min_periods`, the result must be null even though the window may contain valid values. Dropping this check produces a shorter warm-up period than requested and diverges from Polars' behaviour.

7. **Empty input**
   - An empty column must produce an empty result, not crash. The loop naturally handles this, but any pre-computation (e.g. computing `n - 1`) must guard against underflow.

8. **Column not found**
   - Requesting a column by an unknown name must raise a clear error (`std::invalid_argument` with the column name) rather than silently returning a default-constructed column.

9. **Integer vs floating-point types**
   - The current implementation operates on `double`. Applying the same logic to an integer column must not truncate during computation; the output type should be promoted to `double` for consistency with `rolling_std` and `moving_average`.

10. **Aliasing between input and output**
    - `rolling_min` must not read from a buffer it is simultaneously writing to. In this implementation the result is a fresh `std::vector`, so aliasing cannot occur — but if the function were rewritten to write in place, this would become a real hazard.

11. **`min_periods` semantics for `rolling_min` vs `rolling_std`**
    - For `rolling_min` the default `min_periods = 1` is sensible. For `rolling_std` the equivalent default would be 2 (standard deviation of a single point is undefined). Applying the wrong default uniformly across operations is a subtle but common mistake when refactoring.

12. **Large inputs and floating-point stability**
    - The direct scan version is numerically stable but slow. Any switch to a running-sum or running-min approach must be validated against the direct version on random inputs, because the two can disagree at the boundaries of the window.

13. **Negative `window` cast to `size_t`**
    - If the caller passes an `int` window that is negative and it is implicitly converted to `std::size_t`, it becomes an enormous positive number. Validate using a signed type *before* the conversion.

14. **Loop variable types**
    - Mixing `int` and `std::size_t` in `j <= i` or `i + 1 >= window` can cause signed/unsigned comparison warnings and, in pathological cases, wraparound. Use `std::size_t` consistently.
