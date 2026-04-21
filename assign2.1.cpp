#include <bits/stdc++.h>
using namespace std;

struct Node {
    vector<int> board;
    int g;
    int h;

    int f() const { return g + h; }

    bool operator>(const Node& other) const {
        if (f() == other.f()) {
            return h > other.h;
        }
        return f() > other.f();
    }
};

int calculateManhattan(const vector<int>& board) {
    int distance = 0;
    for (int i = 0; i < 9; ++i) {
        if (board[i] != 0) {
            int targetX = (board[i] - 1) % 3;
            int targetY = (board[i] - 1) / 3;
            int currentX = i % 3;
            int currentY = i / 3;
            distance += abs(currentX - targetX) + abs(currentY - targetY);
        }
    }
    return distance;
}

vector<vector<int>> getNeighbors(const vector<int>& board) {
    vector<vector<int>> neighbors;
    auto it = find(board.begin(), board.end(), 0);
    int blank_idx = distance(board.begin(), it);
    int row = blank_idx / 3;
    int col = blank_idx % 3;

    int dRow[] = {-1, 1, 0, 0};
    int dCol[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        int newRow = row + dRow[i];
        int newCol = col + dCol[i];
        
        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3) {
            int new_idx = newRow * 3 + newCol;
            vector<int> newBoard = board;
            swap(newBoard[blank_idx], newBoard[new_idx]);
            neighbors.push_back(newBoard);
        }
    }
    return neighbors;
}

void printPuzzle(const vector<int>& board, int step) {
    cout << "Step " << step << ":\n";
    for (int i = 0; i < 9; ++i) {
        if (board[i] == 0) cout << "_ ";
        else cout << board[i] << " ";
        if ((i + 1) % 3 == 0) cout << "\n";
    }
    cout << "----------\n";
}

void solve8Puzzle(const vector<int>& startState) {
    vector<int> goalState = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    priority_queue<Node, vector<Node>, greater<Node>> openSet;
    map<vector<int>, int> gScore;
    map<vector<int>, vector<int>> cameFrom;
    
    openSet.push({startState, 0, calculateManhattan(startState)});
    gScore[startState] = 0;
    
    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();
        
        if (current.board == goalState) {
            vector<vector<int>> path;
            vector<int> currState = current.board;
            
            while (cameFrom.find(currState) != cameFrom.end()) {
                path.push_back(currState);
                currState = cameFrom[currState];
            }
            path.push_back(startState);
            reverse(path.begin(), path.end());
            
            cout << "Goal reached in " << path.size() - 1 << " moves!\n\n";
            for (size_t i = 0; i < path.size(); ++i) {
                printPuzzle(path[i], i);
            }
            return;
        }
        
        if (current.g > gScore[current.board]) continue;
        
        for (const auto& neighbor : getNeighbors(current.board)) {
            int tentative_gScore = current.g + 1;
            
            if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor]) {
                cameFrom[neighbor] = current.board;
                gScore[neighbor] = tentative_gScore;
                openSet.push({neighbor, tentative_gScore, calculateManhattan(neighbor)});
            }
        }
    }
    cout << "No solution found. Ensure the puzzle is solvable.\n";
}

int main() {
    vector<int> initialState = {1, 2, 3, 4, 0, 6, 7, 5, 8};
    cout << "Solving the 8-puzzle using A* Algorithm...\n\n";
    solve8Puzzle(initialState);
    return 0;
}