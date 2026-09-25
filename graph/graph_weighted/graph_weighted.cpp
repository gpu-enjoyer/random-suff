
#include "graph_weighted.hpp"

#include <queue>  // dijkstra, prim
#include <algorithm> // kruskal: sort()


// Traversal for Dijkstra's algorithm
// O(V^2)
void T_dijkstra::reset(const int v_num) {
        dist.assign(v_num, vec<int>(v_num, inf));
        done.assign(v_num, false);
}
// O(V) amort.
void T_dijkstra::reset(const int v_num, const int start) {
        if (dist.size() != v_num)
            reset(v_num);
        done[start] = true;
        dist[start].assign(v_num, inf);
}
ostream& operator<<(ostream& os, const T_dijkstra& t) {
    os << "dijkstra\n   ";

    for (int v = 0; v < t.dist.size(); ++v)
        os << v << ' ';

    for (int i = 0; i < t.dist.size(); ++i)
        if (t.done[i]) {
            os << "\n " << i << " ";
            for (int d : t.dist[i])
                os << (d == t.inf ? "· " : to_string(d) + " ");
        }

    return os << "\n\n";
}


// Traversal for Kruskal's algorithm
bool edge_shorter(const Edge& e, const Edge& ee) {
    return e.cost < ee.cost;
}
T_kruskal::T_kruskal(const vec<vec<Edge>>& adj_list) { // O(V + E * log E)
    edges.clear();
    parent.assign(adj_list.size(), v0);
    sz.assign(adj_list.size(), 1);
    for (int i = 0; i < adj_list.size(); ++i) // O(V)
        parent[i] = i;
    for (const vec<Edge>& vec_e : adj_list) // O(E)
        for (const Edge& e : vec_e)
            if (e.to < e.from)
                edges.push_back(e);
    sort(edges.begin(), edges.end(), edge_shorter); // O(E * log E)
}
int T_kruskal::find(const int v) { // O(1) amort.
    // Path compression: p[v] = f(pp[v]) = ff(ppp[v]) = ... = v
    return parent[v] == v ? v : parent[v] = find(parent[v]);
}
bool T_kruskal::attach(const Edge& e) { // O(1) amort.
    int a = find(e.from);
    int b = find(e.to);
    if (a == b)
        return false;
    if (sz[a] < sz[b])
        swap(a, b);
    parent[b] = a;
    sz[a] += sz[b];
    return true;
}


// Directed weighted graph
Graph_W::Graph_W(Graph_W&& g) {
    adj_list_ = std::move(g.adj_list_);
    has_edge_ = std::move(g.has_edge_);
    name      = std::move(g.name);
}
Graph_W& Graph_W::operator=(Graph_W&& g) {
    adj_list_ = std::move(g.adj_list_);
    has_edge_ = std::move(g.has_edge_);
    name      = std::move(g.name);
    return *this;
}
// O(V^2)
Graph_W::Graph_W(const int v_num, const string& name = "_no_name") {
    adj_list_ = vec<vec<Edge>>(v_num);
    has_edge_ = vec<vec<bool>>(v_num, vec<bool>(v_num, false));
    this->name = name;
}


// Directed edges
void Graph_W::add_e(const int from, const int to, const int cost) {
    if (has_edge[from][to]) {
        cout << "Graph_W \"" << name << "\": add_e("
            << from << ", " << to << ", " << cost
            << ") not complete \n";
        return;
    }
    has_edge_[from][to] = true;
    adj_list_[from].push_back(Edge(from, to, cost));
}
void Graph_W::add_e(const Edge& e) {
    add_e(e.from, e.to, e.cost);
}
void Graph_W::demo(const string& name = "") {
    *this = Graph_W(3, name + " [0>1>2<0]");
    add_e(0, 1, 1);
    add_e(1, 2, 1);
    add_e(0, 2, 3);
}


// Undirected edges
void Graph_W::add_ue(const int from, const int to, const int cost) {
    if (has_edge[from][to] || has_edge[to][from]) {
        cout << "Graph_W \"" << name << "\": add_ue("
            << from << ", " << to << ", " << cost
            << ") not complete \n";
        return;
    }
    has_edge_[from][to] = true;
    adj_list_[from].push_back(Edge(from, to, cost));
    if (from != to) {
        has_edge_[to][from] = true;
        adj_list_[to].push_back(Edge(to, from, cost));
    }
}
void Graph_W::add_ue(const Edge& e) {
    add_ue(e.from, e.to, e.cost);
}
void Graph_W::demo_undirected(const string& name = "") {
    *this = Graph_W(3, name + " [0-1-2-0]");
    add_ue(0, 1, 1);
    add_ue(1, 2, 2);
    add_ue(2, 0, 3);
}


// O(E)
bool Graph_W::is_non_negative() const {
    for (const vec<Edge>& vec_e : adj_list)
        for (const Edge& e : vec_e)
            if (e.cost < 0)
                return false;
    return true;
}

// O(V + V*(V+V/2*(E/V)))
//  = O(V^2 + EV)
bool Graph_W::is_undirected() const {
    vec<bool> checked(has_edge.size(), false);       // O(V)
    for (int i = 0; i < has_edge.size(); ++i) {      // O(V)
        for (int j = 0; j < has_edge.size(); ++j)    //
        {                                            //   {
            if (checked[j])                          //     if:   Σ = V,   O(1)
                continue;                            //     else: Σ = V/2, O(E/V)
            if (has_edge[i][j] != has_edge[j][i])    //     {
                return false;                        //       got lucky
            int cost_i_j = 0;                        //
            int cost_j_i = 0;                        //
            for (const Edge& e : adj_list[i])        //       Σ = E/2V = O(E/V) amort.
                if (e.to == j) {                     //         O(1)
                    cost_i_j = e.cost;               //
                    break;                           //
                }                                    //
            for (const Edge& e : adj_list[j])        //       Σ = E/2V = O(E/V) amort.
                if (e.to == i) {                     //         O(1)
                    cost_j_i = e.cost;               //
                    break;                           //
                }                                    //
            if (cost_i_j != cost_j_i)                //       got lucky
                return false;                        //
        }                                            //     }
        checked[i] = true;                           //
    }                                                //   }
    return true;                                     //
}


//* O(V + E * log E) amort.
void Graph_W::dijkstra(T_dijkstra& t, const int start) {

    // O(E)
    if (!is_non_negative())
        throw("Graph_W \"" + name + "\"" + " contains negative weights. \n"
            + "dijkstra() did not execute \n");

    // Pair {dist, vertex}
    priority_queue<Pair, vec<Pair>, greater<Pair>> pq;

    // O(V) amort., O(V^2) on 1-st launch
    t.reset(adj_list.size(), start);

    vec<int>& dist = t.dist[start];

    pq.push({0, start});
    dist[start] = 0;
    
    //  Code below complexity:
    //*  O(E * log E) + V * O(E * log E / V)
    //*   = O(E * log E)

    while (!pq.empty()) //*  Σ = O(E)
    {
        Pair p = pq.top(); pq.pop();  //* O(log E)

        if (p.first > dist[p.second])
            continue; // O(1)

        // else (p.first <= dist[p.second])
        //  Only 1 time for any `p` with `p.second` in V

        //  Code below executed
        //*  Σ = V times

        //  Code below complexity:
        //*  O(E * log E / V)


        //  Прежде мне казалось, что можно предоставить граф
        //   с любым количеством новых меньших путей `p.first`.
        //    Это неверно.
        //
        //  В какой-то момент алгоритм в первый раз
        //   посмотрит на `v` из новой вершины `p.second`
        //    и предложит путь `p.first`.
        //
        //  Сначала расстояние `dist[v]` до вершины `v` равно `inf`,
        //   поэтому гарантировано выполнение `else`.
        //
        //  Всякий последующий раз для вершины `v` мы сможем рассмотреть 
        //   путь `p.first` только больший или равный, чем в первый раз,
        //    так как алгоритм берет из кучи самые короткие пути и `e.cost`≥ 0.

        for (const Edge& e : adj_list[p.second])  //* O(E/V) amort.
        {
            int new_dist = dist[p.second] + e.cost;
            if (new_dist < dist[e.to]) {
                dist[e.to] = new_dist;
                pq.push({new_dist, e.to});  //* O(log E)
            }
        }
    }
}

// O(V^2 + V*E*log E) amort.
void Graph_W::dijkstra(T_dijkstra& t) {
    for (int i = 0; i < adj_list.size(); ++i)
        dijkstra(t, i);
}


// Growing a tree.
//  Traverses only one connected component.
//   O(V^2 + E * log E + E * log E) =
//    O(V^2 + E * log E)
//     - Graph_W tree(v_num)  -> O(V^2)
//     - pq.top() + pq.push() -> O(E * log E)
Graph_W Graph_W::prim(const int start) {

    // // O(V^2 + EV)
    // if (!is_undirected())
    //     throw("Graph_W \"" + name + "\"" + " is directed. \n"
    //         + "prim() did not execute \n");

    priority_queue<Edge, vec<Edge>, Edge_greater> pq;

    // O(V^2)
    Graph_W   tree(adj_list.size(), name + " -> prim(" + to_string(start) +")");
    vec<bool> in_tree(adj_list.size(), false);
    int       e_num = 0;

    in_tree[start] = true;
    for (const Edge& e : adj_list[start])
        pq.push(e);

    while(!pq.empty()) // Σ ≤ E 
    {
        if (e_num >= in_tree.size() - 1)
            break;

        Edge e = pq.top(); pq.pop(); // O(log E)

        // В одной из предыдущих итераций "while()"
        //  извлеченное ребро называелось "e".
        // 
        // Тогда же вершина "e.to" (1) была добавлена в
        //  дерево: "tree.add_ue(e); in_tree[e.to] = true".
        //   Затем все ребра (1)->(2), соединяющие с
        //    потенциальными новыми вершинами, отправились
        //     в очередь: "pq.push(ee)".
        // 
        // Сейчас очередное ребро "e", извлеченное из очереди,
        //  соединяет вершины "e.from" (1) -> "e.to" (2).
        // 
        // Значит, теперь гарантировано "in_tree[e.from] == true".
        //  Как и в любой другой итерации: по индукции,
        //   начиная со "start".

        if (in_tree[e.to])
            continue; // *
        else
        {
            e_num += 1;
            tree.add_ue(e);
            in_tree[e.to] = true; // *

            for (Edge ee : adj_list[e.to]) //* Σ ≤ E
                if (!in_tree[ee.to])
                    pq.push(ee);       // O(log E)
        }
    }

    return tree;
}

bool Edge_greater::operator() (const Edge& e, const Edge& ee) const {
    return e.cost > ee.cost;
}


// Growing a forest.
//  Capable of traversing a disconnected graph.
//   O(V^2 + E * log E)
//    - Graph_W tree(v_num)   -> O(V^2)
//    - T_kruskal t(adj_list) -> O(V + E * log E)
Graph_W Graph_W::kruskal() {

    // // O(V^2 + EV)
    // if (!is_undirected())
    //     throw(
    //         "Graph_W \"" + name + "\"" + " is directed. \n"
    //         + "kruskal() did not execute \n");

    // O(V^2)
    Graph_W   tree(adj_list.size(), name + " -> kruskal()");

    // O(V + E * log E)
    T_kruskal t(adj_list);

    // O(E)
    for (int i = 0; i < t.edges.size(); ++i) {
        if(t.attach(t.edges[i]))
            tree.add_ue(t.edges[i]);
    }

    return tree;
}


ostream& operator<<(ostream& os, const Graph_W& g) {
    os << (g.is_undirected() ? "Undirected" : "Directed")
        << " Graph_W \'" << g.name << "\'\n\n";
    for (int i = 0; i < g.adj_list.size(); ++i) {
        os << " " << i << " -> ";
        for (int j = 0; j < g.adj_list[i].size(); ++j) {
            const Edge& e = g.adj_list[i][j];
            os << e.to << '(' << e.cost << ") ";
        }
        os << '\n';
    }
    return os << '\n';
}
