#define NOMINMAX             // Отключает макросы min/max от Windows, чтобы не ломать std::max в solver.h
#define _USE_MATH_DEFINES    // Гарантирует доступность математических констант
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>             // Для std::log10
#include <windows.h>         // Для SetConsoleOutputCP
#include <mpi.h>
#include "solver.h"

int main(int argc, char** argv) {
    // Принудительно устанавливаем кодировку UTF-8 для вывода в консоль Windows
    ::SetConsoleOutputCP(65001);

    // Инициализация среды MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Выводим шапку таблицы только на главном процессе (rank 0)
    if (rank == 0) {
        std::cout << "=================== АНАЛИЗ ПОГРЕШНОСТИ ТЕСТОВОЙ ЗАДАЧИ ===================" << std::endl;
        std::cout << "Количество параллельных процессов (MPI): " << size << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
        std::cout << std::left << std::setw(10) << "n" 
                  << std::setw(25) << "max |u - v| (eps1)" 
                  << std::setw(18) << "-lg(eps1)" << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
    }

    // Список сеток из методички
    std::vector<int> grids = {10, 20, 50, 80, 100, 200, 500, 800, 1000, 10000, 100000, 1000000};

    // Включаем общий секундомер перед началом всех вычислений
    MPI_Barrier(MPI_COMM_WORLD);
    double global_start_time = MPI_Wtime();

    for (int current_n : grids) {
        // Создаем объект решателя для текущей сетки
        BalanceSolver solver(current_n);

        // Основной расчет
        std::vector<double> v = solver.solve();

        // Главный процесс собирает погрешность, считает логарифм и выводит строку таблицы
        if (rank == 0) {
            double max_err = solver.max_error(v);
            double lg_eps = -std::log10(max_err);

            std::cout << std::left << std::setw(10) << current_n 
                      << std::scientific << std::setw(25) << max_err 
                      << std::fixed << std::setprecision(4) << std::setw(18) << lg_eps
                      << std::defaultfloat << std::endl;
            
            if (current_n == 200) {
                std::cout << " >>> ДОСТИГНУТА ТРЕБУЕМАЯ ПОГРЕШНОСТЬ ПРИ n = 200 <<<" << std::endl;
            }
        }
    }

    // Выключаем секундомер после завершения всех циклов
    MPI_Barrier(MPI_COMM_WORLD);
    double global_end_time = MPI_Wtime();

    double local_total_elapsed = global_end_time - global_start_time;
    double max_total_elapsed = 0.0;

    // Находим максимальное суммарное время среди всех процессов
    MPI_Reduce(&local_total_elapsed, &max_total_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // Выводим итог по времени в самом конце
    if (rank == 0) {
        std::cout << "(" << max_total_elapsed << " s)" << std::endl;
    }

    MPI_Finalize();
    return 0;
}