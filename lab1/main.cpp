#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <omp.h>
#include "utils.hpp"

int main(int argc, char** argv) {
    if( argc != 3 || !argv[1] || !argv[2]) {
        std::cout << "Corrupted arguments!" << std::endl;
        return 1;
    }
    std::string file1(argv[1]);
    std::string file2(argv[2]);

//    utils::generateMatrix(file1);
//    utils::generateMatrix(file2);

    std::vector<std::vector<double>> matrix1 = utils::load(file1);
    std::vector<std::vector<double>> matrix2 = utils::load(file2);

    float time = clock();
#pragma omp parallel for //private(i) shared(matrix1, matrix2)
    for (int i = 0; i < matrix1.size(); i++) {
        for (int j = 0; j < matrix1.size(); j++) {
            matrix1[i][j] *= matrix2[i][j];
        }
    }

    time = clock() - time;
    std::cout << "time: " << ((float)time)/CLOCKS_PER_SEC << std::endl;
    utils::answer(matrix1);
    return 0;
}

