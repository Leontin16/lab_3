#pragma once
#include <vector>
#include "../shared/grid.h"

struct BVP_Result {
    grid::UniformGrid grid;
    std::vector<double> u;

    // Добавляем конструктор по умолчанию (инициализируем сетку минимальным n = 2)
    BVP_Result() : grid(2), u() {}
    
    // Конструктор для удобного создания объекта
    BVP_Result(const grid::UniformGrid& g, const std::vector<double>& u_vec) : grid(g), u(u_vec) {}
};

// Функция решения задачи для n разбиений
BVP_Result solve_mixed_main_imp(int n);