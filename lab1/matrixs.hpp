#ifndef LAB1_MATRIXS_HPP
#define LAB1_MATRIXS_HPP

#include <iostream>
#include <omp.h>
#include <vector>

namespace matrixs {
    std::vector<std::vector<int>> commonMultiply(std::vector<std::vector<int>> &a, std::vector<std::vector<int>> &b) {
        int rows = a.size();
        int columns = b[0].size();
        std::vector<std::vector<int>> result(rows, std::vector<int>(columns));
//#pragma omp parallel for
        for (int step = 0; step < rows * columns; step++) {
            int i = step / columns;
            int j = step % columns;
            int sum = 0;
            for (int inner = 0; inner < columns; inner++) {
                sum += a[i][inner] * b[inner][j];
            }
            result[i][j] = sum;
        }
        return result;
    }

    std::vector<std::vector<int>> parallelMultiply(std::vector<std::vector<int>> &a, std::vector<std::vector<int>> &b, int chunkSize) {
        int rows = a.size();
        int columns = b[0].size();
        std::vector<std::vector<int>> result(rows, std::vector<int>(columns));
        if (rows == 1 && columns == 1) {
            double sum = 0;
#pragma omp parallel for reduction(+:sum) schedule(static, chunkSize)
            for (int inner = 0; inner < a[0].size(); inner++) {
                sum += a[0][inner] * b[inner][0];
            }
            result[0][0] = sum;
        }
        else {
#pragma omp parallel for schedule(static, chunkSize)
            for (int step = 0; step < rows * columns; step++) {
                int i = step / columns;
                int j = step % columns;
                int sum = 0;
                for (int inner = 0; inner < columns; inner++) {
                    sum += a[i][inner] * b[inner][j];
                }
                result[i][j] = sum;
            }
        }
        return result;
    }

    std::vector<std::vector<int>> dynamicScheduleMultiply(std::vector<std::vector<int>> &a, std::vector<std::vector<int>> &b, int chunkSize) {
        int rows = a.size();
        int columns = b[0].size();
        std::vector<std::vector<int>> result(rows, std::vector<int>(columns));
#pragma omp parallel for schedule(dynamic, chunkSize)
        for (int step = 0; step < rows * columns; step++) {
            int i = step / columns;
            int j = step % columns;
            int sum = 0;
            for (int inner = 0; inner < columns; inner++) {
                sum += a[i][inner] * b[inner][j];
            }
            result[i][j] = sum;
        }
        return result;
    }
    std::vector<std::vector<int>> guidedScheduleMultiply(std::vector<std::vector<int>> &a, std::vector<std::vector<int>> &b, int chunkSize) {
        int rows = a.size();
        int columns = b[0].size();
        std::vector<std::vector<int>> result(rows, std::vector<int>(columns));
#pragma omp parallel for schedule(guided, chunkSize)
        for (int step = 0; step < rows * columns; step++) {
            int i = step / columns;
            int j = step % columns;
            int sum = 0;
            for (int inner = 0; inner < columns; inner++) {
                sum += a[i][inner] * b[inner][j];
            }
            result[i][j] = sum;
        }
        return result;
    }
}

#endif //LAB1_MATRIXS_HPP
