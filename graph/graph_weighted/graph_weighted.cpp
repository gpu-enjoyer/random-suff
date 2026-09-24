
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
T_kruskal::T_kruskal(const int v_num) {
    edges.clear();
    parent.assign(v_num, v0);
}
void T_kruskal::reset(const vec<vec<Edge>>& adj_list) {
    for (int i = 0; i < adj_list.size(); ++i)
        parent[i] = i;
    for (const vec<Edge>& vec_e : adj_list)
        for (const Edge& e : vec_e)
            if (e.to < e.from)
                edges.push_back(e);
    sort(edges.begin(), edges.end(), edge_shorter);
}
int T_kruskal::find(const int v) {
    // Path compression: p[v] = f(pp[v]) = ff(ppp[v]) = ... = v
    return parent[v] == v ? v : parent[v] = find(parent[v]);
}

bool T_kruskal::attach(const Edge& e) {
    if (find(e.from) != find(e.to)) {
        parent[find(e.from)] = find(e.to);
        return true;
    }
    return false;
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


// bool Graph_W::is_non_negative() const {
//     ;
// }

// O(?)
//  Expensive
bool Graph_W::is_undirected() const {
    vec<bool> checked(has_edge.size(), false);
    for (int i = 0; i < has_edge.size(); ++i) {
        for (int j = 0; j < has_edge.size(); ++j) {
            if (checked[j])
                continue;
            if (has_edge[i][j] != has_edge[j][i])
                return false;
            int cost_i_j = 0;
            int cost_j_i = 0;
            for (const Edge& e : adj_list[i])
                if (e.to == j) {
                    cost_i_j = e.cost;
                    break;
                }
            for (const Edge& e : adj_list[j])
                if (e.to == i) {
                    cost_j_i = e.cost;
                    break;
                }
            if (cost_i_j != cost_j_i)
                return false;
        }
        checked[i] = true;
    }
    return true;
}


//* O(V + E * log E) amort.
void Graph_W::dijkstra(T_dijkstra& t, const int start) {

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
Graph_W Graph_W::prim(const int start) {

    if (!is_undirected())
        throw("Graph_W \"" + name + "\"" + "is directed. prim() did not execute");

    priority_queue<Edge, vec<Edge>, Edge_greater> pq;
    Graph_W   tree(adj_list.size(), name + " -> prim(" + to_string(start) +")");
    vec<bool> in_tree(adj_list.size(), false);
    int       e_num = 0;

    in_tree[start] = true;
    for (const Edge& e : adj_list[start])
        pq.push(e);

    while(!pq.empty()) {
        if (e_num >= in_tree.size() - 1)
            break;
        Edge e = pq.top(); pq.pop();

        // В одной из предыдущих итераций "while()"
        //  извлеченное ребро называелось "e".
        // 
        // Тогда же вершина (1) "e.to" была добавлена в
        //  дерево: "in_tree[e.to] = true".
        //   Затем все ребра (1)->(2), соединяющие с
        //    потенциальными новыми вершинами, отправились
        //     в очередь: "pq.push(ee)".
        // 
        // Сейчас очередное ребро "e", извлеченное из очереди,
        //  соединяет вершины (1) "e.from" -> (2) "e.to".
        // 
        // Значит, теперь "in_tree[e.from] == true".
        //  Как и в любой другой итерации: по индукции,
        //   начиная со "start".

        if (in_tree[e.to])
            continue;
        else {
            e_num += 1;
            tree.add_ue(e);
            in_tree[e.to] = true;
            for (Edge ee : adj_list[e.to])
                if (!in_tree[ee.to])
                    pq.push(ee);
        }
    }

    return tree;
}

bool Edge_greater::operator() (const Edge& e, const Edge& ee) const {
    return e.cost > ee.cost;
}


// Growing a forest.
//  Capable of traversing a disconnected graph.
Graph_W Graph_W::kruskal() {

    if (!is_undirected())
        throw("Graph_W \"" + name + "\"" + "is directed. kruskal() did not execute");

    Graph_W   tree(adj_list.size(), name + " -> kruskal()");
    T_kruskal t(adj_list.size());

    t.reset(adj_list);
    
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
