#include <tuple>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <string>
#include <ctime>
#include <algorithm>
#include "utils.hpp"

using namespace std;

using graph = vector<vector<std::pair<int, int>>>;
const int inf = INT_MAX;

int main(int argc, char* argv[]) {
    if (argc != 4) return -1;

    string inputMatrixFile = argv[1];
    string answerFile = argv[2];
    int startNode = std::atoi(argv[3]);

    //gen_write_adjacency_matrix(10, "matrix.txt", 1, 10);
    graph graph;
    int nodes;
//    int startNode = 0;
    utils::readMatrix("mat.txt", nodes, graph);
    cout << "loaded" << endl;
    nodes = graph.size();
//    utils::printGraph(graph);

    vector<int> distances(nodes, inf), parents(nodes);
    distances[startNode] = 0;
    vector<char> visited(nodes);
    for (int i = 0; i < nodes; ++i) {
        int shortest = -1;
        for (int j = 0; j < nodes; ++j)
            if (!visited[j] && (shortest == -1 || distances[j] < distances[shortest]))
                shortest = j;
        if (distances[shortest] == inf)
            break;
        visited[shortest] = true;

        for (size_t j = 0; j < graph[shortest].size(); ++j) {
            int dest = graph[shortest][j].first,
                    len = graph[shortest][j].second;
            if (distances[shortest] + len < distances[dest]) {
                distances[dest] = distances[shortest] + len;
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

