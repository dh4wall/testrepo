#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
#include <string>

using namespace std;

struct Edge {
    int src, dest, weight;
};

struct Process {
    int id, at, bt, ct, tat, wt;
};

class GreedyAlgorithms {
public:
    void displayDirectedAdj(int n, vector<vector<pair<int, int>>>& adj) {
        cout << "\n--- Directed Graph Adjacency List ---" << endl;
        for (int i = 0; i < n; i++) {
            cout << "Node " << i << " -> ";
            for (auto& edge : adj[i]) {
                cout << "[" << edge.first << " | W:" << edge.second << "] ";
            }
            cout << endl;
        }
    }

    void displayUndirectedAdj(int n, vector<vector<pair<int, int>>>& adj) {
        cout << "\n--- Undirected Graph Adjacency List ---" << endl;
        for (int i = 0; i < n; i++) {
            cout << "Node " << i << " -> ";
            for (auto& edge : adj[i]) {
                cout << "[" << edge.first << " | W:" << edge.second << "] ";
            }
            cout << endl;
        }
    }

    void selectionSort() {
        int n;
        cout << "\n--- Selection Sort ---" << endl;
        cout << "Enter the number of elements: ";
        cin >> n;
        vector<int> arr(n);
        for (int i = 0; i < n; i++) {
            cout << "Enter element " << i + 1 << ": ";
            cin >> arr[i];
        }

        for (int i = 0; i < n - 1; i++) {
            int min_idx = i;
            for (int j = i + 1; j < n; j++) {
                if (arr[j] < arr[min_idx]) min_idx = j;
            }
            swap(arr[min_idx], arr[i]);
        }

        cout << "Sorted Array: ";
        for (int x : arr) cout << x << " ";
        cout << endl;
    }

    void fcfs() {
        int n;
        cout << "\n--- FCFS Scheduling ---" << endl;
        cout << "Enter total processes: ";
        cin >> n;
        vector<Process> proc(n);
        for (int i = 0; i < n; i++) {
            proc[i].id = i + 1;
            cout << "P" << proc[i].id << " Arrival Time: "; cin >> proc[i].at;
            cout << "P" << proc[i].id << " Burst Time: "; cin >> proc[i].bt;
        }

        sort(proc.begin(), proc.end(), [](Process a, Process b) {
            return a.at < b.at;
        });

        int current_time = 0;
        float total_wt = 0, total_tat = 0;
        for (int i = 0; i < n; i++) {
            if (current_time < proc[i].at) current_time = proc[i].at;
            proc[i].ct = current_time + proc[i].bt;
            proc[i].tat = proc[i].ct - proc[i].at;
            proc[i].wt = proc[i].tat - proc[i].bt;
            current_time = proc[i].ct;
            total_wt += proc[i].wt;
            total_tat += proc[i].tat;
        }

        cout << "\nID\tAT\tBT\tCT\tTAT\tWT" << endl;
        for (auto& p : proc) cout << p.id << "\t" << p.at << "\t" << p.bt << "\t" << p.ct << "\t" << p.tat << "\t" << p.wt << endl;
        cout << "Avg WT: " << total_wt / n << " | Avg TAT: " << total_tat / n << endl;
    }

    void dijkstra() {
        int n = 5;
        vector<vector<pair<int, int>>> adj(n);
        adj[0].push_back({1, 10}); adj[0].push_back({4, 5});
        adj[1].push_back({2, 1});  adj[1].push_back({4, 2});
        adj[2].push_back({3, 4});
        adj[3].push_back({2, 6});  adj[3].push_back({0, 7});
        adj[4].push_back({1, 3});  adj[4].push_back({2, 9});  adj[4].push_back({3, 2});

        displayDirectedAdj(n, adj);
        int start;
        cout << "Enter Source Node (0-4): "; cin >> start;
        if (start < 0 || start >= n) return;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        vector<int> dist(n, INT_MAX);
        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            int u = pq.top().second; pq.pop();
            for (auto& edge : adj[u]) {
                if (dist[u] + edge.second < dist[edge.first]) {
                    dist[edge.first] = dist[u] + edge.second;
                    pq.push({dist[edge.first], edge.first});
                }
            }
        }
        cout << "\nShortest distances from " << start << ":" << endl;
        for (int i = 0; i < n; i++) cout << "To " << i << ": " << (dist[i] == INT_MAX ? "INF" : to_string(dist[i])) << endl;
    }

    void prims() {
        int n = 4;
        vector<vector<pair<int, int>>> adj(n);
        auto add_u = [&](int u, int v, int w) { adj[u].push_back({v, w}); adj[v].push_back({u, w}); };
        add_u(0, 1, 10); add_u(0, 2, 6); add_u(0, 3, 5); add_u(1, 3, 15); add_u(2, 3, 4);

        displayUndirectedAdj(n, adj);
        int start;
        cout << "Enter Start Node for Prim's (0-3): "; cin >> start;
        if (start < 0 || start >= n) return;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        vector<int> key(n, INT_MAX);
        vector<bool> inMST(n, false);
        int total_weight = 0;

        key[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            int u = pq.top().second;
            int w = pq.top().first;
            pq.pop();
            if (inMST[u]) continue;
            inMST[u] = true;
            total_weight += w;
            for (auto& edge : adj[u]) {
                if (!inMST[edge.first] && edge.second < key[edge.first]) {
                    key[edge.first] = edge.second;
                    pq.push({key[edge.first], edge.first});
                }
            }
        }
        cout << "Total MST Weight (Prim's): " << total_weight << endl;
    }

    int findParent(int i, vector<int>& parent) {
        if (parent[i] == i) return i;
        return parent[i] = findParent(parent[i], parent);
    }

    void kruskals() {
        int n = 4;
        vector<Edge> edges = { {0,1,10}, {0,2,6}, {0,3,5}, {1,3,15}, {2,3,4} };
        cout << "\n--- Kruskal's MST ---" << endl;
        sort(edges.begin(), edges.end(), [](Edge a, Edge b) { return a.weight < b.weight; });
        vector<int> parent(n);
        for (int i = 0; i < n; i++) parent[i] = i;

        int mst_weight = 0;
        for (auto& e : edges) {
            int u = findParent(e.src, parent);
            int v = findParent(e.dest, parent);
            if (u != v) {
                cout << "Included: " << e.src << "-" << e.dest << " (" << e.weight << ")" << endl;
                mst_weight += e.weight;
                parent[u] = v;
            }
        }
        cout << "Total MST Weight (Kruskal's): " << mst_weight << endl;
    }
};

int main() {
    GreedyAlgorithms ga;
    int choice;
    do {
        cout << "\n--- Greedy Algorithms Menu ---";
        cout << "\n1. Selection Sort\n2. FCFS Scheduling\n3. Dijkstra (Directed)\n4. Prim's (Undirected)\n5. Kruskal's (Undirected)\n6. Exit\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: ga.selectionSort(); break;
            case 2: ga.fcfs(); break;
            case 3: ga.dijkstra(); break;
            case 4: ga.prims(); break;
            case 5: ga.kruskals(); break;
            case 6: break;
            default: cout << "Invalid choice.";
        }
    } while (choice != 6);
    return 0;
}