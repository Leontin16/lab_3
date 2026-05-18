#include <iostream>
#include <iomanip>
#include <cmath>
#include "solver.h"
#include "../shared/grid.h"

int main() {
    int n = 10; // Стартовое число узлов
    double target_eps = 0.5e-6;
    double current_eps = 1.0;
    
    BVP_Result res_n, res_2n;
    int final_n = n;

    std::cout << "Refining grid...\n";

    // Сгущение сетки по правилу Рунге
    while (current_eps > target_eps) {
        res_n = solve_mixed_main_imp(n);
        res_2n = solve_mixed_main_imp(2 * n);

        auto [mx, idx] = grid::max_norm_coarse(res_n.u, res_2n.u);
        current_eps = mx;
        final_n = n;

        if (current_eps > target_eps) {
            n *= 2;
        }
        
        // Предохранитель от бесконечного цикла
        if (n > 1000000) { 
            std::cout << "Refinement halted: max grid size reached.\n";
            break;
        }
    }

    auto [mx_diff, max_idx] = grid::max_norm_coarse(res_n.u, res_2n.u);

    // Вывод результатов строго по формату методички
    std::cout << "\n--- Справка ---\n";
    std::cout << "Для решения задачи использована равномерная сетка с числом разбиений n = " << final_n << ";\n";
    std::cout << "задача должна быть решена с заданной точностью eps = 0.5*10^-6;\n";
    std::cout << "задача решена с точностью eps2 = " << std::scientific << current_eps << ";\n";
    std::cout << "максимальная разность численных решений в общих узлах сетки наблюдается в точке x = " 
              << std::fixed << std::setprecision(6) << res_n.grid.x[max_idx] << "\n\n";

    std::cout << "--- Таблица 3 (Выборочно) ---\n";
    std::cout << std::setw(10) << "№ узла" 
              << std::setw(15) << "x_i" 
              << std::setw(15) << "v(x_i)" 
              << std::setw(15) << "v2(x_{2i})" 
              << std::setw(20) << "Разность" << "\n";
              
    int step = std::max(1, final_n / 10); // Чтобы не засорять консоль, выводим 10 точек
    for (int i = 0; i <= final_n; i += step) {
        double diff = res_n.u[i] - res_2n.u[2 * i];
        std::cout << std::setw(10) << i 
                  << std::setw(15) << std::fixed << std::setprecision(6) << res_n.grid.x[i]
                  << std::setw(15) << std::scientific << res_n.u[i]
                  << std::setw(15) << res_2n.u[2 * i]
                  << std::setw(20) << diff << "\n";
    }

    return 0;
}