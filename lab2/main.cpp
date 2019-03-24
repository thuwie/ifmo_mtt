#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <ctime>
#include "utils.hpp"

#include <mpi.h>

using namespace std;

void generateRoot(const string &matFile, const string &coefFile, int rows, int cols);
void calculateRoot(const string &matFile, const string &coefFile, const string &outFile, float precision);
void jacobi(vector<vector<float>> A, vector<vector<float>> X, int rows, int columns, float precision);

int main(int argc, char* argv[]) {
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// Get the name of the processor
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	// Print off a hello world message
	printf("Hello world from processor %s, rank %d out of %d processors\n",
		processor_name, world_rank, world_size);

	// Finalize the MPI environment.
	MPI_Finalize();
	if (argc == 6 ) {
		
		int options = atoi(argv[5]); // 0 for the generator root, 1 for the calculation root

		if (options == 0) {
			// Generate root

			string inputMatrixFile = argv[1];
			string inputCoefsFile = argv[2];
			int rows = atoi(argv[3]);
			int cols = atoi(argv[4]);
			generateRoot(inputMatrixFile, inputCoefsFile, rows, cols);
			//getchar();
			return 0;
		}
		else {
			// Calculate root

			string inputMatrixFile = argv[1];
			string inputCoefsFile = argv[2];
			string outputMatrixFile = argv[3];
			float precision = std::atof(argv[4]);
			calculateRoot(inputMatrixFile, inputCoefsFile, outputMatrixFile, precision);
			// cout << "boi oh boi";
			//getchar();
			return 0;
		}
	}
	else {
		// Corrupted arguments
		cout << "Corrupted arguments";
		//getchar();
		return 1;
	}    
    return 0;
}

void generateRoot(const string &matFile, const string &coefFile, int rows, int cols) {
	cout << "Generate root" << endl;
	utils::generateMatrix(matFile, rows, cols);
	utils::generateCoefs(coefFile, rows);
	// generateCoefs();
}

void calculateRoot(const string &matFile, const string &coefFile, const string &outFile, float precision) {
	cout << "Calculate root" << endl;
	vector<vector<float>> matrix = utils::load(matFile);
	vector<vector<float>> coefs = utils::load(coefFile);
	cout << "Calculate root" << endl;
	jacobi(matrix, coefs, matrix.size(), matrix[0].size(), precision);
	utils::answer(outFile, coefs);

	/*
	vector<vector<int>> matrix1 = utils::load(file1);
	vector<vector<int>> matrix2 = utils::load(file2);
	int chunksSize = ((argv[4] != nullptr) ? atoi(argv[4]) : 800 * 1000 / maxThreadNum);
	float time = clock();
	vector<vector<int>> answer;
	std::cout << ((float)time) / CLOCKS_PER_SEC << std::endl;
	utils::answer(answer); */
}

// rows,columns - размерность матрицы; vector<vector<int>> matrix - матрица коэффициентов,
// coefs[rows] - начальное приближение, ответ записывается также в coefs[rows];
/*void wjacobi(vector<vector<float>> matrix, vector<vector<float>> coefs, int rows, int columns, float precision)
{
	// double* TempX = new double[N];
	vector<float> TempX;
	TempX.resize(rows);
	double norm; // норма, определяемая как наибольшая разность компонент столбца иксов соседних итераций.

	do {
		for (int i = 0; i < rows; i++) {
			TempX[i] = matrix[i][columns - 1];
			for (int g = 0; g < columns-1; g++) {
				if (i != g)
					TempX[i] -= matrix[i][g] * coefs[g][0];
			}
			TempX[i] /= matrix[i][i];
		}
		norm = fabs(coefs[0][0] - TempX[0]);
		for (int h = 0; h < rows; h++) {
			if (fabs(coefs[h][0] - TempX[h]) > norm)
				norm = fabs(coefs[h][0] - TempX[h]);
			coefs[h][0] = TempX[h];
		}
		printf("%20.10f\n", norm);
		//cout << "Norm: " << norm << " Precision: " << precision << endl;
	} while (norm > precision);
	// delete[]7 TempX;
}*/

void jacobi(vector<vector<float>> matrix, vector<vector<float>> coefs, int rows, int columns, float precision) {
	cout << "debux" << endl;
	vector<vector<float>> tempVector;
	tempVector = coefs;
	cout << "debux" << endl;
	bool flag = false; //OutputDebugStringW(L"My output string.");
	//System.Diagnostics.Debug.WriteLine("your message here");
	while (flag == false) {
		cout << "debux" << endl;
		for (int i = 0; i < rows; i++) {
			float tempSum = 0;
			for (int j = 0; i < columns - 1; j++) {
				if (i != j) {
					tempSum += matrix[i][j] * tempVector[i][0];
				}
			}
			tempSum += matrix[1][columns];
			cout << tempSum << endl;
		}
		flag = true;
	}
	getchar();
}
