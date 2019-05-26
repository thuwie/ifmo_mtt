#ifndef LAB2_UTILS_HPP
#define LAB2_UTILS_HPP

#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <tuple>
#include <vector>
#include <sstream>

/*
inputfile
m(rows) n(columns)
11 12 13 14 ... b1
21 22 23 24 ... b2
31 32 33 34 ... b3
__
m
x1
x2
x3
__
e
__
outfile
*/

namespace utils {
void distributeRows(
	int worldSize, int rows, int cols, int* matrixRows, int* matrixOffset,
	int* answersRows, int* answersOffset) {
	int commonRows = rows / worldSize;
	int hangRows = rows % worldSize;

	matrixRows[0] = commonRows * cols;
	matrixOffset[0] = 0;
	answersRows[0] = commonRows;
	answersOffset[0] = 0;
	for (int i = 0; i < worldSize; i++) {
		matrixRows[i] = commonRows * cols;
		answersRows[i] = commonRows;

		if (hangRows != 0) {
			matrixRows[i] += cols;
			answersRows[i] += 1;
			hangRows -= 1;
		}
	}
	for (int i = 1; i < worldSize; i++) {
		matrixOffset[i] = matrixRows[i - 1] + (matrixOffset[i - 1]);
		answersOffset[i] = answersRows[i - 1] + answersOffset[i - 1];
	}
}

void distribute(int m, int worldSize, int* sendcountsF, int* sendcountsA, int* displsF, int* displsA) {
	int remainder = m % worldSize;
	int sum = 0;
	for (int i = 0; i < worldSize; i++) {
		sendcountsF[i] = m / worldSize;
		if (remainder > 0) {
			sendcountsF[i]++;
			remainder--;
		}
		sendcountsA[i] = sendcountsF[i] * m;
		displsF[i] = sum;
		displsA[i] = sum * m;
		sum += sendcountsF[i];
	}
}

std::tuple<double*, double*, double*, int, int> load(
	const std::string& matrixroute, const std::string& coefroute) {
	int row, col, t1, t2;
	std::ifstream file(matrixroute);
	std::ifstream fileCoef(coefroute);
	if (!file || !fileCoef) {
		std::cerr << "Error opening matrix file.\n";
		throw std::runtime_error("Could not open file");
	}
	file >> row >> col;
	fileCoef >> t1 >> t2;
	if (row < 1 || col < 1) {
		std::cerr << "depr sizes are out of bounds.\n";
		throw std::runtime_error("Matrix size is invalid.");
	}

	double* matrix = new double[row * (col - 1)];
	double* answ = new double[row];
	double* coef = new double[row];
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col - 1; j++) {
			file >> matrix[i * (col - 1) + j];
		}
		file >> answ[i];
		fileCoef >> coef[i];
	}

	return std::make_tuple(matrix, answ, coef, row, col);
}

int get_random() {
    static std::default_random_engine e;
    static std::uniform_int_distribution<> dis(1, 10); // rage 0 - 1
    return dis(e);
}

void generateMatrix(const std::string& matrixroute, int row, int col) {
    srand(time(NULL));
    std::ofstream fout(matrixroute, std::ios_base::trunc);
    ;
    fout << row << " " << col + 1 << std::endl;
    std::vector<std::vector<float>> matrix;
    matrix.resize(row);
    for(auto& outer : matrix)
        outer.resize(col + 1);

    float sum = 0;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col + 1; j++) {
            float randNum = utils::get_random();
            matrix[i][j] = (randNum);
            sum += randNum;
        }
        matrix[i][i] = sum + 1;
        sum = 0;
    }

    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col + 1; j++) {
            fout << matrix[i][j] << " ";
        }
        fout << std::endl;
    }
    fout.close();
}

void generateCoefs(const std::string& matrixroute, int row) {
    std::ofstream fout(matrixroute, std::ios_base::trunc);
    fout << row << " "
         << "1" << std::endl;
    for(int i = 0; i < row; i++) {
        int right = 1;
        int left = (rand() % 6);
        std::string num =
            "" + std::to_string(right) + "." + std::to_string(left);
        float endNum = std::stof(num);
        fout << 1.0 << " ";
        fout << std::endl;
    }
    fout.close();
}

void answer(const std::string& outFile, double* matrix, int n) {
    std::ofstream fout(outFile);
    for(int i = 0; i < n; i++) {
        fout << matrix[i] << std::endl;
    }
    fout.close();
}

void printMatrix(std::vector<std::vector<double>> matrix) {
    for(int i = 0; i < matrix.size(); i++) {
        for(int j = 0; j < matrix.size(); j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void printSomethingDouble(double* mat, int size, const std::string& name) {
    for(int j = 0; j < size; j++) {
        std::cout << name << "[" << j << "] = " << mat[j] << "  " << std::endl;
    }
}

void printSomethingInt(int* mat, int size, const std::string& name) {
    for(int j = 0; j < size; j++) {
        std::cout << name << "[" << j << "] = " << mat[j] << "  " << std::endl;
    }
}
} 

#endif // LAB2_UTILS_HPP
