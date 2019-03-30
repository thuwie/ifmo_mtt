#ifndef LAB2_UTILS_HPP
#define LAB2_UTILS_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>

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
	void generateMatrix(const std::string &matrixroute, int row, int col) {
		srand(time(NULL));
		std::ofstream fout(matrixroute, std::ios_base::trunc);;
		fout << row << " " << col << std::endl;
		float sum = 0;
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col - 1; j++) {
				int right = (rand() % 20);
				int left = (rand() % 10);
				std::string num = "" + std::to_string(right) + "." + std::to_string(left);
				float endNum = std::stof(num);
				fout << endNum << " ";
				sum += endNum;
			}
			fout << sum << std::endl;
			sum = 0;
		}
		fout.close();
	}

	void generateCoefs(const std::string &matrixroute, int row) {
		//srand(time(NULL));
		std::ofstream fout(matrixroute, std::ios_base::trunc);;
		fout << row << " " << "1" << std::endl;
		for (int i = 0; i < row; i++) {
			int r2 = 1;//(rand() % 1000) - 500;
			fout << r2 << " ";
			fout << std::endl;
		}
		fout.close();
	}

	void answer(const std::string &outFile, std::vector<std::vector<float>> matrix) {
		std::ofstream fout(outFile, std::ios_base::trunc);
		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix[i].size(); j++) {
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

	std::vector<std::vector<float>> load(const std::string &matrixroute) {
		std::vector<std::vector<float>> matrix;
		int row, col;
		std::ifstream file(matrixroute);
		if (!file) {
			std::cerr << "Error opening matrix file.\n";
			return matrix;
		}

		file >> row >> col;
		if (row < 1 || col < 1) {
			std::cerr << "depr sizes are out of bounds.\n";
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

#endif //LAB2_UTILS_HPP