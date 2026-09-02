
#pragma once

#include "../traversal/traversal.hpp"  // T_topsort + T_bfs
#include <iostream>                    // operator<<
#include <vector>

using namespace std;


class Graph
{
    private:

        vector<vector<int>> adj_;
        bool topsort_(T_topsort& T, int v);

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

        void bfs(T_bfs& T, const int root);
        void bfs(T_bfs& T);
        void topsort(T_topsort& T);
};

ostream& operator<<(ostream& os, const Graph& g);
