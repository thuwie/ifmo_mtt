#ifndef LAB2_JACOBI_HPP
#define LAB2_JACOBI_HPP

#include <mpi.h>
#include <cstdlib>

namespace parallel {
	double* jacobi(
		int row_num, int col_num, double* matrix_part, double* answers_part, double* coeffs, double eps,
		int rank, int* receiveCount, int* displs) {
		std::cout << "JACOBI " << rank<<std::endl;
		auto* temp_coeffs = new double[row_num];
		double norm = 0.0;
		double global_norm = 0.0;

		do {
			for (int i = 0; i < row_num; i++) {
				temp_coeffs[i] = answers_part[i];
				for (int g = 0; g < col_num; g++) {
					if (i + displs[rank] != g) {
						temp_coeffs[i] -= matrix_part[i * col_num + g] * coeffs[g];
					}
				}
				temp_coeffs[i] /= matrix_part[i * col_num + i + displs[rank]];
			}

			norm = fabs(coeffs[displs[rank]] - temp_coeffs[0]);
			for (int h = 0; h < row_num; h++) {
				if (fabs(coeffs[h + displs[rank]] - temp_coeffs[h]) > norm) {
					norm = fabs(coeffs[h + displs[rank]] - temp_coeffs[h]);
				}
				coeffs[h + displs[rank]] = temp_coeffs[h];
			}
			//MPI_Barrier(MPI_COMM_WORLD);
			MPI_Allgatherv(
				temp_coeffs, row_num, MPI_DOUBLE, coeffs, receiveCount, displs, MPI_DOUBLE,
				MPI_COMM_WORLD);

			MPI_Reduce(
				&norm, &global_norm, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

			MPI_Bcast(&global_norm, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		std::cout << "IM HERE" << rank << std::endl;
		} while (global_norm > eps);
		delete[] temp_coeffs;
		return coeffs;
	}

	double* new_jacobi(
		int row_num, int col_num, double* matrix_part, double* answers_part, double* coeffs, double eps,
		int rank, int* receiveCount, int* displs) {
		std::cout << "JACOBI " << rank << std::endl;
		auto* temp_coeffs = new double[row_num];
		double norm = 0.0;
		double global_norm = 0.0;

		do {
			for (int i = 0; i < row_num; i++) {
				temp_coeffs[i] = answers_part[i];
				for (int g = 0; g < col_num; g++) {
					if (i + displs[rank] != g) {
						temp_coeffs[i] -= matrix_part[i * col_num + g] * coeffs[g];
					}
				}
				temp_coeffs[i] /= matrix_part[i * col_num + i + displs[rank]];
			}

			norm = fabs(coeffs[displs[rank]] - temp_coeffs[0]);
			for (int h = 0; h < row_num; h++) {
				if (fabs(coeffs[h + displs[rank]] - temp_coeffs[h]) > norm) {
					norm = fabs(coeffs[h + displs[rank]] - temp_coeffs[h]);
				}
				coeffs[h + displs[rank]] = temp_coeffs[h];
			}
			//MPI_Barrier(MPI_COMM_WORLD);
			MPI_Allgatherv(
				temp_coeffs, row_num, MPI_DOUBLE, coeffs, receiveCount, displs, MPI_DOUBLE,
				MPI_COMM_WORLD);

			MPI_Reduce(
				&norm, &global_norm, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

			MPI_Bcast(&global_norm, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
			std::cout << "IM HERE" << rank << std::endl;
		} while (global_norm > eps);
		delete[] temp_coeffs;
		return coeffs;
	}
}
#endif LAB2_JACOBI_HPP