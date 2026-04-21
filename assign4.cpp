#include <bits/stdc++.h>
using namespace std;

class NQueens {
public:
    int n;
    int stepCount;
    vector<vector<int>> board;

    NQueens(int size) {
        n = size;
        stepCount = 0;
        board.assign(n, vector<int>(n, 0));
    }

    void printBoard() {
        cout << "Step " << ++stepCount << ":\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j]) cout << " Q ";
                else cout << " . ";
            }
            cout << "\n";
        }
        cout << "----------------\n";
    }

    bool isSafe(int row, int col) {
        for (int i = 0; i < col; i++)
            if (board[row][i]) return false;

        for (int i = row, j = col; i >= 0 && j >= 0; i--, j--)
            if (board[i][j]) return false;

        for (int i = row, j = col; j >= 0 && i < n; i++, j--)
            if (board[i][j]) return false;

        return true;
    }

    bool solveBacktracking(int col = 0) {
        if (col >= n) return true;

        for (int i = 0; i < n; i++) {
            if (isSafe(i, col)) {
                board[i][col] = 1;
                printBoard();
                
                if (solveBacktracking(col + 1)) return true;
                
                board[i][col] = 0;
                printBoard();
            }
        }
        return false;
    }

    bool solveBranchAndBound(int col, vector<bool>& rows, vector<bool>& slash, vector<bool>& backslash) {
        if (col >= n) return true;

        for (int i = 0; i < n; i++) {
            if (!rows[i] && !slash[i + col] && !backslash[i - col + n - 1]) {
                board[i][col] = 1;
                rows[i] = slash[i + col] = backslash[i - col + n - 1] = true;
                printBoard();

                if (solveBranchAndBound(col + 1, rows, slash, backslash)) return true;

                board[i][col] = 0;
                rows[i] = slash[i + col] = backslash[i - col + n - 1] = false;
                printBoard();
            }
        }
        return false;
    }

    void solve(int mode) {
        bool found = false;
        stepCount = 0;
        
        if (mode == 1) {
            found = solveBacktracking();
        } else {
            vector<bool> rows(n, false);
            vector<bool> slash(2 * n - 1, false);
            vector<bool> backslash(2 * n - 1, false);
            found = solveBranchAndBound(0, rows, slash, backslash);
        }

        if (found) {
            cout << "\nFinal Solution Found for N=" << n << " in " << stepCount << " moves!\n";
            printBoard();
        } else {
            cout << "\nNo solution exists for N=" << n << ".\n";
        }
    }
};

int main() {
    int choice, n;

    do {
        cout << "\n--- N-Queens Menu ---\n";
        cout << "1. Backtracking\n";
        cout << "2. Branch and Bound\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1 || choice == 2) {
            cout << "Enter the value of N: ";
            cin >> n;
            
            if (n <= 0) {
                cout << "Please enter a positive integer.\n";
                continue;
            }
            
            NQueens solver(n);
            solver.solve(choice);
        }
    } while (choice != 3);

    return 0;
}