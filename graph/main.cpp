
#include "graph/graph.hpp"
#include "graph_weighted/graph_weighted.hpp"

int main()
{
    Graph     g;
    T_bfs     t_bfs;
    T_topsort t_top;

    g.demo();
    cout << g << "\n";
    g.bfs(t_bfs);
    cout << t_bfs << "\n";
    g.topsort(t_top);
    cout << t_top << "\n\n";

    g.demo_cyclic();
    cout << g << "\n";
    g.bfs(t_bfs);
    cout << t_bfs << "\n";
    g.topsort(t_top);
    cout << t_top << "\n\n";


    Graph_W    gg;
    T_dijkstra t_dij;

    // Directed graph
    gg.demo("gg");
    cout << gg;

    gg.dijkstra(t_dij);
    cout << t_dij;

    // Undirected graph
    gg.demo_undirected("gg");
    cout << gg;

    gg.dijkstra(t_dij);
    cout << t_dij;

    Graph_W gg_prim = gg.prim(0);
    cout << gg_prim;

    Graph_W gg_kruskal = gg.kruskal();
    cout << gg_kruskal;

    return 0;
}
