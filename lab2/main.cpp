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

int main(int argc, char* argv[]) {
	int world_size, world_rank;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	int rows, cols;
	double* matrix;
	double* answer;
	double* coeffs;
	double* final_coeffs = nullptr;
	double *matrix_part, *answer_part;
	int* matrix_element = new int[world_size];
	int* matrix_offset = new int[world_size];
	int* answer_element = new int[world_size];
	int* answer_offset = new int[world_size]; 
	int data_transfer[3];
	std::chrono::time_point<std::chrono::steady_clock> time_start, time_end;

	if (argc != 5)
		return -1;

	std::string input_matrix_file = argv[1];
	std::string input_coeffs_file = argv[2];
	std::string output_matrix_file = argv[3];
	double precision = std::atof(argv[4]);

	if (0 == world_rank) {
		// auto data = utils::read(inputMatrixFile);
		std::tie(matrix, answer, coeffs, rows, cols) = utils::load(input_matrix_file, input_coeffs_file);
		data_transfer[0] = 0;
		data_transfer[1] = rows;
		data_transfer[2] = cols - 1;
	}
	else {
		matrix = new double[world_rank];
		answer = new double[world_rank];
	}

	MPI_Bcast(&data_transfer, 3, MPI_INT, 0, MPI_COMM_WORLD);

	if (0 != world_rank)
		coeffs = new double[data_transfer[1]];

	MPI_Bcast(coeffs, data_transfer[1], MPI_DOUBLE, 0, MPI_COMM_WORLD);

	utils::distributeRows(
		world_size, data_transfer[1], data_transfer[2], matrix_element, matrix_offset,
		answer_element, answer_offset);

	matrix_part = (double*)malloc(sizeof(double) * matrix_element[world_rank]);
	answer_part = (double*)malloc(sizeof(double) * answer_element[world_rank]);

	MPI_Scatterv(
		matrix, matrix_element, matrix_offset, MPI_DOUBLE, matrix_part,
		matrix_element[world_rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Scatterv(
		answer, answer_element, answer_offset, MPI_DOUBLE, answer_part,
		answer_element[world_rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if (0 == world_rank)
		time_start = std::chrono::high_resolution_clock::now();

	final_coeffs = parallel::jacobi(
		answer_element[world_rank], data_transfer[2], matrix_part, answer_part, coeffs, precision,
		 world_rank, answer_element, answer_offset);

	if (0 == world_rank)
		time_end = std::chrono::high_resolution_clock::now();

	if (0 == world_rank) {
		auto duration = time_end - time_start;
		auto milliseconds =
			std::chrono::duration_cast<std::chrono::milliseconds>(duration)
			.count();
		std::cout << milliseconds << "ms";
		utils::answer(output_matrix_file, final_coeffs, data_transfer[1]);
	}

	MPI_Finalize();

	return 0;
}
