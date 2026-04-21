#include <bits/stdc++.h>
using namespace std;

class GraphColorSolver {
public:
    int V;
    vector<vector<int>> graph;
    
    int minColorsBnB;
    vector<int> bestColoringBnB;

    GraphColorSolver(int vertices) {
        V = vertices;
        graph.assign(V, vector<int>(V, 0));
        minColorsBnB = INT_MAX;
    }

    void addEdge(int u, int v) {
        graph[u][v] = 1;
        graph[v][u] = 1;
    }

    bool solveBacktracking(int m, vector<int>& color, int v = 0) {
        if (v == V) return true;

        for (int c = 1; c <= m; c++) {
            bool safe = true;
            for (int i = 0; i < V; i++) {
                if (graph[v][i] == 1 && color[i] == c) {
                    safe = false;
                    break;
                }
            }

            if (safe) {
                color[v] = c;
                if (solveBacktracking(m, color, v + 1)) return true;
                color[v] = 0;
            }
        }
        return false;
    }

    void solveBranchAndBound(vector<int>& color, int v = 0, int maxColor = 0) {
        if (maxColor >= minColorsBnB) return; 

        if (v == V) {
            minColorsBnB = maxColor;
            bestColoringBnB = color;
            return;
        }

        for (int c = 1; c <= maxColor + 1; c++) {
            bool safe = true;
            for (int i = 0; i < V; i++) {
                if (graph[v][i] == 1 && color[i] == c) {
                    safe = false;
                    break;
                }
            }

            if (safe) {
                color[v] = c;
                solveBranchAndBound(color, v + 1, max(maxColor, c));
                color[v] = 0;
            }
        }
    }
};

int main() {
    int n = 6;
    GraphColorSolver gc(n);

    gc.addEdge(0, 1);
    gc.addEdge(0, 2);
    gc.addEdge(0, 3);
    gc.addEdge(1, 2);
    gc.addEdge(2, 3);
    gc.addEdge(3, 4);
    gc.addEdge(4, 5);
    gc.addEdge(5, 1);

    while (true) {
        cout << "\n--- Graph Coloring Menu (n=6) ---\n";
        cout << "1. Check possibility using Backtracking\n";
        cout << "2. Check possibility using Branch & Bound\n";
        cout << "3. Exit\n";
        cout << "Choice: ";
        
        int choice;
        cin >> choice;

        if (choice == 3) {
            break;
        } else if (choice == 1 || choice == 2) {
            cout << "Enter number of colors: ";
            int m;
            cin >> m;

            vector<int> color(n, 0); 

            if (choice == 1) {
                if (gc.solveBacktracking(m, color)) {
                    cout << "Result: POSSIBLE to color with " << m << " colors.\nColor assignments: ";
                    for (int c : color) cout << c << " ";
                    cout << "\n";
                } else {
                    cout << "Result: NOT POSSIBLE to color with " << m << " colors.\n";
                }
            } else {
                gc.minColorsBnB = INT_MAX;
                gc.bestColoringBnB.clear();
                
                gc.solveBranchAndBound(color);

                if (gc.minColorsBnB <= m) {
                    cout << "Result: POSSIBLE to color with " << m << " colors. (Minimum needed is " << gc.minColorsBnB << ")\nColor assignments: ";
                    for (int c : gc.bestColoringBnB) cout << c << " ";
                    cout << "\n";
                } else {
                    cout << "Result: NOT POSSIBLE to color with " << m << " colors. (Minimum needed is " << gc.minColorsBnB << ")\n";
                }
            }
        } else {
            cout << "Invalid choice!\n";
        }
    }
    return 0;
}