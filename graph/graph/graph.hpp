
#pragma once

#include <vector>
#include <queue>
#include <stack>
#include <iostream>

using namespace std;


struct T_bfs
{
    static constexpr int dist0 = -1;
    vector<vector<int>> dist;
    queue<int>  q;
    void reset(const int v_num) {
        dist.assign(v_num, vector<int>(v_num, dist0));
        q = queue<int>();
    }
    void reset(const int v_num, const int v) {
        if (dist.size() != v_num)
            reset(v_num);
        dist[v].assign(v_num, dist0);
        q = queue<int>();
    }
};

struct T_topsort
{
    static constexpr int t0 = -1;
    vector<int> in, out;
    stack<int>  topsort;
    int timer;
    void reset(const int v_num) {
        in.assign(v_num, t0);
        out.assign(v_num, t0);
        topsort = stack<int>();
        timer = 0;
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
        void bfs(T_bfs& T);
        bool topsort(T_topsort& T);
    };

std::string grey(string s);
std::string grey(int digit);

ostream& operator<<(ostream& os, const vector<int>& a);
ostream& operator<<(ostream& os, stack<int> a);

ostream& operator<<(ostream& os, const Graph& g);

ostream& operator<<(ostream& os, const T_bfs& t);
ostream& operator<<(ostream& os, const T_topsort& t);
