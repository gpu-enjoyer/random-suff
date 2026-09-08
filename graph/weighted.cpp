
#include <vector>
#include <queue>
#include <iostream>
#include <climits>

using namespace std;
using p = pair<int, int>;


struct Graph_W
{
    static constexpr int inf = INT_MAX;
    vector<vector<p>> adj; // {cost, vertex_to}
    
    Graph_W() = default;
    Graph_W(const int v_num) : adj(vector<vector<p>>(v_num)) {};

    void add_e(const int v0, const int v1, const int c) {
        adj[v0].push_back({c, v1});
    }

    void demo() {
        *this = Graph_W(7);
        add_e(0, 1, 1);
        add_e(1, 2, 1); add_e(1, 3, 1);
        add_e(3, 2, 1); add_e(3, 4, 1);
        add_e(4, 2, 1);
        add_e(5, 6, 1);
        add_e(6, 4, 1);
    }

    vector<int> dijkstra(const int start)
    {
        vector<int> dist(adj.size(), inf);
        priority_queue<p, vector<p>, greater<p>> pq; // {path, vertex}
    
        pq.push({0, start});
        dist[start] = 0;
    
        while (!pq.empty()) {
            p v = pq.top();
            pq.pop();
            if (dist[v.second] != v.first)
                continue;
            for (p vv : adj[v.second]) {
                int path = dist[v.second] + vv.first;
                if (dist[vv.second] > path) {
                    dist[vv.second] = path;
                    pq.push({path, vv.second});
                }
            }
        }
    
        return dist;
    }

    vector<int> prim();
    vector<int> kruskal();
};


ostream& operator<<(ostream& os, const vector<int>& a) {
    for (int i = 0; i < a.size(); ++i)
        os << a[i] << ' ';
    return os << '\n';
}

ostream& operator<<(ostream& os, const Graph_W& gw) {
    os << "Graph_W\n\n";
    for (int i = 0; i < gw.adj.size(); ++i) {
        os << i << " -> ";
        for (int j = 0; j < gw.adj[i].size(); ++j)
            os << gw.adj[i][j].second << '(' << gw.adj[i][j].first << ") ";
        os << '\n';
    }
    return os << '\n';
}


int main()
{
    Graph_W gw;
    gw.demo();

    cout << gw;

    vector<int> dist = gw.dijkstra(0);
    cout << dist << '\n';

    return 0;
}
