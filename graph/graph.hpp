
#pragma once

#include <vector>
#include <iostream>

using namespace std;

class graph
{
    private:

        vector<vector<int>> adj_;
    
        void dfs_(vector<int>& traversal, vector<bool>& marked, int v);

        enum color {white, grey, black};
        void top_sort_(vector<int>& traversal, vector<color>& colors, int v);

    public:

        const vector<vector<int>>& adj = adj_;
        size_t v_num() { return adj.size(); }

        graph(const graph& other) : adj_(other.adj_) {}
        graph& operator=(const graph& other) { adj_ = other.adj_; return *this; }
        graph() = default;
        graph(int v_num) : adj_(vector<vector<int>>(v_num)) {}

        void add_v() { adj_.emplace_back(); }
        void add_e(size_t v1, size_t v2);
        void demo();

        void dfs(vector<int>& traversal);
        void top_sort(vector<int>& traversal);
    };

template <typename T> ostream& operator<<(ostream& os, const vector<T>& a);
ostream& operator<<(ostream& os, const graph& g);
