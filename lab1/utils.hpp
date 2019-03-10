//
// Created by Kirill on 10.03.2019.
//

#ifndef LAB1_UTILS_HPP
#define LAB1_UTILS_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

namespace utils {
    void generateMatrix(const std::string &matrixroute) {
        srand(time(NULL));
        std::ofstream fout(matrixroute, std::ios_base::trunc);
        const int row = 1000, col = 1000;
        fout << row << " " << col << std::endl;
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                int r2 = (rand() % 1000) - 500;
                fout << r2 << " ";
            }
            fout << std::endl;
        }
        fout.close();
    }

    void answer(std::vector<std::vector<double>> matrix) {
        std::ofstream fout("answer.txt", std::ios_base::trunc);
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix.size(); j++) {
                fout << matrix[i][j] << " ";
            }
            fout << std::endl;
        }
        fout.close();
    }

    void printMatrix(std::vector<std::vector<double>> matrix) {
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix.size(); j++) {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    std::vector<std::vector<double>> load(const std::string &matrixroute) {
        std::vector<std::vector<double>> matrix;
        int row, col;
        std::ifstream file(matrixroute);
        if (!file) {
            std::cerr << "Error opening matrix file.\n";
            return matrix;
        }

        file >> row >> col;
        if (row < 1 || col < 1) {
            std::cerr << "Matrix sizes are out of bounds.\n";
            return matrix;
        }

        // Size the vector using the values read from the file.
        matrix.resize(row);
        for (auto &m : matrix)
            m.resize(col);

        // Read the input file.
        for (auto &outer : matrix)
            for (auto &inner : outer)
                file >> inner;

        return matrix;
    }
}

#endif //LAB1_UTILS_HPP
