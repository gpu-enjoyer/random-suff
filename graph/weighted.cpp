
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <climits>

using namespace std;

template<typename T>
using vec = vector<T>;
using Pair = pair<int, int>; // dijkstra



// Traversal for Dijkstra's algorithm

struct T_dijkstra {
    static constexpr int inf = INT_MAX;
    vec<int> dist;
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


struct Edge {
    int from, to, cost;
    Edge(int v_from, int v_to, int cost)
        : from(v_from), to(v_to), cost(cost) {}
};


// Directed weighted graph

struct Graph_W {

    vec<vec<Edge>> adj_list; // adj_list[from] -> { (from, to, cost) }
    vec<vec<bool>> has_edge; // has_edge[from][to]

    Graph_W() = default;
    Graph_W(const int v_num) {
        adj_list = vec<vec<Edge>>(v_num);
        has_edge = vec<vec<bool>>(v_num, vec<bool>(v_num, false));
    }

    // Add directed edge
    void add_e(const int from, const int to, const int cost) {
        if (has_edge[from][to]) {
            cout << "  directed edge("
                << from << to << cost << ") was not added";
            return;
        }
        has_edge[from][to] = true;
        adj_list[from].push_back(Edge(from, to, cost));
    }

    void demo() {
        *this = Graph_W(3);
        add_e(0, 1, 1);   //  0 -> 1       {1}
        add_e(1, 2, 1);   //       1 -> 2  {1}
        add_e(0, 2, 3);   //  0 ->   -> 2  {3}
    }

    // Add undirected edge
    void add_ue(const int from, const int to, const int cost) {
        if (has_edge[from][to] || has_edge[to][from]) {
            cout << "undirected edge("
                << from << to << cost << ") was not added";
            return;
        }
        has_edge[from][to] = true;
        adj_list[from].push_back(Edge(from, to, cost));
        has_edge[to][from] = true;
        adj_list[to].push_back(Edge(to, from, cost));
    }

    void demo_undirected() {
        *this = Graph_W(3);
        add_ue(0, 1, 1);  // 0 - 1      {1}
        add_ue(1, 2, 2);  //     1 - 2  {2}
        add_ue(2, 0, 3);  // 0 -   - 2  {3}
    }


    bool is_undirected() const {
        vec<bool> checked(adj_list.size(), false);
        for (int i = 0; i < has_edge.size(); ++i) {
            for (int j = 0; j < has_edge.size(); ++j) {
                if (checked[j])
                    continue;
                if (has_edge[i][j] != has_edge[j][i])
                    return false;
                int cost_i_j = 0;
                int cost_j_i = 0;
                for (const Edge& e : adj_list[i])
                    if (e.to == j) {
                        cost_i_j = e.cost;
                        break;
                    }
                for (const Edge& e : adj_list[j])
                    if (e.to == i) {
                        cost_j_i = e.cost;
                        break;
                    }
                if (cost_i_j != cost_j_i)
                    return false;
            }
            checked[i] = true;
        }
        return true;
    }

    void dijkstra(const int start, T_dijkstra& T) {
    
        // {path, vertex}
        priority_queue<Pair, vec<Pair>, greater<Pair>> pq;

        T.reset(adj_list.size(), start);
    
        pq.push({0, start});
        T.dist[start] = 0;
    
        while (!pq.empty()) {
            Pair P = pq.top();
            pq.pop();
            if (P.first > T.dist[P.second])
                continue;
            for (const Edge& E : adj_list[P.second]) {
                int path = T.dist[P.second] + E.cost;
                if (path < T.dist[E.to]) {
                    T.dist[E.to] = path;
                    pq.push({path, E.to});
                }
            }
        }
    }

    vec<int> prim();
    vec<int> kruskal();
};

ostream& operator<<(ostream& os, const Graph_W& g) {
    os << (g.is_undirected() ? "Undirected" : "Directed")
        << " Graph_W\n\n";
    for (int i = 0; i < g.adj_list.size(); ++i) {
        os << i << " -> ";
        for (int j = 0; j < g.adj_list[i].size(); ++j) {
            Edge E = g.adj_list[i][j];
            os << E.to << '{' << E.cost << "} ";
        }
        os << '\n';
    }
    return os << '\n';
}


int main() {

    // Directed graph for Dijkstra()
    Graph_W g;
    g.demo();
    cout << g;

    T_dijkstra t_d;
    g.dijkstra(0, t_d);
    cout << t_d;

    // Undirected graph for Prim() and Kruskal()
    Graph_W gg;
    gg.demo_undirected();
    cout << gg;

    return 0;
}
