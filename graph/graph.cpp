
#include "graph.hpp"


void Graph::add_e(const int v1, const int v2) {
    if (v1 >= v_num() || v2 >= v_num())
        throw "add_e: vertex index out of range";
    adj_[v1].push_back(v2);
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


bool Graph::topsort_(T_topsort& T, int v)
{
    T.marked[v] = true;
    T.t_in[v] = T.timer++;
    T.path.push_back(v); // vector

    for (int vv : adj[v])
        if (T.marked[vv]) {
            // Если уже посетили vv и не вышли из нее,
            //  значит есть путь vv -> v
            if (T.t_out[vv] == 0)
                return false;
        } else {
            if (!topsort_(T, vv))
                return false;
        }

    // Для некоторого v все vv обработаны => выполнять v уже можно.
    //  v требуется выполнять первее u, положенного в stack ранее.
    //   ... u v ... <- topsort
    T.topsort.push(v);

    T.t_out[v] = T.timer++;
    return true;
}

bool Graph::topsort(T_topsort& T) {
    T.reset(v_num());
    for (int v = 0; v < v_num(); ++v)
        if (T.marked[v] == false)
            if (!topsort_(T, v))
                return false;
    return true;
}


void Graph::bfs(T_bfs& T, const int root) {
    if (root >= v_num())
        throw "root >= v_num";
    T.reset(v_num());
    int v = root;
    for (int i = 0; i < v_num(); ++i) {
        v = (root + i) % v_num();
        if (T.marked[v] == true)
            continue;
        T.marked[v] = true;
        T.q.push(v);
        while(!T.q.empty()) {
            v = T.q.front(); T.q.pop();
            T.path.push_back(v);
            for (int vv : adj[v])
                if (T.marked[vv] == false) {
                    T.marked[vv] = true;
                    T.q.push(vv);
                }
        }
    }
}


ostream& operator<<(ostream& os, const vector<int>& a) {
    for (int i = 0; i < a.size(); ++i)
        os << a[i] << ' ';
    return os;
}

ostream& operator<<(ostream& os, stack<int> a) {
    while (a.empty() == false) {
        os << a.top() << ' ';
        a.pop();
    }
    return os;
}

ostream& operator<<(ostream& os, const Graph& g) {
    for (int i = 0; i < g.adj.size(); ++i)
        os << i << " -> " << g.adj[i] << '\n';
    return os << '\n';
}


int main()
{
    Graph g;
    // g.demo();
    g.demo_cyclic();
    cout << g;
    
    T_topsort t_top;
    if (g.topsort(t_top))
        cout << "topsort: " << t_top.topsort << "\n";
    else
        cout << "topsort: cycle detected \n";

    T_bfs t_bfs;
    g.bfs(t_bfs, 0);
    cout << "    bfs: " << t_bfs.path << "\n\n";

    return 0;
}
