
#pragma once

#include <vector>
#include <iostream>

using namespace std;

class graph
{
    private:

        vector<vector<int>> adj_;
        void dfs_(vector<int>& traversal, vector<bool>& marked, int v);

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

        void dfs(vector<int>& traversal);
        void topsort(vector<int>& traversal);
    };

template <typename T> ostream& operator<<(ostream& os, const vector<T>& a);
ostream& operator<<(ostream& os, const graph& g);
