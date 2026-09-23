
#include "graph.hpp"
#include <algorithm> // reverse(cycle)


// Directed unweighted graph

void Graph::add_e(const int v1, const int v2) {
    if (v1 >= v_num() || v2 >= v_num())
        throw "add_e: vertex index out of range";
    adj_[v1].push_back(v2);
    // ++version;
}

void Graph::demo() {
    *this = Graph(7);
    add_e(0, 1);
    add_e(1, 2); add_e(1, 3);
    add_e(3, 2); add_e(3, 4);
    add_e(4, 2);
    add_e(5, 6);
    add_e(6, 4);
}

void Graph::demo_cyclic() {
    demo();
    add_e(3, 0);
    add_e(2, 1);
    add_e(4, 6);
}


// Breadth-First Search.
//  O(V + E) amort., O(V^2) on 1-st launch
void Graph::bfs(T_bfs& t, int v) {

    if (v >= v_num())
        throw out_of_range("root >= v_num");

    t.reset(v_num(), v); // O(V) amort., O(V^2) on 1-st launch

    vector<int>& dist = t.dist[v];
    dist[v] = 0;
    t.q.push(v);

    while(!t.q.empty()) // O(V)
    {
        v = t.q.front();
        t.q.pop();

        for (int vv : adj[v]) // O(E/V) amort.
        {
            if (dist[vv] == t.dist0) {
                dist[vv] = dist[v] + 1;
                t.q.push(vv);
            }
        }
    }
}

// O(V^2 + EV)
void Graph::bfs(T_bfs& t) {
    t.reset(v_num());                  // O(V^2)
    for (int v = 0; v < v_num(); ++v)  // O(V)
        bfs(t, v);                     //  O(V + E)
}


// Topological Sort
//  Vi := "Vertices visited during this call"
//  Ei := "Edges considered during this call"
//   DAG: O(Vi + Ei)
//!  Cyclic graph:
//!   O(Vi + Ei + Ei*Vi) = O(Vi + Ei*Vi)
void Graph::topsort_(T_topsort& t, int v) {

    t.in[v] = t.timer++;

    for (int vv : adj[v]) // Σ = E = O(E)
    {
        // DFS. Visit any vertex 1 time
        if (t.in[vv] == t.t0)
        // 'false' => (*), 'true' => O(V)
        {
            t.parent[vv] = v;
            topsort_(t, vv);  // Depth ≤ V = O(V)
        }
        // (*) Cycle detected
        else if (t.out[vv] == t.t0) //! Σ (grey vertices) ≤ O(E)
        {
            vector<int> cycle;
            // Get cycle
            for (int u = v; u != vv; u = t.parent[u]) //! ≤ O(V)
                cycle.push_back(u);
            cycle.push_back(vv);
            reverse(cycle.begin(), cycle.end());
            t.cycles.push_back(cycle);
        }
    }

    t.out[v] = t.timer++;
    t.topsort.push(v);
}

//  DAG: O(V + E)
//! Cyclic graph:
//!  O(V + ΣVi + ΣEiVi) ≤ O(V + E * ΣVi) = O(V + EV)
void Graph::topsort(T_topsort& t) {
    t.reset(v_num());                 // O(V)
    for (int v = 0; v < v_num(); ++v)
        if (t.in[v] == t.t0)          // Σ ≤ V = O(V)
            topsort_(t, v);
    if (t.cycles.size() != 0)
        t.topsort = stack<int>();     // O(1)
}


// operator<<

ostream& operator<<(ostream& os, const vector<int>& a) {
    for (int i = 0; i < a.size(); ++i)
        os << a[i] << ' ';
    return os;
}

ostream& operator<<(ostream& os, const Graph& g) {
    os << "Graph\n\n";
    for (int i = 0; i < g.adj.size(); ++i)
        os << i << " -> " << g.adj[i] << '\n';
    return os;
}
