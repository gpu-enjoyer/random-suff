
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <climits>

using namespace std;


// Traversal for Dijkstra's algorithm

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


// Directed weighted graph

struct Edge {
    int to, cost;
    Edge(int vertex_to, int cost): to(vertex_to), cost(cost) {};
};

struct Graph_W {
    
    // vertex_from ->  vertex_to {cost} ...
    vector<vector<Edge>> adj;
    
    Graph_W() = default;
    Graph_W(const int v_num) : adj(vector<vector<Edge>>(v_num)) {};

    void add_e(const int from, const int to, const int cost) {
        adj[from].push_back(Edge(to, cost));
    }

    void demo() {
        *this = Graph_W(3);
        add_e(0, 1, 1);   //  0 -> 1       (1)
        add_e(1, 2, 1);   //       1 -> 2  (1)
        add_e(0, 2, 7);   //  0 ->   -> 2  (7)
    }

    void dijkstra(const int start, T_dijkstra& T) {
    
        // {path, vertex}
        using Pair = pair<int, int>;
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        T.reset(adj.size(), start);
    
        pq.push({0, T.start});
        T.dist[start] = 0;
    
        while (!pq.empty()) {
            Pair P = pq.top();
            pq.pop();
            if (T.dist[P.second] != P.first)
                continue;
            for (Edge E : adj[P.second]) {
                int path = T.dist[P.second] + E.cost;
                if (T.dist[E.to] > path) {
                    T.dist[E.to] = path;
                    pq.push({path, E.to});
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
        for (int j = 0; j < gw.adj[i].size(); ++j) {
            Edge E = gw.adj[i][j];
            os << E.to << '{' << E.cost << "} ";
        }
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
