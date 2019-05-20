#include <tuple>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <string>
#include <ctime>
#include <algorithm>
#include <omp.h>
#include "utils.hpp"

using namespace std;

using graph = vector<vector<std::pair<int, int>>>;
const int inf = INT_MAX;

int main(int argc, char *argv[]) {
    if (argc != 5) return -1;
    string inputMatrixFile = argv[1];
    string answerFile = argv[2];
    int startNode = std::atoi(argv[3]);
    int thread_num = std::atoi(argv[4]);
    omp_set_dynamic(0);
    omp_set_num_threads(thread_num);

    //gen_write_adjacency_matrix(10, "matrix.txt", 1, 10);
    graph graph;
    int j ,k, nodes, path_len, len, dest, graph_size, shortest;
    //int startNode = 0;
    utils::readMatrix("mat.txt", nodes, graph);
    cout << "loaded" << endl;
    nodes = graph.size();
    //utils::printGraph(graph);

    vector<int> distances(nodes, inf), parents(nodes);
    vector<char> visited(nodes);
    vector<int> thread_min(omp_get_max_threads(), inf);
    vector<int> thread_id(omp_get_max_threads(), nodes + 1);
    distances[startNode] = 0;
    int chunk = nodes / omp_get_max_threads();

    for (int i = 0; i < nodes; ++i) {
        std::fill(thread_min.begin(), thread_min.end(), inf);
        std::fill(thread_id.begin(), thread_id.end(), nodes + 1);
#pragma omp parallel for schedule(dynamic, chunk) shared(visited, distances, thread_min, thread_id) firstprivate(nodes) private(shortest, j)
        for (j = 0; j < nodes; ++j) {
            shortest = thread_id[omp_get_thread_num()];
            if (!visited[j]
                && (shortest == nodes + 1
                    || distances[j] < distances[shortest])) {
                thread_id[omp_get_thread_num()] = j;
                thread_min[omp_get_thread_num()] = distances[j];
            }
        }

        shortest = thread_id[std::min_element(thread_min.begin(), thread_min.end()) - thread_min.begin()];

        if (distances[shortest] == inf)
            break;
        visited[shortest] = true;

        graph_size = graph[shortest].size();
        chunk = graph_size / omp_get_max_threads();

#pragma omp parallel for firstprivate(shortest, graph_size) private(dest, len, path_len, k) schedule(dynamic, chunk) shared(graph, distances)
        for (k = 0; k < graph_size; ++k) {
            dest = graph[shortest][k].first;
            len = graph[shortest][k].second;
            path_len = distances[shortest] + len;
            if (path_len < distances[dest]) {
                distances[dest] = path_len;
                parents[dest] = shortest;
            }
        }
    }

/*vector<int> path;
for (int v = 4; v != startNode; v = parents[v])
    path.push_back(v);
path.push_back(startNode);
reverse(path.begin(), path.end());*/

    utils::printVector(distances);
    utils::printAnswer(startNode, answerFile, distances);

    return 0;
}

