
#pragma once

#include <vector>
#include <queue>
#include <iostream>

using namespace std;


struct traversal {
    vector<int> path;
    vector<bool> marked;
    traversal() = default;
    void reset(int v_num);
};

struct traversal_dfs : traversal {
    vector<int> t_in, t_out;
    int timer;
    void reset(int v_num);
};

struct traversal_bfs : traversal
{
    vector<int> dist;
    queue<int> q;
    void reset(int v_num);
};


class graph
{
    private:

        vector<vector<int>> adj_;
        void dfs_(traversal_dfs& tr, int v);

    public:

        const vector<vector<int>>& adj = adj_;
        int v_num() { return adj.size(); }

        graph(const graph& other) : adj_(other.adj_) {}
        graph& operator=(const graph& other) { adj_ = other.adj_; return *this; }
        graph() = default;
        graph(int v_num) : adj_(vector<vector<int>>(v_num)) {}

        void add_v() { adj_.emplace_back(); }
        void add_e(const int v1, const int v2);
        void demo();

        void dfs(traversal_dfs& tr);
        void topsort(traversal_dfs& tr);
        
        void bfs(traversal_bfs& tr);
    };

template <typename T> ostream& operator<<(ostream& os, const vector<T>& a);
ostream& operator<<(ostream& os, const graph& g);
