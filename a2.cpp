#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

class Node {
public:
    int x, y;
    int g, h, f;
    Node* parent;

    Node(int x, int y, int g, int h, Node* parent = nullptr) {
        this->x = x;
        this->y = y;
        this->g = g;
        this->h = h;
        this->f = g + h;
        this->parent = parent;
    }
};

class AStar {
private:
    int rows, cols;
    vector<vector<int>> maze;
    vector<vector<bool>> visited;
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

public:
    AStar(vector<vector<int>>& grid) {
        maze = grid;
        rows = grid.size();
        cols = grid[0].size();
        visited.resize(rows, vector<bool>(cols, false));
    }

    int heuristic(int x1, int y1, int x2, int y2) {
        return abs(x1 - x2) + abs(y1 - y2);
    }

    vector<pair<int, int>> getPath(Node* goalNode) {
        vector<pair<int, int>> path;
        Node* curr = goalNode;
        while (curr != nullptr) {
            path.push_back({curr->x, curr->y});
            curr = curr->parent;
        }
        reverse(path.begin(), path.end());
        return path;
    }

    vector<pair<int, int>> solve(int sx, int sy, int gx, int gy, int& finalCost) {
        auto cmp = [](Node* a, Node* b) {
            return a->f > b->f;
        };

        priority_queue<Node*, vector<Node*>, decltype(cmp)> openList(cmp);

        int h = heuristic(sx, sy, gx, gy);
        Node* startNode = new Node(sx, sy, 0, h); 
        openList.push(startNode);

        while (!openList.empty()) {
            Node* current = openList.top();
            openList.pop();

            int x = current->x;
            int y = current->y;

            if (visited[x][y]) continue;
            visited[x][y] = true;

            if (x == gx && y == gy) {
                finalCost = current->g;
                return getPath(current);
            }

            for (auto dir : directions) {
                int nx = x + dir.first;
                int ny = y + dir.second;

                if (nx >= 0 && ny >= 0 && nx < rows && ny < cols &&
                    !visited[nx][ny] && maze[nx][ny] != -1) {

                    int newG = current->g + maze[nx][ny];
                    int newH = heuristic(nx, ny, gx, gy);
                    Node* neighbor = new Node(nx, ny, newG, newH, current);
                    openList.push(neighbor);
                }
            }
        }

        finalCost = -1;
        return {};
    }
};

int main() {
    int rows, cols;
    cout << "\n================ A* SEARCH ALGORITHM ================" << endl;
    cout << "Enter total number of Rows: ";
    cin >> rows;
    cout << "Enter total number of Columns: ";
    cin >> cols;

    vector<vector<int>> maze(rows, vector<int>(cols));
    cout << "\n--- Grid Configuration ---" << endl;
    cout << "Enter the maze values row by row." << endl;
    cout << "(-1 for BLOCKED, 1 or higher for path WEIGHT/COST):" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << "Value for [" << i << "][" << j << "]: ";
            cin >> maze[i][j];
        }
    }

    int sx, sy, gx, gy;
    cout << "\n--- Coordinates ---" << endl;
    cout << "Enter Start Node coordinates (row and col): ";
    cin >> sx >> sy;
    cout << "Enter Goal Node coordinates (row and col): ";
    cin >> gx >> gy;

    if (sx < 0 || sx >= rows || sy < 0 || sy >= cols || gx < 0 || gx >= rows || gy < 0 || gy >= cols) {
        cout << "\nError: Coordinates are outside the grid boundaries!" << endl;
        return 0;
    }

    AStar astar(maze);
    int finalCost;
    vector<pair<int, int>> path = astar.solve(sx, sy, gx, gy, finalCost);

    if (finalCost == -1) {
        cout << "\nRESULT: No path could be found from Start to Goal." << endl;
        return 0;
    }

    cout << "\n--- Path Visualization ---" << endl;
    vector<vector<string>> display(rows, vector<string>(cols, ". "));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == -1) display[i][j] = "# ";
        }
    }

    for (auto p : path) display[p.first][p.second] = "* ";
    display[sx][sy] = "S ";
    display[gx][gy] = "G ";

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << display[i][j];
        }
        cout << endl;
    }

    cout << "\nLegend: S=Start, G=Goal, *=Path, #=Blocked, .=Unvisited" << endl;
    cout << "Total Path Cost: " << finalCost << endl;
    cout << "Path Sequence: ";
    for (size_t i = 0; i < path.size(); i++) {
        cout << "(" << path[i].first << "," << path[i].second << ")" << (i == path.size() - 1 ? "" : " -> ");
    }
    cout << endl;

    return 0;
}