
#pragma once

#include <vector>
#include <queue>
#include <stack>
#include <iostream>

using namespace std;


struct T {
    vector<int> preorder, in, out;
    static constexpr int t0 = 1;
    int timer = t0;
    void reset(int v_num) {
        preorder.clear();
        preorder.reserve(v_num);
        in.assign(v_num, 0);
        out.assign(v_num, 0);
        timer = t0;
    }
};

struct T_bfs : T {
    vector<int> dist;
    queue<int>  q;
    void reset(int v_num) {
        T::reset(v_num);
        dist.clear();
        dist.reserve(v_num);
        q = queue<int>();
    }
};

struct T_topsort : T {
    stack<int>  topsort;
    void reset(int v_num) {
        T::reset(v_num);
        topsort = stack<int>();
    }
};


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
        bool topsort(T_topsort& T);
    };

ostream& operator<<(ostream& os, const vector<int>& a);
ostream& operator<<(ostream& os, stack<int> a);
ostream& operator<<(ostream& os, const Graph& g);
