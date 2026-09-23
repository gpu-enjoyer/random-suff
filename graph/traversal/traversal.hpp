
#pragma once

#include <queue>     // T_bfs
#include <stack>     // T_topsort
#include <iostream>  // operator<<
#include <vector>

using namespace std;


// Traversal for BFS
struct T_bfs {
    static constexpr int dist0 = -1;
    vector<vector<int>>  dist;
    queue<int>           q;
    void reset(const int v_num);              // O(V^2)
    void reset(const int v_num, const int v); // O(V) amort.
};


// Traversal for Topological Sort
struct T_topsort {
    static constexpr int t0 = -1;  // in, out
    vector<vector<int>>  cycles;
    vector<int>          in, out, parent;
    stack<int>           topsort;
    int                  timer;
    void reset(const int v_num); // O(V)
};


ostream& operator<<(ostream& os, const T_bfs& t);
ostream& operator<<(ostream& os, const T_topsort& t);
