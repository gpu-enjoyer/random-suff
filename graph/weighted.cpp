
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


struct Edge {
    int from, to, cost;
    Edge(int v_from, int v_to, int cost)
        : from(v_from), to(v_to), cost(cost) {};
};


// Directed weighted graph

struct Graph_W {

    // adj_list[v_from]  ==  { (v_from, v_to, cost) }
    vector<vector<Edge>> adj_list;
    
    Graph_W() = default;
    Graph_W(const int v_num) : adj_list(vector<vector<Edge>>(v_num)) {};

    // Add directed edge
    void add_e(const int v_from, const int v_to, const int cost) {
        adj_list[v_from].push_back(Edge(v_from, v_to, cost));
    }

    //* Поддерживать инварианту:
    //*  только одна вершина с некоторыми (from, to)
    void demo() {
        *this = Graph_W(3);
        add_e(0, 1, 1);   //  0 -> 1       {1}
        add_e(1, 2, 1);   //       1 -> 2  {1}
        add_e(0, 2, 3);   //  0 ->   -> 2  {3}
    }

    // Add undirected edge
    void add_ue(const int v0, const int v1, const int cost) {
        add_e(v0, v1, cost);
        add_e(v1, v0, cost);
    }

    void demo_undirected() {
        *this = Graph_W(3);
        add_ue(0, 1, 1);  // 0 - 1      {1}
        add_ue(1, 2, 2);  //     1 - 2  {2}
        add_ue(2, 0, 3);  // 0 -   - 2  {3}
    }


    // int v_num();

    //!
    bool is_undirected() {
        // vector<bool> has_checked(v_num(), false);
        for (const vector<Edge>& edges : adj_list) //!
            for (const Edge& e : edges)
                for (const Edge& ee: adj_list[e.to])
                    if (ee.to == e.from) {
                        if (ee.cost == e.cost)
                            continue; //!
                        else
                            return false;
                    }
        return true;
    }

    void dijkstra(const int start, T_dijkstra& T) {
    
        // {path, vertex}
        using Pair = pair<int, int>;
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

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

    vector<int> prim();
    vector<int> kruskal();
};

ostream& operator<<(ostream& os, const Graph_W& g) {
    os << "Graph_W\n\n";
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

    // !
    cout << "gg is " << (gg.is_undirected() ? "UNdirected" : "directed") << "\n\n";

    return 0;
}
