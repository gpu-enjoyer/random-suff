
#include "graph.hpp"


void traversal::reset(int v_num) {
    path.clear();  path.reserve(v_num);
    marked.assign(v_num, false);
}

void traversal_dfs::reset(int v_num) {
    traversal::reset(v_num);
    t_in.clear();  t_in.reserve(v_num);
    t_out.clear(); t_out.reserve(v_num);
    timer = 0;
}

void traversal_bfs::reset(int v_num) {
    traversal::reset(v_num);
    dist.clear(); dist.reserve(v_num);
}


void graph::add_e(const int v1, const int v2) {
    if (v1 >= v_num() || v2 >= v_num())
        throw "add_e: vertex index out of range";
    adj_[v1].push_back(v2);
}

void graph::demo() {
    *this = graph(7);
    add_e(0, 1);
    add_e(1, 2); add_e(1, 3);
    add_e(3, 2); add_e(3, 4);
    add_e(4, 2);
    add_e(5, 6);
    add_e(6, 4);
}


void graph::dfs_(traversal_dfs& tr, int v) {
    tr.marked[v] = true;
    for (int vv : adj[v])
        if (tr.marked[vv] == false)
            dfs_(tr, vv);
    tr.path.push_back(v);
}

void graph::dfs(traversal_dfs& tr) {
    tr.reset(v_num());
    for (int v = 0; v < v_num(); ++v)
        if (tr.marked[v] == false)
            dfs_(tr, v);
}

void graph::topsort(traversal_dfs& tr) {
    dfs(tr);
    for (int v = 0; v <= v_num() - 1; ++v) {
        int vv = v_num() - 1 - v;
        if (v >= vv) break;
        swap(tr.path[v], tr.path[vv]);
    }
}


void graph::bfs(traversal_bfs& tr) {
    tr.reset(v_num());
    for (int v = 0; v < v_num(); ++v) {
        if (tr.marked[v] == true)
            continue;
        tr.marked[v] = true;
        tr.q.push(v);
        while(!tr.q.empty()) {
            v = tr.q.front();
            tr.q.pop();
            tr.path.push_back(v);
            for (int vv : adj[v])
                if (tr.marked[vv] == false) {
                    tr.marked[vv] = true;
                    tr.q.push(vv);
                }
        }
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
    
    traversal_dfs tr_dfs;
    g.topsort(tr_dfs);
    cout << "topsort: " << tr_dfs.path << "\n";

    traversal_bfs tr_bfs;
    g.bfs(tr_bfs);
    cout << "    bfs: " << tr_bfs.path << "\n\n";

    return 0;
}
