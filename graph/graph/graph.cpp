
#include "graph.hpp"


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
}


void Graph::bfs(T_bfs& T, int v) {
    if (v >= v_num())
        throw out_of_range("root >= v_num");
    T.reset(v_num(), v);
    vector<int>& dist = T.dist[v];
    dist[v] = 0;
    T.q.push(v);
    while(!T.q.empty()) {
        v = T.q.front();
        T.q.pop();
        for (int vv : adj[v])
            if (dist[vv] == T.dist0) {
                dist[vv] = dist[v] + 1;
                T.q.push(vv);
            }
    }
}

void Graph::bfs(T_bfs& T) {
    T.reset(v_num());
    for (int v = 0; v < v_num(); ++v)
        bfs(T, v);
}


bool Graph::topsort_(T_topsort& T, int v) {
    T.in[v] = T.timer++;
    for (int vv : adj[v])
        if (T.in[vv] == T.t0) {
            // Еще не посещали vv
            if (!topsort_(T, vv))
                return false;
        }
        else {
            // Посетили и не обработали vv => есть путь vv -> v
            if (T.out[vv] == T.t0)
                return false;
                // TODO: Сохранять путь цикла, продолжать обход
        }
    // Все vv обработаны => выполнять v уже можно.
    //  v требуется выполнять первее u, положенного в stack ранее.
    //   [ .. u v .. ] <- topsort
    T.topsort.push(v);
    T.out[v] = T.timer++;
    return true;
}

void Graph::topsort(T_topsort& T) {
    T.reset(v_num());
    for (int v = 0; v < v_num(); ++v)
        if (T.in[v] == T.t0)
            if (!topsort_(T, v))
                T.cycle = true;
}


// operator<<

ostream& operator<<(ostream& os, const vector<int>& a) {
    for (int i = 0; i < a.size(); ++i)
        os << a[i] << ' ';
    return os;
}

ostream& operator<<(ostream& os, const Graph& g) {
    for (int i = 0; i < g.adj.size(); ++i)
        os << i << " -> " << g.adj[i] << '\n';
    return os;
}
