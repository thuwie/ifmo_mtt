#ifndef LAB2_UTILS_HPP
#define LAB2_UTILS_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <random>
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
	float get_random()
	{
		static std::default_random_engine e;
		static std::uniform_int_distribution<> dis(1, 10); // rage 0 - 1
		return dis(e);
	}
	void generateMatrix(const std::string &matrixroute, int row, int col) {
		srand(time(NULL));
		std::ofstream fout(matrixroute, std::ios_base::trunc);;
		fout << row << " " << col+1 << std::endl;
		std::vector<std::vector<float>> matrix;
		matrix.resize(row);
		for (auto &outer : matrix)
			outer.resize(col+1);

		float sum = 0;
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col+1; j++) {
				float randNum = utils::get_random();
				matrix[i][j] = (randNum);
				sum += randNum;
			}
			matrix[i][i] = sum+1;
			sum = 0;
		}

		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col+1; j++) {
				fout << matrix[i][j] << " ";
			}
			fout << std::endl;
		}
		fout.close();
	}
	
	void generateCoefs(const std::string &matrixroute, int row) {
		//srand(time(NULL));
		std::ofstream fout(matrixroute, std::ios_base::trunc);;
		fout << row << " " << "1" << std::endl;
		for (int i = 0; i < row; i++) {
			int right = 1;//(rand() % 1000) - 500;
			int left = (rand() % 6);
			std::string num = "" + std::to_string(right) + "." + std::to_string(left);
			float endNum = std::stof(num);
			fout << 1.0 << " ";
			fout << std::endl;
		}
		fout.close();
	}

	/*void answer(const std::string &outFile, std::vector<std::vector<float>> matrix) {
		std::ofstream fout(outFile, std::ios_base::trunc);
		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix[i].size(); j++) {
				fout << matrix[i][j] << " ";
			}
			fout << std::endl;
		}
		fout.close();
	}*/
	void answer(const std::string &outFile, long double* &matrix, int n) {
		std::ofstream fout(outFile, std::ios_base::trunc);
		for (int i = 0; i < n; i++) {
		    fout << matrix[i] << std::endl;
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

	void load(const std::string &matrixroute, long double** &matrix, long double* &answ, const std::string &coefroute, long double* &coef, int &rows, int &cols) {
		int row, col, t1,t2;
		std::ifstream file(matrixroute);
		std::ifstream fileCoef(coefroute);
		if (!file || !fileCoef) {
			std::cerr << "Error opening matrix file.\n";
			return;
		}
		file >> row >> col;
		fileCoef >> t1 >> t2;
		if (row < 1 || col < 1) {
			std::cerr << "depr sizes are out of bounds.\n";
			return;
		}
		rows = row;
		cols = col;

		matrix = new long double *[row];
		answ = new long double[row];
		coef = new long double[row];
		for (int i = 0; i < row; i++) {
			matrix[i] = new long double[col - 1];
			
			for (int j = 0; j < col - 1; j++) {
				file >> matrix[i][j];
			}
			file >> answ[i];
			fileCoef >> coef[i];
		}


		/*
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

		return matrix;*/
	}

	std::vector<float> loadCoef(const std::string &matrixroute) {
		std::vector<float> matrix;
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

		// Read the input file.
		for (auto &outer : matrix)
				file >> outer;

		return matrix;
	}
}

#endif //LAB2_UTILS_HPP
