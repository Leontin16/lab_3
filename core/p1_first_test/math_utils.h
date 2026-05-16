#pragma once
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>

inline std::vector<double> solve_4x4(std::vector<std::vector<double>> A, std::vector<double> B) {
    int n = 4;
    for (int i = 0; i < n; ++i) {
        int max_row = i;
        for (int k = i + 1; k < n; ++k) {
            if (std::abs(A[k][i]) > std::abs(A[max_row][i])) max_row = k;
        }
        std::swap(A[i], A[max_row]);
        std::swap(B[i], B[max_row]);

        for (int k = i + 1; k < n; ++k) {
            double factor = A[k][i] / A[i][i];
            for (int j = i; j < n; ++j) {
                A[k][j] -= factor * A[i][j];
            }
            B[k] -= factor * B[i];
        }
    }
    std::vector<double> x(n);
    for (int i = n - 1; i >= 0; --i) {
        x[i] = B[i];
        for (int j = i + 1; j < n; ++j) {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }
    return x;
}