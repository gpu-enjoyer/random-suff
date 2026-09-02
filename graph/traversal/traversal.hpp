
#pragma once

#include <queue>     // T_bfs
#include <stack>     // T_topsort
#include <iostream>  // operator<<
#include <vector>

using namespace std;


// Traversal for BFS
struct T_bfs
{
    vector<vector<int>> dist;
    queue<int>          q;
    
    static constexpr int dist0 = -1;

    void reset(const int v_num);
    void reset(const int v_num, const int v);
};


// Traversal for Topological Sort
struct T_topsort
{
    vector<int> in, out;
    stack<int>  topsort;
    int         timer;
    bool        cycle;  // TODO: vector<vector>
    
    // in, out default value
    static constexpr int t0 = -1;

    void reset(const int v_num);
};


ostream& operator<<(ostream& os, const T_bfs& t);
ostream& operator<<(ostream& os, const T_topsort& t);
