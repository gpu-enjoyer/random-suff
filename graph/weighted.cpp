
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <climits>

using namespace std;
using p = pair<int, int>;


struct T_dijkstra {
    static constexpr int inf = INT_MAX;
    vector<int> dist;
    int start;
    void reset(const int v_num, const int start){
        this->start = start;
        dist.assign(v_num, inf);
    }
};

ostream& operator<<(ostream& os, const T_dijkstra& t) {
    os << "dijkstra\n\n  ";
    for (int v = 0; v < t.dist.size(); ++v)
        os << v << ' ';
    os << '\n' << t.start << ' ';
    for (int v = 0; v < t.dist.size(); ++v)
        os << (t.dist[v] == t.inf ? "· " : to_string(t.dist[v]) + " ");
    return os << "\n\n";
}


struct Graph_W {

    vector<vector<p>> adj; // {cost, vertex_to}
    
    Graph_W() = default;
    Graph_W(const int v_num) : adj(vector<vector<p>>(v_num)) {};

    void add_e(const int v0, const int v1, const int c) {
        adj[v0].push_back({c, v1});
    }

    void demo() {
        *this = Graph_W(3);
        add_e(0, 1, 1);   //  0 -> 1       (1)
        add_e(1, 2, 1);   //       1 -> 2  (1)
        add_e(0, 2, 7);   //  0 ->   -> 2  (7)
    }

    void dijkstra(const int start, T_dijkstra& t) {

        priority_queue<p, vector<p>, greater<p>> pq; // {path, vertex}
        t.reset(adj.size(), start);
    
        pq.push({0, t.start});
        t.dist[start] = 0;
    
        while (!pq.empty()) {
            p v = pq.top();
            pq.pop();
            if (t.dist[v.second] != v.first)
                continue;
            for (p vv : adj[v.second]) {
                int path = t.dist[v.second] + vv.first;
                if (t.dist[vv.second] > path) {
                    t.dist[vv.second] = path;
                    pq.push({path, vv.second});
                }
            }
        }
    }

    vector<int> prim();
    vector<int> kruskal();
};

ostream& operator<<(ostream& os, const Graph_W& gw) {
    os << "Graph_W\n\n";
    for (int i = 0; i < gw.adj.size(); ++i) {
        os << i << " -> ";
        for (int j = 0; j < gw.adj[i].size(); ++j)
            os << gw.adj[i][j].second << '{' << gw.adj[i][j].first << "} ";
        os << '\n';
    }
    return os << '\n';
}


int main() {

    Graph_W gw;
    gw.demo();
    cout << gw;

    T_dijkstra t;
    gw.dijkstra(0, t);
    cout << t;

    return 0;
}
