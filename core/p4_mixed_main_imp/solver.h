#pragma once
#include <vector>
#include <string>
#include "../shared/grid.h"

struct BVP_Result {
    grid::UniformGrid grid;
    std::vector<double> u;

    // Параметры задачи для фронтенда
    double xi_jump;
    double mu1;
    double mu2;
    std::string bc_left;
    std::string bc_right;

    BVP_Result() : grid(2), u(), xi_jump(0.0), mu1(0.0), mu2(0.0),
                   bc_left("unknown"), bc_right("unknown") {}

    BVP_Result(const grid::UniformGrid& g, const std::vector<double>& u_vec,
               double xi, double m1, double m2,
               const std::string& bcl, const std::string& bcr)
        : grid(g), u(u_vec), xi_jump(xi), mu1(m1), mu2(m2),
          bc_left(bcl), bc_right(bcr) {}
};

BVP_Result solve_mixed_main_imp(int n);