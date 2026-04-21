#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>

using namespace std;

class PuneCampusGraph {
public:
    unordered_map<string, vector<string>> adj;

    void addEdge(string u, string v) {
        adj[u].push_back(v);
    }

    bool exists(string node) {
        return adj.find(node) != adj.end();
    }

    void displayAdjList() {
        for (auto const& [college, neighbors] : adj) {
            cout << college << " -> ";
            for (size_t i = 0; i < neighbors.size(); ++i) {
                cout << neighbors[i] << (i == neighbors.size() - 1 ? "" : ", ");
            }
            cout << endl;
        }
    }

    void bfsIterative(string startNode) {
        unordered_set<string> visited;
        queue<string> q;
        visited.insert(startNode);
        q.push(startNode);
        while (!q.empty()) {
            string curr = q.front();
            q.pop();
            cout << curr << " ";
            for (const string& neighbor : adj[curr]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.push(neighbor);
                }
            }
        }
        cout << endl;
    }

    void bfsRecursive(queue<string>& q, unordered_set<string>& visited) {
        if (q.empty()) return;
        string curr = q.front();
        q.pop();
        cout << curr << " ";
        for (const string& neighbor : adj[curr]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
        bfsRecursive(q, visited);
    }

    void dfsRecursive(string curr, unordered_set<string>& visited) {
        visited.insert(curr);
        cout << curr << " ";
        for (const string& neighbor : adj[curr]) {
            if (visited.find(neighbor) == visited.end()) {
                dfsRecursive(neighbor, visited);
            }
        }
    }

    void dfsIterative(string startNode) {
        unordered_set<string> visited;
        stack<string> s;
        s.push(startNode);
        while (!s.empty()) {
            string curr = s.top();
            s.pop();
            if (visited.find(curr) == visited.end()) {
                cout << curr << " ";
                visited.insert(curr);
            }
            for (auto it = adj[curr].rbegin(); it != adj[curr].rend(); ++it) {
                if (visited.find(*it) == visited.end()) {
                    s.push(*it);
                }
            }
        }
        cout << endl;
    }
};

int main() {
    PuneCampusGraph g;
    g.addEdge("COEP", "PICT");
    g.addEdge("COEP", "VIT");
    g.addEdge("PICT", "CUMMINS");
    g.addEdge("VIT", "MIT");
    g.addEdge("MIT", "PICT");
    g.addEdge("CUMMINS", "COEP");

    int choice;
    string startNode;

    do {
        cout << "\n--- Pune College Graph Menu ---" << endl;
        cout << "1. Display Adjacency List" << endl;
        cout << "2. BFS Iterative" << endl;
        cout << "3. BFS Recursive" << endl;
        cout << "4. DFS Iterative" << endl;
        cout << "5. DFS Recursive" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice >= 2 && choice <= 5) {
            cout << "Enter starting college (e.g., COEP, PICT, VIT): ";
            cin >> startNode;
            if (!g.exists(startNode)) {
                cout << "Error: College not found in graph!" << endl;
                continue;
            }
        }

        switch (choice) {
            case 1:
                g.displayAdjList();
                break;
            case 2:
                cout << "BFS Iterative: ";
                g.bfsIterative(startNode);
                break;
            case 3: {
                cout << "BFS Recursive: ";
                queue<string> q;
                unordered_set<string> v;
                q.push(startNode);
                v.insert(startNode);
                g.bfsRecursive(q, v);
                cout << endl;
                break;
            }
            case 4:
                cout << "DFS Iterative: ";
                g.dfsIterative(startNode);
                break;
            case 5: {
                cout << "DFS Recursive: ";
                unordered_set<string> v;
                g.dfsRecursive(startNode, v);
                cout << endl;
                break;
            }
            case 6:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 6);

    return 0;
}