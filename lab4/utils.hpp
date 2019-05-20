#ifndef LAB4_UTILS_HPP
#define LAB4_UTILS_HPP

#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <iostream>
#include <climits>

namespace utils {
    using graph = std::vector<std::vector<std::pair<int, int>>>;
    using matrix = std::vector<int>;
    const int inf = INT_MAX;
    void readMatrix(std::string filename, int size, graph& graph) {
        std::ifstream file(filename);
        matrix matrix;
        int nodes = 0;
        std::string temp;
        if (!file) {
            std::cerr << "Error opening matrix file.\n";
            throw std::runtime_error("Could not open the file");
        }
        file >> nodes;
        size = nodes;
        graph.resize(nodes);
        matrix.resize(nodes * nodes);
        for (int i = 0; i < nodes; i++) {
            for (int j = 0; j < nodes; j++) {
                int index = i * nodes + j;
                file >> temp;
                if ("inf" == temp) {
                    matrix[index] = inf;
                } else {
                    matrix[index] = std::stoi(temp);
                }
            }
        }
        for (int i = 0; i < nodes; i++) {
            for (int j = 0; j < nodes; j++) {
                int index = i * nodes + j;
                if (matrix[index] != inf) {
                    graph[i].push_back(std::make_pair(j, matrix[index]));
                }
            }
        }
    }

    void printGraph(graph graph) {
        int size = 4;
        for (int i = 0; i < size; i++) {
            for (std::vector<std::pair<int, int>>::const_iterator iter = graph[i].begin();
                 iter != graph[i].end();
                 ++iter) {
                std::cout << "[" << i << "] - #" << iter->first << ", Second: " << iter->second << std::endl;
            }
        }
    }

    bool gen_write_adjacency_matrix(
            size_t nodes_number, std::string filename, size_t min_border,
            size_t max_border) {
        std::ofstream fstr;
        bool ok = false;

        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_int_distribution<size_t> distr(min_border, max_border);
        std::uniform_int_distribution<size_t> probability(0, 1);

        std::vector<std::vector<size_t>> matrix(nodes_number);
        for (auto &intra : matrix)
            intra.resize(nodes_number);

        auto inf = std::numeric_limits<size_t>::max();

        for (size_t i = 0; i < nodes_number; ++i) {
            for (size_t j = i; j < nodes_number; ++j) {
                if (j == i) {
                    matrix[i][j] = 0;
                    continue;
                }
                auto prob = probability(generator);
                auto path_len = inf;
                if (prob)
                    path_len = distr(generator);
                matrix[i][j] = matrix[j][i] = path_len;
            }
        }

        fstr.open(filename);
        if (fstr) {
            ok = true;
            fstr << nodes_number << std::endl;
            for (const auto &inner : matrix) {
                for (auto val : inner) {
                    if (val == inf) {
                        fstr << "inf ";
                        continue;
                    }
                    fstr << val << " ";
                }
                fstr << std::endl;
            }
            fstr.close();
        }

        return ok;
    }

    void printVector(const std::vector<int> &distances) {
        for (int i = 0; i < distances.size(); i++) {
            auto dist = distances[i] == inf ? "inf" : std::to_string(distances[i]);
            std::cout << "[" << i << "] - " << dist << std::endl;
        }
    }

    void printAnswer(int root, const std::string &outFile, const std::vector<int> &distances) {
        std::ofstream fout(outFile, std::ios_base::trunc);
        fout << "From the node " << root << std::endl;
        for (int i = 0; i < distances.size(); i++) {
            auto dist = distances[i] == inf ? "inf" : std::to_string(distances[i]);
            fout << "[" << i << "] - " << dist << std::endl;
        }
    }
}
#endif //LAB4_UTILS_HPP
