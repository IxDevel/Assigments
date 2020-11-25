#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <queue>
#include <iomanip>

using namespace std;

/*  Week 2 Assignment: Implement HexGame
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
    explicit Graph(int size);
    void addEdge(int row, int col, int weight);
    int getElement(int row, int col) const;
    int getMatrixSize() const;
    void getMatrix(vector<vector<int>> &g) const;
    friend std::ostream& operator<<(std::ostream& out, const Graph& g);
};

// This method returns a copy of the graph matrix
void Graph::getMatrix(vector<vector<int>> &g) const {
    g.assign(matrix.begin(), matrix.end());
}

// Graph object constructor
Graph::Graph(int size): matrix(*(new vector<vector<int>>(size, vector<int> (size, 0)))) {}

// This appends and edge between to vertices
void Graph::addEdge(int row, int col, int weight) {
    if (row < matrix.size() && row > -1 && col < matrix.size() && col > -1 && col != row)
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

// Allows to the graph class to be printed whit cout
std::ostream& operator<<(std::ostream& out, const Graph& g) {
    for (int i = 0; i < g.getMatrixSize(); i++) {
        for (int j = 0; j < g.getMatrixSize(); j++)
            out << "[" << std::setw(2) << g.getElement(i,j) << "]";
        out << endl;
    }
    return out;
}

// A class to check the win condition
class Checker {
public:
    bool checkWinCondition(Graph &g, char targetMoveV, char targetMoveH, vector<char> &values) const;
};

/*  This method is used to get a path from a node to the others
 *  Parameters:
 *  Graph g: A Graph filled object
 *  initialNode: The initial node were whe are going start
 */
bool Checker::checkWinCondition(Graph &g, char targetMoveV, char targetMoveH, vector<char> &values) const {
    // Just copy the matrix from the graph
    vector<vector<int>> matrix;
    g.getMatrix(matrix);
    int size =matrix.size();
    int mod = static_cast<int>(sqrt(size));
    // Visisted vector
    vector<bool> visited(size, false);
    // Trace queue
    queue<int> traceH, traceV;

    // Pre-made the exits and starts values
    vector<int> sourceH(mod, 0), sourceV(mod, 0), targetH(mod, 0), targetV(mod, 0);
    for(int pos = 0; pos < mod; pos++) {
        sourceV[pos] = pos;
        targetV[pos] = mod*(mod-1) + pos;
        sourceH[pos] = mod*pos;
        targetH[pos] = mod*pos + (mod-1);
    }

    // Current selected node
    int currentNode;
    // This part takes a start pos and the use it to get the connected
    // nodes from that position. At the end if it realize that you have
    // reached a end node it returns true, meaning that a player won the
    // game, else it returns false
    for(int pos=0; pos < mod; pos++) {
        for (int i=0; i<size; i++)
            visited[i] = false;
        if (values[sourceH[pos]] == targetMoveH) {
            traceH.push(sourceH[pos]);
            visited[sourceH[pos]] = true;
            do {
                currentNode = traceH.front();
                traceH.pop();

                for(int i=0; i<size; i++)
                    if (matrix[currentNode][i] != 0 && values[i]==targetMoveH && visited[i]==false) {
                        traceH.push(i);
                        visited[i] = true;
                    }
            } while(!traceH.empty());
        }

        if (values[sourceV[pos]] == targetMoveV) {
            traceV.push(sourceV[pos]);
            visited[sourceV[pos]] = true;
            do {
                currentNode = traceV.front();
                traceV.pop();
                for(int i=0; i < size; i++)
                    if (matrix[currentNode][i] != 0 && values[i]==targetMoveV && visited[i]==false) {
                        traceV.push(i);
                        visited[i] = true;
                    }
            } while(!traceV.empty());
        }

        for(bool b: visited)
            cout << b <<  " ";
        cout << endl;
        for(int i=0; i<size; i++)
            for(int j=0; j<mod; j++)
                if(visited[i] == true && i == targetH[j] &&  values[targetH[j]] == targetMoveH) {
                    cout << "The winner is player with: " << targetMoveH << endl << i << endl;
                    return true;
                } else if(visited[i] == true && i == targetV[j] && values[targetV[j]] == targetMoveV) {
                    cout << "The winner is player with: " << targetMoveV << endl << targetV[j] << endl;
                    return true;
                }
    }
    cout << "There isn't a winner yet" << endl << endl;
    return false;
}

// Hexboard class to put the game board
// It has a value list for the game tokens
// Checker object
// Size var
class HexBoard {
private:
    Graph &graph;
    vector<char> &values;
    Checker wC;
    int size;
public:
    explicit HexBoard(int size);
    int getElement(int row, int col) const {return graph.getElement(row,col);};
    int getSize() const {return size;}
    void getValues(vector<char> &v) const {v.assign(values.begin(), values.end());}
    friend std::ostream& operator<<(std::ostream& out, const HexBoard& b);
    bool checkWinCondition(char targetMoveV, char targetMoveH) {return wC.checkWinCondition(graph,targetMoveV, targetMoveH, values);}
    void makeMove(char targetToken, int targetPos);
};

// Add a token to the board
void HexBoard::makeMove(char targetToken, int targetPos) {
    if (targetPos > -1 && targetPos < size && values[targetPos] == '.')
        values[targetPos] = targetToken;
}

// Constructor
HexBoard::HexBoard(int length):
    graph(*(new Graph(length*length))),
    size(length*length),
    values(*(new vector<char>(length*length, '.'))) {
    for(int row, col, mod = static_cast<int>(sqrt(size)), tile=0; tile < size; tile++) {
        if (tile % mod != mod - 1) {
            graph.addEdge(tile, tile + 1, 1);
            graph.addEdge(tile, tile - mod + 1, 1);
        }
        if (tile % mod != 0) {
            graph.addEdge(tile, tile - 1, 1);
            graph.addEdge(tile, tile + mod - 1, 1);
        }
        graph.addEdge(tile, tile + mod, 1);
        graph.addEdge(tile, tile - mod, 1);
    }
}

// Output printer
std::ostream& operator<<(std::ostream& out, const HexBoard& b) {
    vector<char> values;
    b.getValues(values);
    for(int i=0; i < static_cast<int>(sqrt(b.getSize())); i++)
        cout << "   " << i ;
    cout << endl << "   ";
    for(int pos = 0, size = b.getSize(), mod = static_cast<int>(sqrt(size)); pos < size; pos++) {
        if (pos % mod < mod -1)
            cout << values[pos] << " - ";
        else
            cout << values[pos];
        if (pos % mod == mod - 1 && pos < size-1) {
            cout << endl << "   ";
            for (int times = 0; times < (pos / mod)*2+1; times++)
                cout << " ";
            for (int times = 0; times < mod; times++) {
                cout << "\\ ";
                if(times + 1 != mod)
                    cout << "/ ";
            }

            cout << endl;
            cout << std::setw(3) << pos+1;
            for (int times = 0; times < (pos / mod)*2+2; times++)
                cout << " ";
        }
    }
    return out;
}

// Just main :)
int main() {
    auto &board = *(new HexBoard(11));
    int op=0, pos=0;
    do {
        //system("cls");
        cout << board << endl;
        cout << "Chose a player" << endl;
        cout << "1 for player V (top-bottom)" << endl;
        cout << "2 for horizontal player H (left-right)" << endl;
        cout << "Other number to exit" << endl;
        cout << "Option:";
        std::cin >> op;
        if (op > 0 && op < 3) {
            cout << "Chose a pos from "<< 0 << " to " << board.getSize()-1 << endl;
            cout << "Option:";
            std::cin >> pos;
            board.makeMove((op==1)? 'V':'H', pos);
        } else
            break;
    } while (!board.checkWinCondition('V', 'H'));
    return 0;
}
