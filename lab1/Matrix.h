#pragma once
#ifndef LAB1_MATRIX_HPP
#define LAB1_MATRIX_HPP

class Matrix {
private:
    int rows, columns;
    int **matrix;
    Matrix(int nRows, int nColumns, int **mtx);
public:
    Matrix(char* filename);

    ~Matrix();

    int getRowsNum();

    int getColumnsNum();

    int getElement(int i, int j);

    Matrix operator*(Matrix &a);

};

#endif //LAB1_MATRIX_HPP