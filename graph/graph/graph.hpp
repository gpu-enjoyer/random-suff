
#pragma once

#include "../traversal/traversal.hpp"  // T_topsort + T_bfs
#include <iostream>                    // operator<<
#include <vector>

using namespace std;


// Directed unweighted graph
class Graph
{
    private:

        vector<vector<int>> adj_;

        // DAG:    O(Vi + Ei)
        // Cyclic: O(Vi + Ei*Vi)
        void topsort_(T_topsort& t, int v);

    public:

        const vector<vector<int>>& adj = adj_;
        int v_num() { return adj.size(); }

        Graph(const Graph& other) : adj_(other.adj_) {}
        Graph& operator=(const Graph& other) { adj_ = other.adj_; return *this; }
        Graph() = default;
        Graph(int v_num) : adj_(vector<vector<int>>(v_num)) {}

        void add_v() { adj_.emplace_back(); }
        void add_e(const int v1, const int v2);
        void demo();
        void demo_cyclic();

        // 1-st launch: O(V^2)
        // Amortized:   O(V + E)
        void bfs(T_bfs& t, const int root);

        // O(V^2 + EV)
        void bfs(T_bfs& t);

        // DAG:    O(V + E)
        // Cyclic: O(V + EV)
        void topsort(T_topsort& t);
};

ostream& operator<<(ostream& os, const Graph& g);
