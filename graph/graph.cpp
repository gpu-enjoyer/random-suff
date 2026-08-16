
#include "graph.hpp"
#include <queue>


void graph::add_e(
    const int v1,
    const int v2)
{
    if (v1 >= v_num() || v2 >= v_num())
        throw "add_e: vertex index out of range";
    adj_[v1].push_back(v2);
}

void graph::demo()
{
    *this = graph(7);
    this->add_e(0, 1);
    this->add_e(1, 2);
    this->add_e(1, 3);
    this->add_e(3, 2);
    this->add_e(3, 4);
    this->add_e(4, 2);
    this->add_e(5, 6);
    this->add_e(6, 4);
}


void graph::bfs(
    vector<int>& traversal)
{
    traversal.clear();
    traversal.reserve(v_num());
    vector<bool> marked(v_num(), false);
    queue<int> q;
    for (int v = 0; v < v_num(); ++v) {
        if (marked[v] == true)
            continue;
        marked[v] = true;
        q.push(v);
        while(!q.empty()) {
            v = q.front();
            q.pop();
            traversal.push_back(v);
            for (int vv : adj[v])
                if (marked[vv] == false) {
                    marked[vv] = true;
                    q.push(vv);
                }
        }
    }
}

void graph::dfs_(
    vector<int>&  traversal,
    vector<bool>& marked,
    int           v)
{
    marked[v] = true;
    for (int vv : adj[v])
        if (marked[vv] == false)
            dfs_(traversal, marked, vv);
    traversal.push_back(v);
}

void graph::dfs(
    vector<int>& traversal)
{
    traversal.clear();
    traversal.reserve(v_num());
    vector<bool> marked(v_num(), false);
    for (int v = 0; v < v_num(); ++v)
        if (marked[v] == false)
            dfs_(traversal, marked, v);
}

void graph::topsort(
    vector<int>& traversal)
{
    dfs(traversal);
    for (int v = 0; v < v_num(); ++v) {
        int vv = v_num() - 1 - v;
        if (v >= vv)
            break;
        swap(traversal[v], traversal[vv]);
    }
}


template <typename T>
ostream& operator<<(
    ostream& os,
    const vector<T>& a)
{
    for (int i = 0; i < a.size(); ++i)
        os << a[i] << ' ';
    return os;
}

ostream& operator<<(
    ostream& os,
    const graph& g)
{
    for (int i = 0; i < g.adj.size(); ++i)
        os << i << " -> " << g.adj[i] << '\n';
    return os << '\n';
}


int main()
{
    graph g;
    g.demo();
    cout << g;
    
    vector<int> traversal;

    g.bfs(traversal);
    cout << "    bfs: " << traversal << "\n";

    g.topsort(traversal);
    cout << "topsort: " << traversal << "\n\n";

    return 0;
}
