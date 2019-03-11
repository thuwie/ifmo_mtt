#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <omp.h>
#include "utils.hpp"
#include "matrixs.hpp"

using namespace std;

int main(int argc, char **argv) {
    string file1(argv[1]);
    string file2(argv[2]);
    int maxThreadNum = 4;
    omp_set_num_threads(maxThreadNum);
    if (argc == 6) {
        utils::generateMatrix(file1, atoi(argv[3]), atoi(argv[4]));
        utils::generateMatrix(file2, atoi(argv[4]), atoi(argv[5]));
    } else {
        if (argc != 4 || !argv[1] || !argv[2] || !argv[3]) {
            cout << "Corrupted arguments!" << std::endl;
            return 1;
        }
        int option = atoi(argv[3]);
        vector<vector<int>> matrix1 = utils::load(file1);
        vector<vector<int>> matrix2 = utils::load(file2);
        int chunksSize = ((argv[4]!= nullptr) ? atoi(argv[4]) : 800*1000/maxThreadNum);
        float time = clock();
        vector<vector<int>> answer;
        switch(option) {
            case(0): {
                answer = matrixs::commonMultiply(matrix1, matrix2);
                time = clock() - time;
                break;
            }
            case(1): {
                answer = matrixs::parallelMultiply(matrix1, matrix2, chunksSize);
                time = clock() - time;
                break;
            }
            case(2): {
                answer = matrixs::dynamicScheduleMultiply(matrix1, matrix2, chunksSize);
                time = clock() - time;
                break;
            }
            case(3): {
                answer = matrixs::guidedScheduleMultiply(matrix1, matrix2, chunksSize);
                time = clock() - time;
                break;
            }
            default:break;
        }
        std::cout <<((float) time) / CLOCKS_PER_SEC << std::endl;
        utils::answer(answer);
    }

    return 0;
}

