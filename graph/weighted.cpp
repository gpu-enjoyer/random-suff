
#include <string>
#include <vector>
#include <queue>  // dijkstra, prim
#include <iostream>
#include <climits>

using namespace std;

template<typename T> using vec = vector<T>;
using Pair = pair<int, int>; // dijkstra



// Traversal for Dijkstra's algorithm

struct T_dijkstra {
    static constexpr int inf = INT_MAX;
    vec<vec<int>> dist;
    vec<bool>     done;
    void reset(const int v_num) {
        dist.assign(v_num, vec<int>(v_num, inf));
        done.assign(v_num, false);
    }
    void reset(const int v_num, const int start) {
        if (dist.size() != v_num)
            reset(v_num);
        done[start] = true;
        dist[start].assign(v_num, inf);
    }
};

ostream& operator<<(ostream& os, const T_dijkstra& t) {
    os << "dijkstra\n   ";

    for (int v = 0; v < t.dist.size(); ++v)
        os << v << ' ';

    for (int i = 0; i < t.dist.size(); ++i)
        if (t.done[i]) {
            os << "\n " << i << " ";
            for (int d : t.dist[i])
                os << (d == t.inf ? "· " : to_string(d) + " ");
        }

    return os << "\n\n";
}


struct Edge {
    int from, to, cost;
    Edge()
        : from(-1), to(-1), cost(-1) {}
    Edge(int v_from, int v_to, int cost)
        : from(v_from), to(v_to), cost(cost) {}
};

struct Edge_greater {
    bool operator() (const Edge& e, const Edge& ee) const {
        return e.cost > ee.cost;
    }
};

// Directed weighted graph

class Graph_W {

private:

    vec<vec<Edge>> adj_list_; // adj_list[from] -> { (from, to, cost) }
    vec<vec<bool>> has_edge_; // has_edge[from][to] // todo: unordered_set

public:

    // Warning: copying will cause problems
    const vec<vec<Edge>>& adj_list = adj_list_;
    const vec<vec<bool>>& has_edge = has_edge_;

    string name = "__empty";

    Graph_W() = default;
    Graph_W(Graph_W&& g) {
        adj_list_ = std::move(g.adj_list_);
        has_edge_ = std::move(g.has_edge_);
        name      = std::move(g.name);
    }
    Graph_W& operator=(Graph_W&& g) {
        adj_list_ = std::move(g.adj_list_);
        has_edge_ = std::move(g.has_edge_);
        name      = std::move(g.name);
        return *this;
    }
    explicit Graph_W(const int v_num, string name = "__no_name")
        : adj_list_(v_num),
          has_edge_(v_num, vec<bool>(v_num, false)),
          name(std::move(name)) {}


    // Add directed edge
    void add_e(const int from, const int to, const int cost) {
        if (has_edge[from][to]) {
            cout << "Graph_W \"" << name << "\": add_e("
                << from << ", " << to << ", " << cost
                << ") not complete \n";
            return;
        }
        has_edge_[from][to] = true;
        adj_list_[from].push_back(Edge(from, to, cost));
    }
    void add_e(const Edge& e) {
        add_e(e.from, e.to, e.cost);
    }

    void demo(string name = "") {
        name += " [0>1>2<0]";
        *this = Graph_W(3, name);
        add_e(0, 1, 1);
        add_e(1, 2, 1);
        add_e(0, 2, 3);
    }

    // Add undirected edge
    void add_ue(const int from, const int to, const int cost) {
        if (has_edge[from][to] || has_edge[to][from]) {
            cout << "Graph_W \"" << name << "\": add_ue("
                << from << ", " << to << ", " << cost
                << ") not complete \n";
            return;
        }
        has_edge_[from][to] = true;
        adj_list_[from].push_back(Edge(from, to, cost));
        if (from != to) {
            has_edge_[to][from] = true;
            adj_list_[to].push_back(Edge(to, from, cost));
        }
    }
    void add_ue(const Edge& e) {
        add_ue(e.from, e.to, e.cost);
    }

    void demo_undirected(string name = "") {
        name += " [0-1-2-0]";
        *this = Graph_W(3, name);
        add_ue(0, 1, 1);
        add_ue(1, 2, 2);
        add_ue(2, 0, 3);
    }


    bool is_undirected() const {
        vec<bool> checked(has_edge.size(), false);
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


    void dijkstra(T_dijkstra& t, const int start) {
    
        // {path, vertex}
        priority_queue<Pair, vec<Pair>, greater<Pair>> pq;

        t.reset(adj_list.size(), start);

        vec<int>& d = t.dist[start];
    
        pq.push({0, start});
        d[start] = 0;
    
        while (!pq.empty()) {
            Pair P = pq.top();
            pq.pop();
            if (P.first > d[P.second])
                continue;
            for (const Edge& E : adj_list[P.second]) {
                int path = d[P.second] + E.cost;
                if (path < d[E.to]) {
                    d[E.to] = path;
                    pq.push({path, E.to});
                }
            }
        }
    }

    void dijkstra(T_dijkstra& t) {
        for (int i = 0; i < adj_list.size(); ++i)
            dijkstra(t, i);
    }


    //* Sketch

    // Growing a tree.
    //  Traverses only one connected component.
    Graph_W prim(const int start)
    {
        if (!is_undirected())
            throw("Graph_W \"" + name + "\"" + "is directed. prim() not complete");
    
        priority_queue<Edge, vec<Edge>, Edge_greater> pq;
        Graph_W tree(adj_list.size(), name + " -> prim(" + to_string(start) +")");
        vec<bool> in_tree(adj_list.size(), false);
        int v_num = 0;

        //* Инициализация

        for (const Edge& e : adj_list[start])
            pq.push(e);

        //* Суть алгоритма
        while(!pq.empty())
        {
            // Завершится, когда будут собраны все вершины либо закончится куча
            if (v_num >= tree.adj_list.size())
                break;

            Edge e = pq.top();
            pq.pop();

            if (in_tree[e.from] && in_tree[e.to])
                continue;
            else
            {
                v_num += 1;

                tree.add_ue(e);
                in_tree[e.from] = true;
                in_tree[e.to] = true;

                for (Edge ee : adj_list[e.from])
                    if (!in_tree[ee.to])
                        pq.push(ee);
            }
        }

        return tree;
    }


    // Growing a forest.
    //  struct DSU (Disjoint-Set Unit): find + union.
    //   Capable of traversing a disconnected graph.
    Graph_W kruskal(const int start) {
        Graph_W tree(adj_list.size(), "tree");
        if (!is_undirected())
            throw("Graph_W \"" + name + "\"" + "is directed. kruskal() not complete");
        // ...
        return tree;
    }
};

ostream& operator<<(ostream& os, const Graph_W& g) {
    os << (g.is_undirected() ? "Undirected" : "Directed")
        << " Graph_W \'" << g.name << "\'\n\n";
    for (int i = 0; i < g.adj_list.size(); ++i) {
        os << " " << i << " -> ";
        for (int j = 0; j < g.adj_list[i].size(); ++j) {
            const Edge& e = g.adj_list[i][j];
            os << e.to << '{' << e.cost << "} ";
        }
        os << '\n';
    }
    return os << '\n';
}


int main() {

    // Directed graph for dijkstra()
    Graph_W g;
    g.demo("g");
    cout << g;

    T_dijkstra t;
    g.dijkstra(t);
    cout << t;

    // Undirected graph for dijkstra(), prim(), kruskal()
    Graph_W gg;
    gg.demo_undirected("gg");
    cout << gg;

    T_dijkstra tt;
    gg.dijkstra(tt);
    cout << tt;

    Graph_W gg_prim = gg.prim(0);
    cout << gg_prim;

    return 0;
}
