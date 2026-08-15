
#include "graph.hpp"


void graph::add_e(
    size_t v1,
    size_t v2)
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


void graph::dfs_(
    vector<int>& traversal,
    vector<bool>& marked,
    int v)
{
    marked[v] = true;
    traversal.push_back(v);
    for (int vv : adj[v])
        if (marked[vv] == false)
            dfs_(traversal, marked, vv);
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


void graph::top_sort_(
    vector<int>& traversal,
    vector<color>& colors,
    int v)
{
    colors[v] = grey;
    for (int vv : adj[v])
        switch (colors[vv]) {
            case grey:
                throw "top_sort: cycle: "
                    + to_string(v) + " ... "
                    + to_string(vv);
            case white:
                top_sort_(traversal, colors, vv);
            case black:
                ;
        }
    colors[v] = black;
    traversal.push_back(v);
}

void graph::top_sort(
    vector<int>& traversal)
{
    traversal.clear();
    traversal.reserve(v_num());
    vector<color> colors(v_num(), white);
    for (int v = 0; v < v_num(); ++v)
        if (colors[v] == white)
            top_sort_(traversal, colors, v);
    for (int v = 0; v < v_num(); ++v) {
        if (v >= v_num() - 1 - v) break;
        swap(traversal[v], traversal[v_num() - 1 - v]);
    }
}


template <typename T>
ostream& operator<<(
    ostream& os,
    const vector<T>& a)
{
    for (size_t i = 0; i < a.size(); ++i)
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

    g.dfs(traversal);
    cout << "dfs: " << traversal << "\n";

    g.top_sort(traversal);
    cout << "top_sort: " << traversal << "\n\n";
    
    return 0;
}
