
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <climits>

using namespace std;

template<typename T> using vec = vector<T>;
using Pair = pair<int, int>; // dijkstra


// Edges

struct Edge {
    int from, to, cost;
    Edge()
        : from(-1), to(-1), cost(-1) {}
    Edge(int v_from, int v_to, int cost)
        : from(v_from), to(v_to), cost(cost) {}
};

struct Edge_greater {
    bool operator() (const Edge& e, const Edge& ee) const;
};

bool edge_shorter(const Edge& e, const Edge& ee);


// Traversal for Dijkstra's algorithm

struct T_dijkstra {
    static constexpr int inf = INT_MAX;
    vec<vec<int>> dist;
    vec<bool> done;
    void reset(const int v_num);
    void reset(const int v_num, const int start);
};

ostream& operator<<(ostream& os, const T_dijkstra& t);


// Traversal for Kruskal's algorithm

struct T_kruskal {
    static constexpr int v0 = -1;
    vec<Edge> edges;
    vec<int>  parent;
    T_kruskal(const int v_num);
    void reset(const vec<vec<Edge>>& adj_list);
    int  find(const int v);
    bool attach(const Edge& e);
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
        string                name = "_empty";

        Graph_W() = default;
        Graph_W(Graph_W&& g);
        Graph_W& operator=(Graph_W&& g);
        explicit Graph_W(const int v_num, const string& name);

        void add_e(const int from, const int to, const int cost);
        void add_e(const Edge& e);
        void demo(const string& name);

        void add_ue(const int from, const int to, const int cost);
        void add_ue(const Edge& e);
        void demo_undirected(const string& name);

        bool is_undirected() const;

        void dijkstra(T_dijkstra& t, const int start);
        void dijkstra(T_dijkstra& t);

        Graph_W prim(const int start);

        Graph_W kruskal();

};

ostream& operator<<(ostream& os, const Graph_W& g);
