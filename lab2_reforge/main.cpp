#include <mpi.h>

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

#include "utils.hpp"
#include "jacobi.hpp"

std::pair<double*, double*> readArraysFromFile(
	std::string filename, std::string coefsFilename) {
	std::ifstream fin(filename);
	std::ifstream finC(coefsFilename);
	int m, n, c;
	fin >> m >> n;
	fin >> c;

	double* matrix = new double[n * m + 1];
	double* coefs = new double[n + 1];

	auto loop = n * m + 1;
	for (int i = 1; i < loop; ++i) {
		fin >> matrix[i];
	}
	for (int i = 0; i < n; i++) {
		finC >> coefsFilename[i];
	}
	matrix[0] = m;
	return std::make_pair(matrix, coefs);
}


int main(int argc, char* argv[]) {
	int world_size, id;
	// data buffers
	double* matrix = nullptr;
	double* coefs = nullptr;
	double* my_submatrix = nullptr;
	// ptr buffers
	int* repeater = nullptr;
	int* displs = nullptr;
	// calculated data
	int matrix_row_length = 0;
	int normal_distribution_number = 0;
	int cellar = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	if (0 == id) {
		std::tie(matrix, coefs) = readArraysFromFile("input_data.txt", "input_coefs.txt");
		matrix_row_length = matrix[0];
		++matrix;
	}

	MPI_Bcast(&matrix_row_length, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (0 != id) {
		matrix = new double[matrix_row_length * (matrix_row_length + 1)];
		coefs = new double[matrix_row_length];
	}

	cellar = matrix_row_length % world_size;
	normal_distribution_number = matrix_row_length / world_size;

	auto* matrix_elements_to_send = new int[world_size];
	auto* matrix_elements_displ = new int[world_size];
	
	auto* coefs_elements_to_send = new int[world_size];
	auto* coefs_elements_displ = new int[world_size];

	repeater = new int[world_size];
	int runtime_offset = 0;

	if (id==0) {
		std::cout << "cellar " << cellar << std::endl;
		std::cout << "ndn " << normal_distribution_number << std::endl;
		std::cout << "mrl " << matrix_row_length << std::endl;
	}

	for (int i = 0; i < world_size; i++) {
		repeater[i] = normal_distribution_number;
		if (cellar) {
			normal_distribution_number++;
			cellar--;
		}
		// send everything to main processor...
		matrix_elements_to_send[i] = repeater[i] * matrix_row_length;
		matrix_elements_displ[i] = runtime_offset * matrix_row_length;
		coefs_elements_to_send[i] = repeater[i];
		coefs_elements_displ[i] = runtime_offset;



		runtime_offset += repeater[i];
	}
	
	if (id == 0) {
		utils::printSomethingInt(matrix_elements_to_send, world_size, "me");
		utils::printSomethingInt(matrix_elements_displ, world_size, "md");
		utils::printSomethingInt(coefs_elements_to_send, world_size, "ce");
		utils::printSomethingInt(coefs_elements_displ, world_size, "cd");
	}

	my_submatrix = new double[repeater[id] * matrix_row_length];

	MPI_Scatterv(
		matrix, matrix_elements_to_send, matrix_elements_displ, MPI_DOUBLE,
		my_submatrix, matrix_elements_to_send[id], MPI_DOUBLE, 0,
		MPI_COMM_WORLD);
	MPI_Bcast(coefs, matrix_row_length, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	parallel::new_jacobi();

	MPI_Finalize();

	return 0;
}