#include <iostream>
#include <fstream>
#include <omp.h>
#include "Matrix.h"

Matrix::Matrix(char *filename) {
    std::ifstream input;
    try {
        input.open(filename);
        input >> rows >> columns;
        matrix = new int *[rows];
        for (int i = 0; i < rows; i++) {
            matrix[i] = new int[columns];
            for (int j = 0; j < columns; j++) {
                input >> matrix[i][j];
            }
        }
    }
    catch (std::ifstream::failure &e) {
        std::cout << "IO exception" << std::endl;
    }
    input.close();
}

Matrix::Matrix(int rows, int columns, int **m) {
    rows = rows;
    columns = columns;
    matrix = m;
}

Matrix::~Matrix() {
    for (int i = 0; i < rows; i++) {
        delete matrix[i];
    }
    delete matrix;
}

int Matrix::getRowsNum() {
    return rows;
}

int Matrix::getColumnsNum() {
    return columns;
}

int Matrix::getElement(int i, int j) {
    return matrix[i][j];
}

Matrix Matrix::operator*(Matrix &b) {
    int bColumns = b.getColumnsNum();
    int **result = new int *[rows];
    for (int i = 0; i < rows; i++) {
        result[i] = new int[bColumns];
        for (int j = 0; j < bColumns; j++) {
            result[i][j] = 0;
        }
    }
#pragma omp parallel for
    for (int step = 0; step < rows * bColumns; step++) {
        int i = step / bColumns;
        int j = step % bColumns;
        int sum = 0;
        for (int inner = 0; inner < columns; inner++) {
            sum += getElement(i, inner) * b.getElement(inner, j);
        }
        result[i][j] = sum;
    }
    return Matrix(rows, bColumns, result);
}