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
//void jacobi(vector<vector<float>> A, vector<vector<float>> X, int rows, int columns, float precision);
void Jacobi(long double** A, long double* F, long double* X, int rows, int cols, double eps)
{
	long double* TempX = new long double[rows];
	long double norm; // норма, определяемая как наибольшая разность компонент столбца иксов соседних итераций.

	do {
		for (int i = 0; i < rows; i++) {
			TempX[i] = F[i];
			for (int g = 0; g < cols; g++) {
				if (i != g)
					TempX[i] -= A[i][g] * X[g];
			}
			TempX[i] /= A[i][i];
		}
		norm = fabs(X[0] - TempX[0]);
		for (int h = 0; h < rows; h++) {
			if (fabs(X[h] - TempX[h]) > norm)
				norm = fabs(X[h] - TempX[h]);
			X[h] = TempX[h];
		}
	} while (norm > eps);

	/*for (int i = 0; i < N; i++) {
		cout << "Temp [" << i << "] = " << TempX[i] << endl;
	}*/
	delete[] TempX;
}

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
			
			long double** A;
			long double* F;
			long double* X;
			int rows, cols;
			utils::load(inputMatrixFile, A, F, inputCoefsFile, X, rows,cols);
			/*for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					cout << "A[" << i << "]["<<j<<"] = " << A[i][j] << "   ";
				}
				cout << endl;
			}
			for (int j = 0; j < 8; j++) {
				cout << "F[" << j << "] = " << F[j] << "  " << endl;
			}
			for (int j = 0; j < 8; j++) {
				cout << "X[" << j << "] = " << X[j] << " " << endl;
			}*/
			int N = rows;
			cout << N;
			for (int i = 0; i < rows; i++) {
				cout << fixed<<"Pupa[" << i << "]: " << A[i][rows] << endl;
			}
			cout << "popy Mb|L?" << endl;
			Jacobi(A, F, X, rows,cols, precision);
			for (int j = 0; j < N; j++) {
				cout << "X[" << j << "] = " << X[j] << "  " << endl;
			}
			utils::answer(outputMatrixFile,X,N);
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
	/*cout << "Calculate root" << endl;
	vector<vector<float>> matrix = utils::load(matFile);
	vector<float> coefs = utils::loadCoef(coefFile);
	cout << "Calculate root" << endl;

	/*jacobi(matrix, coefs, matrix.size(), matrix[0].size(), precision);
	utils::answer(outFile, coefs);*/
	
	/*
	vector<vector<int>> matrix1 = utils::load(file1);
	vector<vector<int>> matrix2 = utils::load(file2);
	int chunksSize = ((argv[4] != nullptr) ? atoi(argv[4]) : 800 * 1000 / maxThreadNum);
	float time = clock();
	vector<vector<int>> answer;
	std::cout << ((float)time) / CLOCKS_PER_SEC << std::endl;
	utils::answer(answer); */
}

