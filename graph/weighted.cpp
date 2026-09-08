
#include <vector>
#include <queue>
#include <iostream>
#include <climits>

using namespace std;
using p = pair<int, int>;


struct Graph_W
{
    static constexpr int inf = INT_MAX;
    vector<vector<p>> adj; // {vertex_to, cost}
    
    Graph_W() = default;
    Graph_W(const int v_num) : adj(vector<vector<p>>(v_num)) {};

    void add_e(const int v1, const int v2, const int c) {
        adj[v1].push_back({v2, c});
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

    vector<int> dijkstra() {
        vector<int> dist(adj.size(), inf);
        priority_queue<p> pq;
        // ...
        return dist;
    }

    vector<int> prim();
    vector<int> kruskal();
};

ostream& operator<<(ostream& os, const Graph_W& gw) {
    os << "Graph_W\n\n";
    for (int i = 0; i < gw.adj.size(); ++i) {
        os << i << " -> ";
        for (int j = 0; j < gw.adj[i].size(); ++j)
            os << gw.adj[i][j].first << '(' << gw.adj[i][j].second << ") ";
        os << '\n';
    }
    return os << '\n';
}


int main()
{
    Graph_W gw;
    gw.demo();

    cout << gw;

    return 0;
}
