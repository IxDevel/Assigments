#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <queue>
#include <iomanip>

using namespace std;

/*  Week 3 Assignment: Short path algorithm
 *  Created at: 21-10-2020
 *  Last modification: 24-10-2020
 *  Made by: Isaac Da Pena
 */

/*  Graph class is a matrix graph representation
 */
class Graph {
private:
    vector<vector<int>> &matrix;
public:
    Graph(int size);
    void fillRandom(float density);
    void addEdge(int row, int col, int weight);
    int getElement(int row, int col) const;
    int getMatrixSize() const;
    void getMatrix(vector<vector<int>> &g) const;
};

// This method returns a copy of the graph matrix
void Graph::getMatrix(vector<vector<int>> &g) const {
    g.assign(matrix.begin(), matrix.end());
}

// Graph object constructor
Graph::Graph(int size): matrix(*(new vector<vector<int>>(size, vector<int> (size, 0)))){}

// This appends and edge between to vertices
void Graph::addEdge(int row, int col, int weight) {
    if (row < matrix.size() and col < matrix.size())
        matrix[col][row] = matrix[row][col] = weight;
}

// Returns the matrix size, it is the same as matrix.size()
int Graph::getMatrixSize() const {
    return this->matrix.size();
}

// Returns a element of the matrix
int Graph::getElement(int row, int col) const {
    return this->matrix[col][row];
}

// Fills with random weights the graph using a density
void Graph::fillRandom(float density) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,10);
    auto roller = std::bind(distribution, generator);
    for (int i = 0; i < matrix.size(); i++)
        for (int j = 0; j < matrix.size(); j++) {
            if (i < j+1) break;
            if (roller() < density*10)
                addEdge(i, j, roller());
            else
                addEdge(i, j, 0);
        }
}

// Allows to the graph class to be printed whit cout
std::ostream& operator<<(std::ostream& out, const Graph& g) {
    for (int i = 0; i < g.getMatrixSize(); i++) {
        for (int j = 0; j < g.getMatrixSize(); j++)
            out << "[" << std::setw(2) << g.getElement(i,j) << "]";
        out << endl;
    }
    return out;
}

// This is the short path class, it is used to find the shortest
class ShortPath {
public:
    void getShortPath(Graph &g, int initialNode) const;
};

/*  This method is used to get the shortest path from a node to the others
 *  It is based on the Floyd-Warshal algorithm, so it allows to compute the
 *  from one node to the others using a cost matrix and a tracer vector.
 *  Initially I made a recursive method of dijsktra algorithm but I realize
 *  that it was inefficient so I made a iterative version. When I was investigating
 *  I found that it was possible to get all the paths from a node.
 *  Parameters:
 *  Graph g: A Graph filled object
 *  initialNode: The initial node were whe are going start
 */
void ShortPath::getShortPath(Graph &g, int initialNode) const {
    // Just copy the matrix from the graph
    vector<vector<int>> matrix;
    g.getMatrix(matrix);
    vector<int> distance(matrix.size(), 0);
    vector<bool> visited(matrix.size(), false);
    vector<queue<int>> trace(matrix.size());
    const int NOEDGE = 9999;

    for (int row = 0; row < matrix.size(); row++) {
        if (matrix[initialNode][row] == 0)
            distance[row] = NOEDGE;
        else
            distance[row] = matrix[initialNode][row];
        trace[row].push(initialNode);
    }

    distance[initialNode] = 0;
    visited[initialNode] = true;

    for (int row = 0, candidate, min; row < matrix.size(); row++)
        if (!visited[row]) {
            candidate = -1;
            min = NOEDGE;
            for (int ver = 0; ver < matrix.size(); ver++)
                if (!visited[ver] && min > distance[ver]) {
                    candidate = ver;
                    min = distance[ver];
                }
            
            visited[candidate] = true;
            for (int ver = 0, already = 0; ver < matrix.size(); ver++)
                if (!visited[ver] && matrix[candidate][ver] != 0 && distance[ver] > distance[candidate] + matrix[candidate][ver]) {
                    distance[ver] = distance[candidate] + matrix[candidate][ver];
                    // Append the candidate
                    trace[ver].push(candidate);
                    already = 1;
                }
            // Add the current node to de queue vector
            trace[row].push(row);
        }
        else if (trace[row].front() != row)
            trace[row].push(row);

    // Print the trace queue vector
    for (int node = 0; node < matrix.size(); node++) {
        if(node == initialNode) continue;
        cout << "Path from " << initialNode << " to " << node << " length: " << distance[node] << endl;
        if (trace[node].size() < 2) {
            cout << "Nodes are not connected" << endl;
            continue;
        }
        while (true) {
            cout << trace[node].front();
            trace[node].pop();
            if (trace[node].empty()) {
                cout << endl;
                break;
            }
            else
                cout << "->";
        }
    }
}

int main() {
    // Create a new Graph object
    auto &g = *(new Graph(50));
    // Create a new short path finder
    auto &finder = *(new ShortPath());
    // Fill the Graph
    g.fillRandom(0.8);
    // Print it
    cout << g << endl;
    // Find the shortest paths from and to all nodes
    finder.getShortPath(g, i);
    return 0;
}
